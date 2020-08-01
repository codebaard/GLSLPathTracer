#include <glfwHandler.h> //does all the glfw Work
#include <shader.h>
#include <list>

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
    jLog::Instance()->Log(INFO, "You shouldn't see me in a release build.");
#endif

    try {
        Application = new glfwHandler(cli->ScreenWidth, cli->ScreenHeight);
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

#pragma region Create OpenGL Objects

    ComputeShader* CompShader = new ComputeShader();
    Framebuffer* DisplayRoutine = new Framebuffer();

    try {
        CompShader->AddShaderToPipeline(cli->CWD, "\\shader\\compShader.comp", COMPUTE);
        DisplayRoutine->SetShaderProgram(cli->CWD, "\\shader\\fsQuadShader.vert", "\\shader\\fsQuadShader.frag");

        CompShader->InitShader();

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

        CompShader->use();
        CompShader->setFloat("roll", (float)frame++ * 0.01f);
        
        DisplayRoutine->ActivateImageTexture(); //do this immediately before dispatching computeShader!
        CompShader->DispatchCompute(512 / 16, 512 / 16, 1);

        DisplayRoutine->FetchTexture();
        DisplayRoutine->ShowRenderedTexture();

        // #### Rendering finished. 

        Application->ConcludeRenderLoop();

    }
#pragma endregion

#pragma region End Program
    Application->Terminate();

    //clean up
    delete CompShader;
    delete DisplayRoutine;
    delete Application;

    return 0;
#pragma endregion
}

