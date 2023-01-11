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
        //kringe();
        
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
        m_Skeleton = std::move(Mesh.m_Skeleton);
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
        Animate(shader, "mixamorig_Hips", deltaTime);

        

        for (int i = 0; i < textures.size(); i++) {
            textures[i]->bind(i);
        }
         
       
        Renderer_OpenGL::draw(*VAO);
    }

    void Mesh::Test(unsigned char* data, int m_width, int m_height,int format)
    {
       
       // Renderer_OpenGL::draw(*VAO);
    }

    void Mesh::Animate(ShaderProgram& shader,std::string animation,double deltaTime)
    {
        anima->play("mixamorig_Hips");
        anima->update(deltaTime*100);
        
        auto transforms = anima->getTransforms();
        //auto tr = ar->GetFinalBoneMatrices();
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

    void Mesh::kringe()
    {
        for (int i = 0; i < vertices.size(); i++) {
            vertices[i].BoneID[0] = 101;
            vertices[i].BoneID[1] = 101;
            vertices[i].BoneID[2] = 101;
            vertices[i].BoneID[3] = 101;
        }
    }

  
    
    void Mesh::setupMesh()
	{
        VAO = std::make_unique<VertexArray>();
        VBO = std::make_unique<VertexBuffer>();
        EBO = std::make_unique<IndexBuffer>();
        VAO->bind();
        VBO->init(vertices.data(), vertices.size() * sizeof(Vertex), vertlay);
        EBO->init(indices.data(), indices.size());

        
        VAO->add_vertex_buffer(*VBO);
        VAO->set_index_buffer(*EBO);
        
        //VAO->unbind();
        //create buffers / arrays
        /*glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
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
        glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneID));

        // weights
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
        glBindVertexArray(0);
       */
	}
}