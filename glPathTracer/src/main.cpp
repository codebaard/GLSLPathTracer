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
    Model* SceneModel;
    Rendermesh* Faces;
    PointLight* pl;

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

    try {
        SceneModel = new Model(cli->FilePathToModel + cli->ModelName);

        Faces = new Rendermesh();
        Faces->ParseModelData(SceneModel);
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

    ComputeShader* CompShader = new ComputeShader();

    Framebuffer* DisplayRoutine = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);
    Framebuffer* PostProcessingPipeline = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);

    testSSBO* ssbo = new testSSBO();

    try {
        CompShader->AddShaderToPipeline(cli->CWD, "\\shader\\compShader", COMPUTE);

        DisplayRoutine->SetShaderProgram(cli->CWD, "\\shader\\fsQuadShader.vert", "\\shader\\fsQuadShader.frag");
        PostProcessingPipeline->SetShaderProgram(cli->CWD, "\\shader\\fsQuadShader.vert", "\\shader\\GammaHDR.frag");

        CompShader->InitShader();
    }
    catch (const std::exception& e) {
        jLog::Instance()->Error(std::string("Shader build Error. Message: ") + e.what());
#if NDEBUG
        system("pause");
#endif
        return -1;
    }

    try {
        ssbo->FillBuffer();
    }
    catch (std::exception e) {
        jLog::Instance()->Error(e.what());
    }


#pragma endregion

#pragma region Renderloop
    jLog::Instance()->Log(INFO, "Starting Renderloop...");
    int frame = 0;

    while (!Application->WindowShouldClose()) {

        Application->BeginRenderLoop();

        // #### Render code start here

        //calculate PVM-Matrices
        glm::mat4 projection = glm::perspective(glm::radians(glfwHandler::giveCamera()->camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = glfwHandler::giveCamera()->camera.GetViewMatrix();

        glm::mat4 model = glm::mat4(1.0f); //place model in world origin
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        //prepare transformation matrices


        // ### Rendering + Post processing ###        

        ssbo->LoadBuffer();

        

        //CompShader->use();
        //CompShader->setFloat("roll", (float)frame++ * 0.01f);
        //
        //DisplayRoutine->ActivateImageTexture(); //do this immediately before dispatching computeShader!
        //CompShader->DispatchCompute(512 / 16, 512 / 16, 1);

        //DisplayRoutine->FetchTexture();
        //DisplayRoutine->ShowRenderedTexture();

        // #### Rendering finished. 

        Application->ConcludeRenderLoop();

    }
#pragma endregion

#pragma region End Program
    Application->Terminate();

    //clean up
    delete PostProcessingPipeline;
    delete DisplayRoutine;
    delete Application;

    delete pl;

    return 0;
#pragma endregion
}

