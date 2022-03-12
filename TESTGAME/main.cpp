#define _CRT_SECURE_NO_WARNINGS
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "shader.h"
#include "followcamera.h"
#include "input.h"
#include "world.h"
#include "background.h"
#include <GLFW/glfw3.h>
#include "imgui/imgui_manager.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//启动独显渲染
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//摄像机，这里定义在外部
FollowCamera* TheCamera;
//输入，定义在外部
Input* TheInput;
//玩家，定义在外部
Player* ThePlayer;
//世界
World* TheWorld;
//背景
BackGround* TheBackGround;

GAMEMODE gameon = OFF;

std::vector<std::string> worldfiles =
{
    "assets/worlds/level1.txt",
    "assets/worlds/level2.txt",
    "assets/worlds/level3.txt",
    "assets/worlds/level4.txt",
    "assets/worlds/level5.txt",
};

unsigned int worldnumber = 5;
unsigned int currentworldlevel = 0;
unsigned int passedlevel = 0;

int main()
{
    srand(time(0));
    //glfw初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16); //MSAA抗锯齿

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(1280, 960, "TESTGAME", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //当帧缓冲区大小改变时，改变视区大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad接管opengl函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //读取数据
    FILE* datafile = fopen("profile/gameprogress.txt", "r");
    if (datafile != nullptr)
    {
        fscanf(datafile, "%d", &passedlevel);
        fclose(datafile);
    }

    //创建玩家类
    ThePlayer = new Player("assets/testplayer.txt", "assets/images/testplayer.png");
    //创建输入类
    TheInput = new Input(window, ThePlayer);
    //创建摄像机类
    TheCamera = new FollowCamera(ThePlayer);
    TheCamera->LockToZFront();
    //创建世界
    TheWorld = new World();
    //设置背景
    TheBackGround = new BackGround();

    //加载着色器
    Shader mainshader("shaders/mainshader/main.vert", "shaders/mainshader/main.frag");
    Shader bgshader("shaders/bgshader/bg.vert", "shaders/bgshader/bg.frag");

    //初始化ImGui（使用自定义类）
    ImGuiManager imgui(window);

    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float currentFrame = glfwGetTime();
    float lastFrame = currentFrame;
    float deltaTime = currentFrame - lastFrame;
    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwSetWindowTitle(window, ("TESTGAME | " + std::to_string(ImGui::GetIO().Framerate) + "fps").c_str());

        glClearColor(0.1f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //创建新ImGui帧
        imgui.NewFrame();
        TheInput->ProcessInput(deltaTime);

        if (gameon == ON)
        {
            if (!TheWorld->HasLoaded())
            {
                TheWorld->LoadWorld(worldfiles[currentworldlevel]);
                ThePlayer->OnBecameHuman();
            }

            //世界更新
            TheWorld->UpdateEntities(deltaTime);

            //更新相机位置
            TheCamera->UpdateCameraPos(TheWorld->GetWorldSize());
            glm::vec3 uCameraPos = TheCamera->Position;
            //更新view矩阵
            glm::mat4 view = TheCamera->GetViewMatrix();
            //更新projection矩阵
            glm::vec2 halfsize = TheCamera->GetHalfViewSize();
            glm::mat4 projection = glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y, 1.0f, 35.0f);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            /***************************************/
            //关闭深度测试
            glDisable(GL_DEPTH_TEST);

            bgshader.use();
            TheBackGround->Update(deltaTime);
            TheBackGround->Draw(bgshader);

            /***************************************/
            //开启深度测试
            glEnable(GL_DEPTH_TEST);

            mainshader.use();
            mainshader.setMat("uView", view);
            mainshader.setMat("uProjection", projection);

            TheWorld->Draw(mainshader);
            /***************************************/

        }
        else if (gameon == PAUSE)
        {
            glClearColor(0.4f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            /***************************************/
            //关闭深度测试
            glDisable(GL_DEPTH_TEST);
            bgshader.use();
            TheBackGround->Draw(bgshader);
            ///***************************************/
            ////开启深度测试
            //glEnable(GL_DEPTH_TEST);
            //mainshader.use();
            //TheWorld->Draw(mainshader);
            ///***************************************/
            //UI
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        else if (gameon == OFF || gameon == CHOOSE || gameon == COMPLETE)
        {
            if (TheWorld->HasLoaded())
            {
                TheWorld->QuitWorld();
                ThePlayer->PlayerDeath();
            }
            //UI
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        //imgui.DrawFrame();
        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //施放缓冲区（其实会自动的吧）
    imgui.Terminate();
    //退出
    glfwTerminate();

    //写入数据
    FILE* wdatafile = fopen("profile/gameprogress.txt", "w");
    if (wdatafile != nullptr)
    {
        fprintf(wdatafile, "%d", passedlevel);
        fclose(wdatafile);
    }

    return 0;
}

//这个函数用来调整gl的viewport视区大小
//\param width  视区宽度
//\param height 视区高度
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}