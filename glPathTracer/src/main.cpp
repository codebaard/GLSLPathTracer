#include <glfwHandler.h> //does all the glfw Work
#include <shader.h>
#include <list>
#include <Model.h>
#include <SSBO.h>
#include <Rendermesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <jLog.h>
#include <Framebuffer.h>
#include <cliHandler.h>

#if _DEBUG
#include <direct.h> //for _getcwd()
#endif


int main(int argc, char* argv[]) {
#pragma region InitializeProgram
    jLog::Instance()->ConfigLogger(DATETIME24HWITHSECONDS, DEBUGGING, CONSOLE);

    //Objects
    cliHandler* cli;
    glfwHandler* Application;

    //scene objects
    Model* SceneModel;
    PointLight* pl = new PointLight(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(100.0f, 100.0f, 100.0f));

    //transform matrices
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    //start program
    try {
        cli = new cliHandler(argc, argv);
    }
    catch (std::exception e) {
        jLog::Instance()->Error(std::string(e.what()));
#if NDEBUG
        system("pause");
#endif
        return -1;
    }

#if _DEBUG //path structure in debug is different
    char* cwd; //current path
    char buf[_MAX_PATH];
    cwd = _getcwd(buf, strlen(buf));
    cli->CWD = cwd;
#endif

    try {
        Application = new glfwHandler(cli->ScreenWidth, cli->ScreenHeight);
        Application->FPVCam(true);
    }
    catch (std::exception e) {
        jLog::Instance()->Error("Couldn't create openGL-Context. Reason:\n");
        jLog::Instance()->Error(std::string(e.what()));
#if NDEBUG
        system("pause");
#endif
        return -1;
    }

#pragma endregion


#pragma region Load Scene Data


    try {
#if _DEBUG
        SceneModel = new Model(cli->FilePathToModel + cli->ModelName);
#elif NDEBUG
        SceneModel = new Model(cli->FilePathToModel);
#endif

    }
    catch (std::exception e) {
        jLog::Instance()->Error(std::string("Model loading failed: ") + e.what());
#if NDEBUG
        system("pause");
#endif
        return -1;
    }

#pragma endregion

#pragma region Create OpenGL Objects

    RenderShader* Renderer = new RenderShader();

    Framebuffer* IntrermediateTexture = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);
    Framebuffer* PostProcessingPipeline = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);

    try {
        Renderer->AddShaderToPipeline(cli->CWD, "\\shader\\pbrShader.vert", VERTEX);
        Renderer->AddShaderToPipeline(cli->CWD, "\\shader\\pbrShader.frag", FRAGMENT);

        IntrermediateTexture->SetShaderProgram(cli->CWD, "\\shader\\fsQuadShader.vert", "\\shader\\fsQuadShader.frag");
        PostProcessingPipeline->SetShaderProgram(cli->CWD, "\\shader\\fsQuadShader.vert", "\\shader\\GammaHDR.frag");

        Renderer->InitShader();
    }
    catch (const std::exception& e) {
        jLog::Instance()->Error(std::string("Shader build Error. Message: ") + e.what());
#if NDEBUG
        system("pause");
#endif
        return -1;
    }

#pragma endregion

#pragma region Renderloop
    jLog::Instance()->Log(INFO, "Starting Renderloop...");
    int frame = 0;

    while (!Application->WindowShouldClose()) {

        Application->BeginRenderLoop();

        // #### Render code start here
        //calculate PVM-Matrices
        projection = glm::perspective(glm::radians(glfwHandler::giveCamera()->camera.Zoom), (float)cli->ScreenWidth / (float)cli->ScreenHeight, 0.1f, 1000.0f);
        view = glfwHandler::giveCamera()->camera.GetViewMatrix();
        model = glm::mat4(1.0f); //init everytime
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        Renderer->use();
        Renderer->setMat4fv("projection", glm::value_ptr(projection));
        Renderer->setMat4fv("model", glm::value_ptr(model));
        Renderer->setMat4fv("view", glm::value_ptr(view));

        Renderer->setVec3fv("camPos", glm::value_ptr(glfwHandler::giveCamera()->camera.Position));
        Renderer->setLight(pl);

        // ### Rendering + Post processing ###
        IntrermediateTexture->EnableRenderToTexture(); //Draw call renders to "IntrermediateTexture-Buffer"          

        SceneModel->Draw(*Renderer); //draw call

        IntrermediateTexture->DisableRenderToTexture();

        //Post-Processing Pipeline for HDR LinearSpace -> sRGB Gamma
        PostProcessingPipeline->EnableRenderToTexture(); //Draw call renders to "PostProcessing-Buffer"
        IntrermediateTexture->ShowRenderedTexture(); // draw call
        PostProcessingPipeline->DisableRenderToTexture();
        PostProcessingPipeline->ShowRenderedTexture();

        // #### Rendering finished. 
        Application->ConcludeRenderLoop();

    }
#pragma endregion

#pragma region End Program
    Application->Terminate();

    //clean up
    delete PostProcessingPipeline;
    delete IntrermediateTexture;
    delete cli;
    delete Application;
    delete SceneModel;
    delete pl;
    delete Renderer;

    return 0;
#pragma endregion
}

