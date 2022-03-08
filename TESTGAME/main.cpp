#define _CRT_SECURE_NO_WARNINGS
#include "shader.h"
#include "followcamera.h"
#include "input.h"
#include "world.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>

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

int main()
{
    //glfw初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16); //MSAA抗锯齿

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(960, 960, "TESTGAME", nullptr, nullptr);
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
    
    //创建玩家类
    ThePlayer = new Player("assets/testplayer.txt");
    //创建输入类
    TheInput = new Input(window, ThePlayer);
    //创建摄像机类
    TheCamera = new FollowCamera(ThePlayer);
    //创建玩家
    TheWorld = new World("assets/worlds/testworld.txt");

    //加载着色器
    Shader mainshader("shaders/mainshader/main.vert", "shaders/mainshader/main.frag");

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

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
        TheInput->ProcessInput();
        TheWorld->UpdateEntities(deltaTime);

        //更新相机位置
        TheCamera->UpdateCameraPos();
        glm::vec3 uCameraPos = TheCamera->Position;
        //更新view矩阵
        glm::mat4 view = TheCamera->GetViewMatrix();
        //更新projection矩阵
        glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 25.5f);
        //glm::mat4 projection = glm::perspective(glm::radians(thecam.Zoom), 1.0f * 1280 / 720, 0.1f, 100.0f);

        //渲染指令
        //清除颜色缓冲
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  //先随便写点
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainshader.use();
        mainshader.setMat("uView", view);
        mainshader.setMat("uProjection", projection);
        
        //mainshader.setMat("uModel", glm::mat4(1.0f));
        //m.Draw();
        TheWorld->Draw(mainshader);

        //交换颜色缓冲（双缓冲）
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //施放缓冲区（其实会自动的吧）

    //退出
    glfwTerminate();
    //system("pause");
    return 0;
}

//这个函数用来调整gl的viewport视区大小
//\param width  视区宽度
//\param height 视区高度
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}