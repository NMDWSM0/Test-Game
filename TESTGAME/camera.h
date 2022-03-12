#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// ������ƶ��ķ�ʽ����ǰ�����ˡ������������֣���WASD������
enum Camera_Movement {
    FORWARD = 1,
    BACKWARD = 2,
    LEFT = 3,
    RIGHT = 4,
    UP = 5,
    DOWN = 6,
};

// Ĭ�����������
// ƫ����
const float YAW = -90.0f;
// ������
const float PITCH = 0.0f;
// ������ƶ��ٶ�
const float SPEED = 2.5f;
// ŷ����ת��������
const float SENSITIVITY = 0.1f;
// FOV�Ƕ�
const float ZOOM = 45.0f;


// һ��������OPENGL�м���ViewMatrix�ĳ����������
class Camera
{
public:
    /* ��������� */
    // �Ƿ�����
    bool locked = false;
    // λ��
    glm::vec3 Position;
    // �۲췽��
    glm::vec3 Front;
    // ������Ϸ���
    glm::vec3 Up;
    // ������ҷ���
    glm::vec3 Right;
    // �����Ϸ���һ��Ϊy��������
    glm::vec3 WorldUp;

    /* ŷ���� */
    // ƫ����
    float Yaw;
    // ������
    float Pitch;

    /* ��������� */
    // ������ƶ��ٶ�
    float MovementSpeed;
    // ŷ����ת��������
    float MouseSensitivity;
    // FOV�Ƕ�
    float Zoom;

protected:
    // ����ŷ���Ǳ仯�����������Ժ��Ϸ���
    virtual void updateCameraVectors();

public:

    // �������������ʹ��������
    //\param position: �������ʼλ�ã�Ĭ��Ϊ vec3(0.0f, 0.0f, 0.0f)
    //\param up: �����Ϸ���Ĭ��Ϊy�������� vec3(0.0f, 1.0f, 0.0f)
    //\param yaw: ��ʼƫ���ǣ�Ĭ��Ϊ -90��
    //\param pitch: ��ʼ�����ǣ�Ĭ��Ϊ 0��
    //\param yaw��pitch�ĳ�ʼֵ��Ӧ��ʼFront����Ϊz�Ḻ���� vec3(0.0f, 0.0f, -1.0f)
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // �������������ʹ��ֵ��
    //\param posX, posY, posZ: �������ʼλ�ã�Ĭ��Ϊ 0.0f, 0.0f, 0.0f
    //\param upX, upY, upZ: �����Ϸ���Ĭ��Ϊy�������� 0.0f, 1.0f, 0.0f
    //\param yaw: ��ʼƫ���ǣ�Ĭ��Ϊ -90��
    //\param pitch: ��ʼ�����ǣ�Ĭ��Ϊ 0��
    //\param yaw��pitch�ĳ�ʼֵ��Ӧ��ʼFront����Ϊz�Ḻ���� vec3(0.0f, 0.0f, -1.0f)
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // ����ViewMatrix
    glm::mat4 GetViewMatrix();

    // ��������¼���WASD����������ƶ�
    //\param direction: ������ƶ�����
    //\param deltaTime: ��������ʱ�䣬���ڼ����ƶ�����
    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // ��������¼�������ƶ����Ƹ����Ǻ�ƫ���Ǳ仯
    //\param xoffset, yoffset: ���ƫ����
    //\param constrainPitch: ������Ƿ�ͻ�����ƣ�Ĭ��Ϊ��
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // ��������¼��������ֿ������ţ�FOV���ţ�
    //\param yoffset: ����yƫ����
    virtual void ProcessMouseScroll(float yoffset);
    
    //�����������2D�ռ�
    virtual void LockToZFront(glm::vec3 defaultpos = glm::vec3(0.0f, 0.0f, 3.0f));

    //���������
    virtual void Unlock();

    //�ⲿ����ǿ�Ƹ������
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

    // ������/���ޣ���ֹ��ֱʱ��������
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // ŷ���Ǳ仯�������������Է�����Ϸ���
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
    // �����µ�Front����
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // �����µ�Right��Up����
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