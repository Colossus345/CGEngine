#pragma once
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CGEngine {

	struct PosKey {
		double mTime;
		glm::vec3 mPos;
	};
	struct RotKey {
		double mTime;
		glm::quat mRot;
	};
	struct ScaleKey {
		double mTime;
		glm::vec3 mScale;
	};



	class Animation {

	public:
		double getLength() const;
		
		unsigned int getNumPos() const;
		unsigned int getNumRot() const;
		unsigned int getNumScale() const;

		void updateTime(double deltaTime);

		Animation(const aiNodeAnim* anim, double length);
		
		
		std::vector<PosKey>		getPosKeys(double time) const;
		std::vector<RotKey>		getRotKeys(double time) const;
		std::vector<ScaleKey>	getScaleKeys(double time) const;

		glm::mat4 InterpolatePosition(float animationTime);

		glm::mat4 InterpolateRotation(float animationTime);

		glm::mat4 InterpolateScaling(float animationTime);


	private:

		double mLength;

		double mTimeStamp;

		unsigned int mNumPosition;
		unsigned int mNumRotation;
		unsigned int mNumScaling;
		
		std::vector<PosKey>		mPositions;
		std::vector<RotKey>		mRotations;
		std::vector<ScaleKey>	mScalings;



		
		
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		
		
		int GetPositionIndex(float animationTime);
			
		int GetRotationIndex(float animationTime);
					
		int GetScaleIndex(float animationTime);
		

	};

}