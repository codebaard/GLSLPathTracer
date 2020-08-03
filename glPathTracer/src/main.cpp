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
    Rendermesh* myRendermesh;

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
        SceneModel = new Model(cli->FilePathToModel + cli->ModelName);

        myRendermesh = new Rendermesh();
        myRendermesh->ParseModelData(SceneModel);
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
    //TransformShader->SetInternalArraySize(myRendermesh->Facecount); //runtime change to GLSL-Code

    //Framebuffer* DisplayRoutine = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);
    //Framebuffer* PostProcessingPipeline = new Framebuffer(cli->ScreenWidth, cli->ScreenHeight);

    TransformSSBO* TransformBuffer = new TransformSSBO();
    RendermeshSSBO* RendermeshBuffer = new RendermeshSSBO();

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
        TransformBuffer->MapBufferToAdressSpace(TransformShader->ID);
        RendermeshBuffer->MapBufferToAdressSpace(TransformShader->ID);

        TransformBuffer->FillBuffer(projection, view, model);
        RendermeshBuffer->FillBuffer(myRendermesh);
    }
    catch (std::exception e) {
        jLog::Instance()->Error(e.what());
    }

    jLog::Instance()->Log(INFO, "GLM Sizes : float, vec3, mat4");
    jLog::Instance()->Log(INFO, std::string(std::to_string(sizeof(glm::float32))));
    jLog::Instance()->Log(INFO, std::string(std::to_string(sizeof(glm::vec3))));
    jLog::Instance()->Log(INFO, std::string(std::to_string(sizeof(glm::mat4))));

    jLog::Instance()->Log(INFO, "Struct Triangle");
    jLog::Instance()->Log(INFO, std::string(std::to_string(sizeof(Triangle))));

    jLog::Instance()->Log(INFO, "Struct Matrices");
    jLog::Instance()->Log(INFO, std::string(std::to_string(sizeof(Matrices))));

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

        //store PVM-Matrices in GPU Memory
        TransformBuffer->FillBuffer(projection, view, model);
        RendermeshBuffer->FillBuffer(myRendermesh);

        // ### Rendering + Post processing ###    
        TransformShader->use();
        TransformShader->DispatchCompute(myRendermesh->Facecount, 1, 1);
        //TransformShader->DispatchCompute(1, 1, 1);

        RendermeshBuffer->ReadBuffer();
        TransformBuffer->ReadBuffer();

        jLog::Instance()->Log(INFO, RendermeshBuffer->DebugGiveVector());

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
    delete cli;
    delete Application;
    delete SceneModel;
    delete myRendermesh;
    delete TransformShader;
    delete TransformBuffer;
    delete RendermeshBuffer;

    return 0;
#pragma endregion
}

