#include "ResourseManager.hpp"
#include "CgEngineCore/Rendering/Model.hpp"
#include "CgEngineCore/Rendering/Texture2D.hpp"



namespace CGEngine {

    ResourseManager::TextureMap ResourseManager::m_Textures;
    ResourseManager::ModelMap   ResourseManager::m_Models;

    std::string directory;
	std::shared_ptr<Model> ResourseManager::loadOBJ(std::string const& path)
	{
        
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // если НЕ 0
        {
            return nullptr;
        }
        
        directory = path.substr(0, path.find_last_of('/'));

        
        std::shared_ptr<Model>& p_model = m_Models.emplace(path, std::make_unique<Model>(processNode(scene->mRootNode, scene))).first->second;
        
        return p_model;
	}

	std::shared_ptr<Texture2D> ResourseManager::loadTexture(const std::string& filename)
	{
        std::string name = filename;
        
        if (m_Textures.count(filename)) {
            return getTexture(filename);
        }
        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (!data)
        {
            LOG_WARN("Texture failed to load at path: \"{0}\" ", filename);
            
            
            return loadTexture("C:/Users/Syndafloden/Documents/CGEngine/res/miss.png");
        
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

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

 
        auto heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        if (textures.size() == 0) {
            std::string filename = "C:/Users/Syndafloden/Documents/CGEngine/res/miss.png";
            
            textures.push_back(loadTexture(filename.c_str()));
            

        }
        //std::vector<Texture>

        return Mesh(vertices, indices, textures);

    }

    std::vector<std::shared_ptr<Texture2D>> ResourseManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<std::shared_ptr<Texture2D>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            std::string filename = directory + '/' + str.C_Str();
            if (m_Textures.count(filename)) {
                continue;
            }
            
            textures.push_back(loadTexture(filename.c_str()));
        }
        
        return textures;
    }

    


}