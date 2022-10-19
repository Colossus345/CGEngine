#include "Mesh.hpp"

#include"Render_OpenGl.hpp"

#include <glad/glad.h>


namespace CGEngine {
    Mesh::Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture> textures)
        
    {
        this->vertices = vert;
        this->indices = ind;
        this->textures = textures;

        
        setupMesh();
    
    
    }
    void Mesh::Draw(unsigned int id)
    {
       
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // перед связыванием активируем нужный текстурный юнит

            // Получаем номер текстуры (номер N в diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // конвертируем unsigned int в строку
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // конвертируем unsigned int в строку
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // конвертируем unsigned int в строку

            // Теперь устанавливаем сэмплер на нужный текстурный юнит
            glUniform1i(glGetUniformLocation(id, (name + number).c_str()), i);
            
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        

        Renderer_OpenGL::draw(*VAO);

        glActiveTexture(GL_TEXTURE0);
    }
    void Mesh::setupMesh()
	{
        VAO = std::make_unique<VertexArray>();
        VBO = std::make_unique<VertexBuffer>();
        EBO = std::make_unique<IndexBuffer>();
        VAO->bind();
        VBO->init(vertices.data(), vertices.size() * sizeof(Vertex), vertlay);
        EBO->init(indices.data(),indices.size());

        VAO->add_vertex_buffer(*VBO);
        VAO->set_index_buffer(*EBO);
        VAO->unbind();
       
	}
}