#pragma once
#include <vector>
#include <string>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <map>

class Animator;

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};

class Model
{
public:
    Model(const char* path)
    {
        loadModel(path);
    }
    auto& GetBoneInfoMap() { return m_BoneInfoMap; }////////////
    int& GetBoneCount() { return m_BoneCounter; }
    void Draw(Animator* animator);
private:
    // model data
    std::vector<Mesh> meshes;
    //std::string directory;
    //Animator *animator;

    //NEW
    std::map<std::string, BoneInfo> m_BoneInfoMap; //
    int m_BoneCounter = 0;

    void SetVertexBoneDataToDefault(Vertex& vertex);
        //END NEW
    void loadModel(const char* path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
    //    std::string typeName);
};
