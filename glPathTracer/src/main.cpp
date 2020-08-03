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

    //ComputeShader* CompShader = new ComputeShader();
    ComputeShader* TransformShader = new ComputeShader();
    TransformShader->SetInternalArraySize(Faces->Facecount); //runtime change to GLSL-Code

    //Framebuffer* DisplayRoutine = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);
    //Framebuffer* PostProcessingPipeline = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);

    TransformSSBO* Transform = new TransformSSBO();
    RendermeshSSBO* Rendermesh = new RendermeshSSBO();

    try {
        TransformShader->AddShaderToPipeline(cli->CWD, "\\shader\\MVPVertexShader.comp", COMPUTE);

        //DisplayRoutine->SetShaderProgram(cli->CWD, "\\shader\\fsQuadShader.vert", "\\shader\\fsQuadShader.frag");
        //PostProcessingPipeline->SetShaderProgram(cli->CWD, "\\shader\\fsQuadShader.vert", "\\shader\\GammaHDR.frag");

        //CompShader->InitShader();
        TransformShader->InitShader();
    }
    catch (const std::exception& e) {
        jLog::Instance()->Error(std::string("Shader build Error. Message: ") + e.what());
#if NDEBUG
        system("pause");
#endif
        return -1;
    }

    try {
        Transform->FillBuffer(projection, view, model);
        Rendermesh->FillBuffer(Faces);
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
        projection = glm::perspective(glm::radians(glfwHandler::giveCamera()->camera.Zoom), (float)cli->ScreenWidth / (float)cli->ScreenHeight, 0.1f, 1000.0f);
        view = glfwHandler::giveCamera()->camera.GetViewMatrix();
        model = glm::mat4(1.0f); //init everytime
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        //prepare transformation matrices
        Transform->RefreshBuffer(projection, view, model);

        // ### Rendering + Post processing ###    
        TransformShader->use();
        TransformShader->DispatchCompute(Faces->Facecount, 1, 1);

        Rendermesh->ReadBuffer();
        std::string str = std::to_string(Rendermesh->Mesh->Faces->v1.x);
        jLog::Instance()->Log(INFO, str);

        //CompShader->use();
        //CompShader->setFloat("roll", (float)frame++ * 0.01f);
        //
        //DisplayRoutine->ActivateImageTexture(); //do this immediately before dispatching computeShader!
        //CompShader->DispatchCompute(512 / 16, 512 / 16, 1);

        // #### Rendering finished. 
        Application->ConcludeRenderLoop();

    }
#pragma endregion

#pragma region End Program
    Application->Terminate();

    //clean up
    //delete PostProcessingPipeline;
    //delete DisplayRoutine;
    delete Application;

    return 0;
#pragma endregion
}

