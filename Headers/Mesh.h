#pragma once
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <string>
#include <vector>
#define MAX_BONE_INFLUENCE 4

class Animator;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    //bone indexes which will influence this vertex
    int boneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float boneWeights[MAX_BONE_INFLUENCE];

};
struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Animator* animator);
private:
    //  render data
    unsigned int VAO, VBO, EBO;
    unsigned int shader;
    unsigned int texture;

    void setupMesh();
    void loadTexture();
};

