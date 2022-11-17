#include "ResourseManager.hpp"
#include "CgEngineCore/Rendering/Model.hpp"
#include "CgEngineCore/Rendering/Texture2D.hpp"
#include "CgEngineCore/Rendering/ShaderProgram.hpp"

#include<fstream>


namespace CGEngine {

    ResourseManager::TextureMap ResourseManager::m_Textures;
    ResourseManager::ModelMap   ResourseManager::m_Models;
    ResourseManager::ShaderMap  ResourseManager::m_Shaders;
    std::string                 ResourseManager::m_path;
    std::string directory;
	std::shared_ptr<Model> ResourseManager::loadOBJ(std::string const& path)
	{
        
        std::string filename = m_path + '/' + path;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // если НЕ 0
        {
            LOG_ERROR("ERROR::ASSIMP::{0}",std::string(importer.GetErrorString()));
            return nullptr;
        }
        
        directory = filename.substr(0, filename.find_last_of('/'));

        
        std::shared_ptr<Model>& p_model = m_Models.emplace(filename, std::make_unique<Model>(processNode(scene->mRootNode, scene))).first->second;
        
        return p_model;
	}

	std::shared_ptr<Texture2D> ResourseManager::loadTexture(const std::string& filename)
	{
        std::string name = m_path+'/' + filename;
        
        if (m_Textures.count(name)) {
            return getTexture(name);
        }
        int width, height, nrComponents;
        unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrComponents, 0);
        if (!data)
        {
            LOG_WARN("Texture failed to load at path: \"{0}\" ", name);
            
          
            
            if (m_Textures.count(m_path + '/' + "res/miss.png")) {
                return getTexture(m_path + '/' + "res/miss.png");
            }
            else {
                return loadTexture("res/miss.png");
            }
        
        }
        std::shared_ptr<Texture2D> p_tex = m_Textures.emplace(name,
        std::make_shared<Texture2D>(data, width, height, nrComponents)).first->second;

        stbi_image_free(data);


        return p_tex;
	}
    std::shared_ptr<Texture2D> ResourseManager::modelLoadTexture(const std::string& filename)
    {
        std::string name =filename;

        if (m_Textures.count(name)) {
            return getTexture(name);
        }
        int width, height, nrComponents;
        unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrComponents, 0);
        if (!data)
        {
            LOG_WARN("Texture failed to load at path: \"{0}\" ", name);

            if (filename == "res/miss.png")
                return nullptr;
            return loadTexture("res/miss.png");

        }
        std::shared_ptr<Texture2D>& p_tex = m_Textures.emplace(name,
            std::make_shared<Texture2D>(data, width, height, nrComponents)).first->second;

        stbi_image_free(data);


        return p_tex;
    }

    std::shared_ptr<Texture2D> ResourseManager::getTexture(const std::string& name)
    {
        TextureMap::const_iterator it = m_Textures.find(name);
        if (it != m_Textures.end())
        {
            return it->second;
        }
        LOG_WARN("Cant find texture with path: {0}", name);
        return nullptr;
    }

    std::shared_ptr<ShaderProgram> ResourseManager::loadShader(std::string const& shaderName, std::string const& vertex_path, std::string const& fragment_path)
    {
        std::string vertexString = getFileString(vertex_path);
        if (vertexString.empty()) {
            LOG_ERROR("No vertex shader at path {0}", vertex_path);
            return nullptr;
        }

        std::string fragmentString = getFileString(fragment_path);
        if (fragmentString.empty()) {
            LOG_ERROR("No fragment shader at path {0}", fragment_path);
            return nullptr;
        }

        std::shared_ptr<ShaderProgram>& newShader = m_Shaders.emplace(shaderName, std::make_shared<ShaderProgram>(vertexString.c_str(), fragmentString.c_str())).first->second;
        if (newShader->isCompiled()) {

            return newShader;

        }
        LOG_ERROR("Cant load shader program\nVertex:{0}\nFragment:{1}", vertex_path, fragment_path);
        return nullptr;
    }

    std::shared_ptr<ShaderProgram> ResourseManager::getShader(std::string const& name)
    {
        return std::shared_ptr<ShaderProgram>();
    }

    std::string ResourseManager::getFileString(const std::string& relativePath)
    {
        std::fstream f;
        f.open(m_path + '/' + relativePath.c_str(), std::ios::in | std::ios::binary);

        if (!f.is_open()) {
            LOG_ERROR("Cannot open file {0}", m_path + '/' + relativePath.c_str());
            return std::string();
        }

        std::stringstream buffer;

        buffer << f.rdbuf();
        
        return buffer.str();
    }

    ModelNode ResourseManager::processNode(aiNode* node, const aiScene* scene)
    {
        std::vector<Mesh> meshes;
        std::vector<ModelNode> nodes;

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
           
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            nodes.push_back(processNode(node->mChildren[i], scene));
        }
        return ModelNode(meshes,nodes);
    }

    Mesh ResourseManager::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;

        
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            
            if (mesh->mTextureCoords[0]) 
            {
                glm::vec2 vec;

                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        auto texmas = initMaterialTextures(scene);
        textures.insert(textures.end(), texmas.begin(), texmas.end());

        if (textures.size() == 0) {
            std::string filename = "res/miss.png";
            
            textures.push_back(loadTexture(filename.c_str()));
            

        }
        

        return Mesh(vertices, indices, textures);

    }

    std::vector<std::shared_ptr<Texture2D>> ResourseManager::initMaterialTextures(const aiScene* scene)
    {
        std::vector<std::shared_ptr<Texture2D>> textures;
        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            const aiMaterial* pMaterial = scene->mMaterials[i];
            
           
            if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0){
            aiString Path;
            
            std::string filename = directory;

            aiReturn res = pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path);
            if (res == AI_SUCCESS) {
                std::string p(Path.data);
                if (p.substr(0, 2) == ".\\") {
                    p = p.substr(2, p.size() - 2);
                }
                filename += '/';
                filename += p;
            
            }
            else {
                continue;
            }
            
            if (m_Textures.count(filename)) {
                textures.push_back(getTexture(filename));
                continue;
            }
            textures.push_back(modelLoadTexture(filename));
            }
            
            
            
        }
        
        return textures;
    }

    void ResourseManager::getExeDir(std::string& path)
    {
        size_t found = path.find_last_of("/\\");
        m_path = path.substr(0, found);


    }

    


}