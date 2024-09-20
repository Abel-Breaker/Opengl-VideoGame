#pragma once
#include "Animation.h"
class Animator
{
public:
    Animator(Animation* Animation);

    void UpdateAnimation(float dt);

    void PlayAnimation(Animation* pAnimation);

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

    std::vector<glm::mat4> GetFinalBoneMatrices();

private:
    std::vector<glm::mat4> FinalBoneMatrices;
    Animation* CurrentAnimation;
    float CurrentTime;
    float DeltaTime;
};

