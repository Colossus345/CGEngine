#include "Mesh.hpp"

#include"Render_OpenGl.hpp"
#include"ShaderProgram.hpp"
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
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures, Bone* skeleton)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        this->m_Skeleton = std::move(skeleton);
        
        m_Skeleton->calcInverseBindTransform(glm::mat4(1.f));

        anima = std::make_unique<Animator>(skeleton);
        
        setupMesh();
    }
    Mesh& Mesh::operator=(Mesh&& Mesh) noexcept
    {
        VAO = std::move(Mesh.VAO);
        VBO = std::move(Mesh.VBO);
        EBO = std::move(Mesh.EBO);
        anima = std::move(Mesh.anima);
        vertices = Mesh.vertices;
        indices  = Mesh.indices;
        textures = Mesh.textures;
        Mesh.vertices.clear();
        Mesh.indices.clear();
        Mesh.textures.clear();
        Mesh.anima.release();

        return *this;
    }
    Mesh::Mesh(Mesh&& Mesh) noexcept
    {
        VAO = std::move(Mesh.VAO);
        VBO = std::move(Mesh.VBO);
        EBO = std::move(Mesh.EBO);
        anima = std::move(Mesh.anima);
        vertices = Mesh.vertices;
        indices  = Mesh.indices;
        textures = Mesh.textures;
        m_Skeleton = std::move(Mesh.m_Skeleton);
        Mesh.vertices.clear();
        Mesh.indices.clear();
        Mesh.textures.clear();
        Mesh.anima.release();
        

    }
    void Mesh::Draw(unsigned int id, ShaderProgram& shader, double& deltaTime)
    {   
        //Animate(shader, "mixamorig_Hips", deltaTime*1000);

        

        for (int i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
        }
         
        Renderer_OpenGL::draw(*VAO);
    }

    void Mesh::Test(unsigned char* data, int m_width, int m_height,int format)
    {
       
        Renderer_OpenGL::draw(*VAO);
    }

    void Mesh::Animate(ShaderProgram& shader,std::string animation,double deltaTime)
    {
        anima->play("mixamorig_Hips");
        anima->update(deltaTime);
        auto transforms = anima->getTransforms();
        for (int i = 0; i < transforms.size(); i++)
        {
            std::string a = "finalBonesMatrices[" + std::to_string(i) + "]";
            shader.setMatrix4(a.c_str(), transforms[i]);

        }
        


    }

    void Mesh::add_skeleton(Bone* skeleton)
    {
        m_Skeleton = skeleton;
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