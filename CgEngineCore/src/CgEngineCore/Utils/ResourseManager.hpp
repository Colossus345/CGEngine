#pragma once

#include<string>
#include<memory>
#include<map>
#include<vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


//struct aiNode;
//struct aiScene;
//struct aiMesh;
//struct aiMaterial;



namespace CGEngine {
	
	class Model;
	class Texture2D;
	class Mesh;
	struct ModelNode;
	struct Texture;


	class ResourseManager {

	public:
		ResourseManager() = delete;
		~ResourseManager() = delete;
		ResourseManager(const ResourseManager&) = delete;
		ResourseManager(ResourseManager&&) = delete;
		ResourseManager& operator = (const ResourseManager&) = delete;
		ResourseManager& operator=(ResourseManager&&) = delete;


		static std::shared_ptr<Model> loadOBJ(std::string const& path);
		static std::shared_ptr<Texture2D>  loadTexture(const std::string& filename);
		static std::shared_ptr<Texture2D>  getTexture(const std::string& name);

	//private:

		static ModelNode processNode(aiNode* node, const aiScene* scene);
		static Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		static std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		


		typedef std::map<const std::string, std::shared_ptr<Model>> ModelMap;
		static ModelMap m_Models;
		typedef std::map<const std::string, std::shared_ptr<Texture2D>> TextureMap;
		static TextureMap m_Textures;

		static std::string m_path;
		//static std::string directory;
	};

}