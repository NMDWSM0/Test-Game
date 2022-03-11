#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "followcamera.h"
#include "player.h"
#include "GAMEMODE.h"

extern FollowCamera* TheCamera;
extern GAMEMODE gameon;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = 0.0f, lastY = 0.0f;
    if (firstMouse || glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    TheCamera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    TheCamera->ProcessMouseScroll(yoffset);
}

class Input
{
	GLFWwindow* window;
    Player* player;
	GLuint defaultmode;

public:
	Input(GLFWwindow* w, Player* ThePlayer, GLuint cursormode = GLFW_CURSOR_DISABLED);

    void ProcessInput(float deltaT);
};

Input::Input(GLFWwindow* w, Player* ThePlayer, GLuint cursormode):
	window(w), player(ThePlayer), defaultmode(cursormode)
{
	glfwSetInputMode(window, GLFW_CURSOR, defaultmode);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void Input::ProcessInput(float deltaT)
{
    if (gameon != ON)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //glfwSetWindowShouldClose(window, true);
        gameon = PAUSE;

    if (defaultmode == GLFW_CURSOR_DISABLED)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            TheCamera->LockToZFront();
        else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
            TheCamera->Unlock();
    }

    //float currentFrame = glfwGetTime();
    //float deltaTime = currentFrame - lastFrame;
    //lastFrame = currentFrame;

    if (!player->IsStunned())
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player->Up();

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player->Down();

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS)
            player->Left();
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS)
            player->Right();
        else
            player->PauseLR();

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            player->ShootArrow();
    }
    else
    {
        player->PauseLR();
    }

    player->UpdateKeyCD(deltaT);
}

#endif 