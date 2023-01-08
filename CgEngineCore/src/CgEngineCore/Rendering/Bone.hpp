#pragma once
#include "glm/glm.hpp"
#include <string>
#include <map>
#include <list>
#include "Animation.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CGEngine {

	//struct aiAnimation;

	class Bone {
	public:
		int id;
		std::string name;
		std::list<Bone> children;

		glm::mat4 animatedTransform;

		glm::mat4 localBindTransform;
		glm::mat4 inverseBindTransform;

		glm::mat4 offset;

		std::map<std::string,Animation*> animation;

		void setupAnim( aiAnimation** animation, const unsigned int numAnim);
		
		Bone(int id, std::string name, glm::mat4 bindLocalTransform, glm::mat4 offset,  aiAnimation** animation, const unsigned int numAnim);
		~Bone();


		void addChild(Bone child);
		void applyRoot(glm::mat4 offset);

		glm::mat4 getAnimatedTransform() const;

		void updateAnimation(double currentTime,std::string anima, glm::mat4 parentTransform, std::vector<glm::mat4>& mFinalBoneMatrix);
	
		void setAnimatedTransform(glm::mat4 newAnimatedTransform);

		void calcInverseBindTransform(glm::mat4 parentBindTransform);


	};

}