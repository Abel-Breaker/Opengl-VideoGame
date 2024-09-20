#pragma once
#include <glm/detail/type_mat.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include "Bone.h"
#include <assimp/postprocess.h>
#include <map>
#include <assimp/scene.h>
#include "Model.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    Animation() = default;

    Animation(const std::string& animationPath, Model* model);

    ~Animation()
    {
    }

    Bone* FindBone(const std::string& name);


    float GetTicksPerSecond();

    float GetDuration();

    const AssimpNodeData& GetRootNode();

    const std::map<std::string, BoneInfo>& GetBoneIDMap();

private:
    void ReadMissingBones(const aiAnimation* animation, Model& model);

    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};

