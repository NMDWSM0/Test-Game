#include "imgui_manager.h"
#include "../GAMEMODE.h"
#include <string>

extern GAMEMODE gameon;

extern unsigned int worldnumber;
extern unsigned int currentworldlevel;
extern unsigned int passedlevel;

ImGuiManager::ImGuiManager(GLFWwindow* window) :
    window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // 键盘控制
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // 手柄控制？

    // 设置风格
    ImGui::StyleColorsLight();

    ImGuiStyle* style = &ImGui::GetStyle();
    style->FrameRounding = 10.0f;
    style->GrabRounding = 10.0f;
    style->WindowMinSize = ImVec2(200.0f, 350.0f);

    ImFont* font = io.Fonts->AddFontFromFileTTF("assets/fonts/zh-cn.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    // 选择渲染终端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void ImGuiManager::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Menu");

    ImGui::Text("fps : %.1f", ImGui::GetIO().Framerate);

    if (gameon == OFF)
    {
        ImGui::Button("Start Game", ImVec2(180.0f, 60.0f));
        if (ImGui::IsItemClicked())
            gameon = CHOOSE;

        ImGui::Separator();

        ImGui::Button("Exit", ImVec2(180.0f, 60.0f));
        if (ImGui::IsItemClicked())
            glfwSetWindowShouldClose(window, true);
    }

    else if (gameon == PAUSE)
    {
        ImGui::Button("Quit Game", ImVec2(180.0f, 60.0f));
        if (ImGui::IsItemClicked())
            gameon = OFF;

        ImGui::Separator();

        ImGui::Button("Back To Game", ImVec2(180.0f, 60.0f));
        if (ImGui::IsItemClicked())
            gameon = ON;
    }

    else if (gameon == CHOOSE)
    {
        ImGui::Text("Choose Game Level");
        for (int i = 0; i < passedlevel; i++)
        {
            ImGui::Button((std::string("* Level ") + std::to_string(i + 1)).c_str(), ImVec2(100.0f, 40.0f));
            if (ImGui::IsItemClicked())
            {
                currentworldlevel = i;
                gameon = ON;
            }
        }
        if (passedlevel < worldnumber)
        {
            ImGui::Button((std::string("o Level ") + std::to_string(passedlevel + 1)).c_str(), ImVec2(100.0f, 40.0f));
            if (ImGui::IsItemClicked())
            {
                currentworldlevel = passedlevel;
                gameon = ON;
            }
        }
        for (int i = passedlevel + 1; i < worldnumber; i++)
        {
            ImGui::Button((std::string("x Level ") + std::to_string(i + 1)).c_str(), ImVec2(100.0f, 40.0f));
        }

        ImGui::Separator();

        ImGui::Button("Back To Menu", ImVec2(180.0f, 60.0f));
        if (ImGui::IsItemClicked())
            gameon = OFF;
    }

    else if (gameon == COMPLETE)
    {
        if (currentworldlevel == worldnumber - 1)
        {
            ImGui::Text("You Have Won The Game");
            ImGui::Button("Back To Menu", ImVec2(180.0f, 60.0f));
            if (ImGui::IsItemClicked())
                gameon = OFF;
        }
        else
        {
            ImGui::Text(("You Completed Level " + std::to_string(currentworldlevel + 1)).c_str());
            ImGui::Button("Next Level", ImVec2(180.0f, 60.0f));
            if (ImGui::IsItemClicked())
            {
                currentworldlevel += 1;
                gameon = ON;
            }

            ImGui::Separator();

            ImGui::Button("Back To Menu", ImVec2(180.0f, 60.0f));
            if (ImGui::IsItemClicked())
                gameon = OFF;
        }
    }
    
    ImGui::End();
    ImGui::Render();
}

void ImGuiManager::DrawFrame()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Terminate()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}