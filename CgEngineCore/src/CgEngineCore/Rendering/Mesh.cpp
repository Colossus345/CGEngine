#include "Mesh.hpp"

#include"Render_OpenGl.hpp"

#include"Texture2D.hpp"

#include <glad/glad.h>


namespace CGEngine {
    Mesh::Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<std::shared_ptr<Texture2D>> textures)
        
    {
        this->vertices = vert;
        this->indices = ind;
        this->textures = textures;

        
        setupMesh();
    
    
    }
    Mesh& Mesh::operator=(Mesh&& Mesh) noexcept
    {
        VAO = std::move(Mesh.VAO);
        VBO = std::move(Mesh.VBO);
        EBO = std::move(Mesh.EBO);
        vertices = Mesh.vertices;
        indices  = Mesh.indices;
        textures = Mesh.textures;
        Mesh.vertices.clear();
        Mesh.indices.clear();
        Mesh.textures.clear();

        return *this;
    }
    Mesh::Mesh(Mesh&& Mesh) noexcept
    {
        VAO = std::move(Mesh.VAO);
        VBO = std::move(Mesh.VBO);
        EBO = std::move(Mesh.EBO);
        vertices = Mesh.vertices;
        indices  = Mesh.indices;
        textures = Mesh.textures;
        Mesh.vertices.clear();
        Mesh.indices.clear();
        Mesh.textures.clear();

    }
    void Mesh::Draw(unsigned int id)
    {
        for (int i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
        }
         
        Renderer_OpenGL::draw(*VAO);
    }

    void Mesh::Test(unsigned char* data, int m_width, int m_height,int format)
    {
       
        Renderer_OpenGL::draw(*VAO);
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