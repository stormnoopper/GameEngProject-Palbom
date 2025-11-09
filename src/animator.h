#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/animation.h> // Provides Animation, AssimpNodeData, Bone, etc.
#include <learnopengl/model_animation.h> // Provides Model

class Animator
{
public:
    // Construct with an optional initial animation
    explicit Animator(Animation* animation = nullptr)
        : m_CurrentAnimation(animation),
          m_CurrentTime(0.0f),
          m_DeltaTime(0.0f),
          m_Looping(true)
    {
        // Default bone palette size (matches common shader arrays)
        m_FinalBoneMatrices.resize(100, glm::mat4(1.0f));
    }

    // Advance time and recompute bone transforms
    void UpdateAnimation(float dt);

    // Start playing an animation from t = 0 (keeps current looping mode)
    void PlayAnimation(Animation* pAnimation);

    // Switch to another animation, optionally preserving current time
    // If resetTime == true: start at t = 0, else keep m_CurrentTime
    void SwitchAnimation(Animation* pAnimation, bool resetTime = true);

    // Accessors for time and animation
    float GetCurrentTime() const { return m_CurrentTime; }
    void  SetCurrentTime(float t) { m_CurrentTime = (t < 0.0f ? 0.0f : t); }

    Animation* GetCurrentAnimation() const { return m_CurrentAnimation; }

    // Loop control
    void SetLooping(bool shouldLoop) { m_Looping = shouldLoop; }
    bool IsLooping() const { return m_Looping; }

    // Final matrices for the shader
    const std::vector<glm::mat4>& GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

private:
    void CalculateBoneTransform(const AssimpNodeData& node, const glm::mat4& parentTransform);

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;

    Animation* m_CurrentAnimation = nullptr;
    float      m_CurrentTime      = 0.0f;
    float      m_DeltaTime        = 0.0f;
    bool       m_Looping          = true;
};
