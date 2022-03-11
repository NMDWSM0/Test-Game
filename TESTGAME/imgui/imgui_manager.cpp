#include "imgui_manager.h"
#include "../GAMEMODE.h"

extern GAMEMODE gameon;

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
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // 选择渲染终端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void ImGuiManager::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetWindowSize(ImVec2(250.0f, 700.0f));

    ImGui::Begin("Menu");

    ImGui::Text("fps : %.1f", ImGui::GetIO().Framerate);

    if (gameon == OFF)
    {
        ImGui::Button("Start Game", ImVec2(120.0f, 40.0f));
        if (ImGui::IsItemClicked())
            gameon = ON;
        ImGui::Button("Exit", ImVec2(120.0f, 40.0f));
        if (ImGui::IsItemClicked())
            glfwSetWindowShouldClose(window, true);
    }
    else if (gameon == PAUSE)
    {
        ImGui::Button("Quit Game", ImVec2(120.0f, 40.0f));
        if (ImGui::IsItemClicked())
            gameon = OFF;

        ImGui::Button("Back To Game", ImVec2(120.0f, 40.0f));
        if (ImGui::IsItemClicked())
            gameon = ON;
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