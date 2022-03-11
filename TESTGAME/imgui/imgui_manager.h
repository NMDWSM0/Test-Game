#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class ImGuiManager
{
	GLFWwindow* window;
public:
	ImGuiManager(GLFWwindow* window);
	void NewFrame();
	void DrawFrame();
	void Terminate();
};