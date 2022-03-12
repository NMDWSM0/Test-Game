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
//����
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
    
    //��ȡ����
    FILE* datafile = fopen("profile/gameprogress.txt", "r");
    if (datafile != nullptr)
    {
        fscanf(datafile, "%d", &passedlevel);
        fclose(datafile);
    }

    //���������
    ThePlayer = new Player("assets/testplayer.txt", "assets/images/testplayer.png");
    //����������
    TheInput = new Input(window, ThePlayer);
    //�����������
    TheCamera = new FollowCamera(ThePlayer);
    TheCamera->LockToZFront();
    //��������
    TheWorld = new World();
    //���ñ���
    TheBackGround = new BackGround();

    //������ɫ��
    Shader mainshader("shaders/mainshader/main.vert", "shaders/mainshader/main.frag");
    Shader bgshader("shaders/bgshader/bg.vert", "shaders/bgshader/bg.frag");

    //��ʼ��ImGui��ʹ���Զ����ࣩ
    ImGuiManager imgui(window);

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
        glfwSetWindowTitle(window, ("TESTGAME | " + std::to_string(ImGui::GetIO().Framerate) + "fps").c_str());

        glClearColor(0.1f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //������ImGui֡
        imgui.NewFrame();
        TheInput->ProcessInput(deltaTime);

        if (gameon == ON)
        {
            if (!TheWorld->HasLoaded())
            {
                TheWorld->LoadWorld(worldfiles[currentworldlevel]);
                ThePlayer->OnBecameHuman();
            }

            //�������
            TheWorld->UpdateEntities(deltaTime);

            //�������λ��
            TheCamera->UpdateCameraPos(TheWorld->GetWorldSize());
            glm::vec3 uCameraPos = TheCamera->Position;
            //����view����
            glm::mat4 view = TheCamera->GetViewMatrix();
            //����projection����
            glm::vec2 halfsize = TheCamera->GetHalfViewSize();
            glm::mat4 projection = glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y, 1.0f, 35.0f);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            /***************************************/
            //�ر���Ȳ���
            glDisable(GL_DEPTH_TEST);

            bgshader.use();
            TheBackGround->Update(deltaTime);
            TheBackGround->Draw(bgshader);

            /***************************************/
            //������Ȳ���
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
            //�ر���Ȳ���
            glDisable(GL_DEPTH_TEST);
            bgshader.use();
            TheBackGround->Draw(bgshader);
            ///***************************************/
            ////������Ȳ���
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
        //������ɫ���壨˫���壩
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //ʩ�Ż���������ʵ���Զ��İɣ�
    imgui.Terminate();
    //�˳�
    glfwTerminate();

    //д������
    FILE* wdatafile = fopen("profile/gameprogress.txt", "w");
    if (wdatafile != nullptr)
    {
        fprintf(wdatafile, "%d", passedlevel);
        fclose(wdatafile);
    }

    return 0;
}

//���������������gl��viewport������С
//\param width  �������
//\param height �����߶�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}