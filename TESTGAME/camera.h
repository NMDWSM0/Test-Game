#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 摄像机移动的方式，有前进后退、向左向右四种，用WASD键控制
enum Camera_Movement {
    FORWARD = 1,
    BACKWARD = 2,
    LEFT = 3,
    RIGHT = 4,
    UP = 5,
    DOWN = 6,
};

// 默认摄像机参数
// 偏航角
const float YAW = -90.0f;
// 俯仰角
const float PITCH = 0.0f;
// 摄像机移动速度
const float SPEED = 2.5f;
// 欧拉角转动灵敏度
const float SENSITIVITY = 0.1f;
// FOV角度
const float ZOOM = 45.0f;


// 一个用于在OPENGL中计算ViewMatrix的抽象摄像机类
class Camera
{
public:
    /* 摄像机属性 */
    // 是否锁定
    bool locked = false;
    // 位置
    glm::vec3 Position;
    // 观察方向
    glm::vec3 Front;
    // 摄像机上方向
    glm::vec3 Up;
    // 摄像机右方向
    glm::vec3 Right;
    // 世界上方向：一般为y轴正方向
    glm::vec3 WorldUp;

    /* 欧拉角 */
    // 偏航角
    float Yaw;
    // 俯仰角
    float Pitch;

    /* 摄像机参数 */
    // 摄像机移动速度
    float MovementSpeed;
    // 欧拉角转动灵敏度
    float MouseSensitivity;
    // FOV角度
    float Zoom;

protected:
    // 根据欧拉角变化计算摄像机面对和上方向
    virtual void updateCameraVectors();

public:

    // 构造摄像机对象（使用向量）
    //\param position: 摄像机初始位置，默认为 vec3(0.0f, 0.0f, 0.0f)
    //\param up: 世界上方向，默认为y轴正方向 vec3(0.0f, 1.0f, 0.0f)
    //\param yaw: 初始偏航角，默认为 -90度
    //\param pitch: 初始俯仰角，默认为 0度
    //\param yaw和pitch的初始值对应初始Front方向为z轴负方向 vec3(0.0f, 0.0f, -1.0f)
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // 构造摄像机对象（使用值）
    //\param posX, posY, posZ: 摄像机初始位置，默认为 0.0f, 0.0f, 0.0f
    //\param upX, upY, upZ: 世界上方向，默认为y轴正方向 0.0f, 1.0f, 0.0f
    //\param yaw: 初始偏航角，默认为 -90度
    //\param pitch: 初始俯仰角，默认为 0度
    //\param yaw和pitch的初始值对应初始Front方向为z轴负方向 vec3(0.0f, 0.0f, -1.0f)
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // 返回ViewMatrix
    glm::mat4 GetViewMatrix();

    // 处理键盘事件：WASD控制摄像机移动
    //\param direction: 摄像机移动方向
    //\param deltaTime: 按键按下时间，用于计算移动距离
    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // 处理鼠标事件：鼠标移动控制俯仰角和偏航角变化
    //\param xoffset, yoffset: 鼠标偏移量
    //\param constrainPitch: 摄像机是否突破限制，默认为是
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // 处理鼠标事件：鼠标滚轮控制缩放（FOV缩放）
    //\param yoffset: 滚轮y偏移量
    virtual void ProcessMouseScroll(float yoffset);
    
    //锁定摄像机到2D空间
    virtual void LockToZFront(glm::vec3 defaultpos = glm::vec3(0.0f, 0.0f, 3.0f));

    //解锁摄像机
    virtual void Unlock();

    //外部调用强制更新相机
    void UpdateCamera()
    {
        updateCameraVectors();
    }
};

//
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
    //return glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    if (locked)
        return;
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += WorldUp * velocity;
    if (direction == DOWN)
        Position -= WorldUp * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    if (locked)
        return;
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // 设置上/下限，防止垂直时出现问题
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // 欧拉角变化，更新摄像机面对方向和上方向
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (locked)
        return;
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 60.0f)
        Zoom = 60.0f;
}

void Camera::updateCameraVectors()
{
    // 计算新的Front向量
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // 计算新的Right和Up向量
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::LockToZFront(glm::vec3 defaultpos)
{
    locked = true;

    Position = defaultpos;
    Yaw = -90.0f;
    Pitch = 0.0f;

    updateCameraVectors();
}

void Camera::Unlock()
{
    locked = false;
}

#endif