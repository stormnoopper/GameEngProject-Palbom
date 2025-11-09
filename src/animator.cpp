#include "animator.h"
#include <cmath> // fmodf

void Animator::UpdateAnimation(float dt)
{
    m_DeltaTime = dt;
    if (!m_CurrentAnimation) return;

    const float tps      = m_CurrentAnimation->GetTicksPerSecond();
    const float duration = m_CurrentAnimation->GetDuration();

    // Advance time in animation ticks
    m_CurrentTime += tps * dt;

    // Loop or clamp at the end
    if (m_Looping)
    {
        if (duration > 0.0f)
            m_CurrentTime = std::fmod(m_CurrentTime, duration);
        else
            m_CurrentTime = 0.0f;
    }
    else
    {
        if (m_CurrentTime > duration) m_CurrentTime = duration;
        if (m_CurrentTime < 0.0f)     m_CurrentTime = 0.0f;
    }

    // Recompute the bone transforms for the current time
    m_FinalBoneMatrices.assign(m_FinalBoneMatrices.size(), glm::mat4(1.0f));
    CalculateBoneTransform(m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
}

void Animator::PlayAnimation(Animation* pAnimation)
{
    if (!pAnimation) return;
    m_CurrentAnimation = pAnimation;
    m_CurrentTime      = 0.0f;
    // Keep m_Looping as-is — caller decides whether the new clip should loop
    // Reinitialize palette
    if (m_FinalBoneMatrices.empty()) m_FinalBoneMatrices.resize(100, glm::mat4(1.0f));
}

void Animator::SwitchAnimation(Animation* pAnimation, bool resetTime)
{
    if (!pAnimation) return;

    // Switch to the target animation
    m_CurrentAnimation = pAnimation;

    // Optionally preserve time (useful for seamless run phase continuity)
    if (resetTime)
        m_CurrentTime = 0.0f;

    // Keep current looping mode; callers can call SetLooping(true/false) after this
    if (m_FinalBoneMatrices.empty()) m_FinalBoneMatrices.resize(100, glm::mat4(1.0f));
}

void Animator::CalculateBoneTransform(const AssimpNodeData& node, const glm::mat4& parentTransform)
{
    if (!m_CurrentAnimation) return;

    const std::string& nodeName = node.name;
    glm::mat4 nodeTransform     = node.transformation;

    // If the animation has a bone matching this node, apply its animated transform at current time
    Bone* bone = m_CurrentAnimation->FindBone(nodeName);
    if (bone)
    {
        bone->Update(m_CurrentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    // If this node is a bone we know about, write the final matrix into the palette
    const auto& boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    auto it = boneInfoMap.find(nodeName);
    if (it != boneInfoMap.end())
    {
        int   index     = it->second.id;
        auto& offset    = it->second.offset;
        if (index >= 0 && index < static_cast<int>(m_FinalBoneMatrices.size()))
        {
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }
        // If index is beyond current palette, you can resize:
        // else if (index >= static_cast<int>(m_FinalBoneMatrices.size())) { ... }
    }

    // Process children
    for (int i = 0; i < node.childrenCount; i++)
    {
        CalculateBoneTransform(node.children[i], globalTransformation);
    }
}
