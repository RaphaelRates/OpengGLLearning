#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Enum para definir movimentos possíveis da câmera
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    NONE
};

// Valores padrão da câmera
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// Classe que abstrai os cálculos da câmera
class Camera
{
public:
    // Atributos da câmera
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Ângulos de Euler
    float Yaw;
    float Pitch;
    // Opções da câmera
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // Controle de movimento
    bool IsMoving = false;
    bool isRunning = false;
    float WalkCycle = 0.0f;

    // Construtores
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Retorna a matriz de visão calculada usando Euler Angles e LookAt
    glm::mat4 GetViewMatrix()
{
    glm::vec3 adjustedPosition = Position;

    if (IsMoving)
    {
        WalkCycle += MovementSpeed * (isRunning ? 0.01f: 0.005f);
        if (WalkCycle > 2.0f * glm::pi<float>())
            WalkCycle -= 2.0f * glm::pi<float>();

        float verticalAmplitude = 0.04f;
        float horizontalAmplitude = 0.00f;
        float tiltAmplitude = 0.01f;

        adjustedPosition.y += cos(WalkCycle) * verticalAmplitude;
        adjustedPosition.x += sin(WalkCycle * 2.0f) * horizontalAmplitude;

        glm::vec3 target = adjustedPosition + Front;
        float tilt = sin(WalkCycle) * tiltAmplitude;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(tilt), Up);
        target = glm::vec3(rotation * glm::vec4(target - adjustedPosition, 1.0f)) + adjustedPosition;

        return glm::lookAt(adjustedPosition, target, Up);
    }

    // Quando não há movimento, suaviza a parada do ciclo de caminhada
    WalkCycle *= 0.9f;  // Suaviza a desaceleração, reduzindo o WalkCycle gradualmente
    return glm::lookAt(adjustedPosition, adjustedPosition + Front, Up);
}

// Processa a entrada de teclado
void
ProcessKeyboard(Camera_Movement direction, float deltaTime,bool isRunnig)
{
    float velocity = MovementSpeed * deltaTime;
    bool isRunngin = isRunnig;
    IsMoving = false;

    if (direction == FORWARD)
    {
        Position += Front * velocity;
        IsMoving = true;
    }
    if (direction == BACKWARD)
    {
        Position -= Front * velocity;
        IsMoving = true;
    }
    if (direction == LEFT)
    {
        Position -= Right * velocity;
        IsMoving = true;
    }
    if (direction == RIGHT)
    {
        Position += Right * velocity;
        IsMoving = true;
    }
    if (direction == NONE)
    {
        IsMoving = false; // Não há movimento, animação de caminhada é interrompida
    }

    Position.y = 0.0f;
}

// Processa a entrada do mouse
void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

// Processa a entrada da roda do mouse
void ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

private:
// Atualiza os vetores da câmera com base nos ângulos de Euler
void updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
}
;

#endif
