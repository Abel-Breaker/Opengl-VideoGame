#include "../Headers/Mesh.h"
#include <glad.h>
#include "../Headers/ShaderManager.h"
#include "../Headers/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include "../Headers/Animator.h"
#include "../Headers/Frame.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    //this->textures = textures;

    shader = ShaderManager::setAndGetShader("GameCore/Shaders/animationShader.vert", "GameCore/Shaders/animationShader.frag");

    setupMesh();
    loadTexture();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // ids
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
    // weights
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, boneWeights));

    glBindVertexArray(0);
}

//CARGA DE TEXTURAS
void Mesh::loadTexture() {
    // load and create a texture 
    // -------------------------
    //stbi_set_flip_vertically_on_load(true);  // Invertir la imagen al cargar

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Textures/zombie.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    }
    else
    {
        printf("Error al cargar la textura %s\n", "C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Textures/wolf.png");
        //std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Mesh::Draw(Animator* animator)
{
    glUseProgram(this->shader);

    Camera::updateCamera(shader);

    animator->UpdateAnimation(Frame::getLapsoTime());
    
    int boneLoc;
    std::vector<glm::mat4> transforms = animator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i) {
        std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
        boneLoc = glGetUniformLocation(shader, name.c_str());
        if (boneLoc != -1) {
            glUniformMatrix4fv(boneLoc, 1, GL_FALSE, glm::value_ptr(transforms[i]));
            //printf("%f\n", transforms[i]);
        }
        else {
            printf("No se cargó en el shader\n");
        }
        
    }
    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    // translate it down so it's at the center of the scene
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
    // it's a bit too big for our scene, so scale it down
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    /* APLICAMOS TEXTURA */
    glUniform1i(glGetUniformLocation(this->shader, "texture1"), 0);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}