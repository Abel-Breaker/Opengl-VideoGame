#include "../Headers/Animator.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

Animator::Animator(Animation* Animation)
{
    CurrentTime = 0.0;
    CurrentAnimation = CurrentAnimation;

    CurrentAnimation = Animation;

    FinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float dt)
{
    DeltaTime = dt;
    if (CurrentAnimation)
    {
        CurrentTime += CurrentAnimation->GetTicksPerSecond() * dt;

        if (CurrentTime > CurrentAnimation->GetDuration()) {
            PlaySound(TEXT("C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Sounds/ZombieScream.wav"), NULL, SND_ASYNC | SND_FILENAME);
        }

        CurrentTime = fmod(CurrentTime, CurrentAnimation->GetDuration());
        CalculateBoneTransform(&CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(Animation* pAnimation)
{
    CurrentAnimation = pAnimation;
    CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    // Obtener nombre del nodo y su transformaci�n
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    // Obtener informaci�n del hueso si existe
    Bone* bone = CurrentAnimation->FindBone(nodeName);

    if (bone)
    {
        bone->Update(CurrentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    // Calcular la transformaci�n global
    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    // Obtener informaci�n de huesos (suponiendo que no cambia durante la ejecuci�n)
    static const auto& boneInfoMap = CurrentAnimation->GetBoneIDMap();

    auto it = boneInfoMap.find(nodeName);
    if (it != boneInfoMap.end())
    {
        int index = it->second.id;
        glm::mat4 offset = it->second.offset;
        FinalBoneMatrices[index] = globalTransformation * offset;
    }

    // Recursi�n para los hijos del nodo
    for (int i = 0; i < node->childrenCount; i++)
    {
        CalculateBoneTransform(&node->children[i], globalTransformation);
    }
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return FinalBoneMatrices;
}