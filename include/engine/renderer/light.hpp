#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <string>

enum LightType
{
    DIRECTIONAL,
    POINT,
    SPOT
};

class Light
{
public:
    LightType Type; // Tipo de luz
    glm::vec3 Position; // Posição (relevante para luzes pontuais e spotlights)
    glm::vec3 Direction; // Direção (relevante para luzes direcionais e spotlights)
    glm::vec3 Color; // Cor da luz
    float Intensity; // Intensidade da luz
    float CutOff; // Ângulo do cone interno (relevante para spotlights)
    float OuterCutOff; // Ângulo do cone externo (relevante para spotlights)

    // Atenuação para luzes pontuais
    float Constant;
    float Linear;
    float Quadratic;

    Light(LightType type = DIRECTIONAL, glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f)
        : Type(type), Color(color), Intensity(intensity), Position(glm::vec3(0.0f)), Direction(glm::vec3(0.0f, -1.0f, 0.0f)), 
          CutOff(glm::cos(glm::radians(12.5f))), OuterCutOff(glm::cos(glm::radians(15.0f))),
          Constant(1.0f), Linear(0.09f), Quadratic(0.032f)
    {
    }

    glm::vec3 GetEffectiveColor() const
    {
        return Color * Intensity;
    }

    void SetPosition(const glm::vec3& position)
    {
        Position = position;
    }

    void SetDirection(const glm::vec3& direction)
    {
        Direction = glm::normalize(direction);
    }

    void SetAttenuation(float constant, float linear, float quadratic)
    {
        Constant = constant;
        Linear = linear;
        Quadratic = quadratic;
    }

    void SetSpotlightCutOff(float innerAngle, float outerAngle)
    {
        CutOff = glm::cos(glm::radians(innerAngle));
        OuterCutOff = glm::cos(glm::radians(outerAngle));
    }

    std::string GetLightTypeAsString() const
    {
        switch (Type)
        {
        case DIRECTIONAL: return "Directional";
        case POINT: return "Point";
        case SPOT: return "Spot";
        default: return "Unknown";
        }
    }
};

#endif
