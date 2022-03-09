#define _CRT_SECURE_NO_WARNINGS
#include "shader.h"
#include "followcamera.h"
#include "input.h"
#include "world.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>

//����������Ⱦ
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//����������ﶨ�����ⲿ
FollowCamera* TheCamera;
//���룬�������ⲿ
Input* TheInput;
//��ң��������ⲿ
Player* ThePlayer;
//����
World* TheWorld;

int main()
{
    srand(time(0));
    //glfw��ʼ��
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16); //MSAA�����

    //��������
    GLFWwindow* window = glfwCreateWindow(1280, 960, "TESTGAME", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //��֡��������С�ı�ʱ���ı�������С
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad�ӹ�opengl����ָ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //���������
    ThePlayer = new Player("assets/testplayer.txt");
    //����������
    TheInput = new Input(window, ThePlayer);
    //�����������
    TheCamera = new FollowCamera(ThePlayer);
    //�������
    TheWorld = new World("assets/worlds/testworld.txt");

    //������ɫ��
    Shader mainshader("shaders/mainshader/main.vert", "shaders/mainshader/main.frag");

    //������Ȳ���
    glEnable(GL_DEPTH_TEST);
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float currentFrame = glfwGetTime();
    float lastFrame = currentFrame;
    float deltaTime = currentFrame - lastFrame;
    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ����
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        TheInput->ProcessInput(deltaTime);
        TheWorld->UpdateEntities(deltaTime);

        //�������λ��
        TheCamera->UpdateCameraPos(TheWorld->GetWorldSize());
        glm::vec3 uCameraPos = TheCamera->Position;
        //����view����
        glm::mat4 view = TheCamera->GetViewMatrix();
        //����projection����
        glm::vec2 halfsize = TheCamera->GetHalfViewSize();
        glm::mat4 projection = glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y, 1.0f, 25.5f);

        //��Ⱦָ��
        //�����ɫ����
        glClearColor(0.4f, 0.3f, 0.8f, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainshader.use();
        mainshader.setMat("uView", view);
        mainshader.setMat("uProjection", projection);
        
        TheWorld->Draw(mainshader);

        //������ɫ���壨˫���壩
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //ʩ�Ż���������ʵ���Զ��İɣ�

    //�˳�
    glfwTerminate();
    //system("pause");
    return 0;
}

//���������������gl��viewport������С
//\param width  �������
//\param height �����߶�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}