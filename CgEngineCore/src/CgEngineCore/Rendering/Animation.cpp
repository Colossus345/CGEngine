#include "Animation.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace CGEngine {

	double Animation::getLength() const
	{
		return mLength;
	}
	unsigned int Animation::getNumPos() const
	{
		return mNumPosition;
	}
	unsigned int Animation::getNumRot() const
	{
		return mNumRotation;
	}
	unsigned int Animation::getNumScale() const
	{
		return mNumScaling;
	}
	void Animation::updateTime(double deltaTime)
	{
		mTimeStamp = +deltaTime;  
		if (mTimeStamp > mLength) {
			mTimeStamp -= mLength;
;		}
	}
	inline glm::vec3 GetGLMVec(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}


	Animation::Animation(const aiNodeAnim* anim,double length)
	{
		
		mLength = length;
		mTimeStamp = 0;
		
		mNumPosition =anim->mNumPositionKeys;
		mNumRotation = anim->mNumRotationKeys;
		mNumScaling = anim->mNumScalingKeys;

		mPositions.reserve(mNumPosition);
		mRotations.reserve(mNumRotation);
		mScalings.reserve(mNumScaling);

		for (int i = 0; i < mNumPosition; i++) {

			PosKey temp;
			temp.mPos = GetGLMVec(anim->mPositionKeys[i].mValue);
			temp.mTime = anim->mPositionKeys[i].mTime;
			mPositions.push_back(temp);
		}
		for (int i = 0; i < mNumRotation; i++) {
			RotKey temp;
			temp.mRot = GetGLMQuat(anim->mRotationKeys[i].mValue);
			temp.mTime = anim->mRotationKeys[i].mTime;
			mRotations.push_back(temp);
		}
		for (int i = 0; i < mNumScaling; i++) {
			ScaleKey temp;
			temp.mScale = GetGLMVec(anim->mScalingKeys[i].mValue);
			temp.mTime = anim->mScalingKeys[i].mTime;
			mScalings.push_back(temp);
		}


	}

	std::vector<PosKey> Animation::getPosKeys(double time) const
	{
		return mPositions;
	}
	std::vector<RotKey> Animation::getRotKeys(double time) const
	{
		return mRotations;
	}
	std::vector<ScaleKey> Animation::getScaleKeys(double time) const
	{
		return mScalings;
	}

	glm::mat4 Animation::InterpolatePosition(float animationTime)
	{
		
		if (1 == mNumPosition)
			return glm::translate(glm::mat4(1.0f), mPositions[0].mPos);
		if (animationTime > mLength) {
			while (animationTime > mLength) {
				animationTime -= mLength;
			}
		}
		int p0Index = GetPositionIndex(animationTime);
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(mPositions[p0Index].mTime,
			mPositions[p1Index].mTime, animationTime);
		glm::vec3 finalPosition = glm::mix(mPositions[p0Index].mPos,
			mPositions[p1Index].mPos, scaleFactor);
		glm::mat4 a = glm::translate(glm::mat4(1.0f), finalPosition);
		return a;
	}
	glm::mat4 Animation::InterpolateRotation(float animationTime)

	{
		if (1 == mNumRotation)
		{
			auto rotation = glm::normalize(mRotations[0].mRot);
			return glm::toMat4(rotation);
		}
		if (animationTime > mLength) {
			while (animationTime > mLength) {
				animationTime -= mLength;
			}
		}
		int p0Index = GetRotationIndex(animationTime);
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(mRotations[p0Index].mTime,
			mRotations[p1Index].mTime, animationTime);
		glm::quat finalRotation = glm::slerp(mRotations[p0Index].mRot,
			mRotations[p1Index].mRot, scaleFactor);
		finalRotation = glm::normalize(finalRotation);
		return glm::toMat4(finalRotation);
	}
	glm::mat4 Animation::InterpolateScaling(float animationTime)
	{
		if (1 == mNumScaling)
			return glm::scale(glm::mat4(1.0f), mScalings[0].mScale);
		if (animationTime > mLength) {
			while (animationTime > mLength) {
				animationTime -= mLength;
			}
		}
		int p0Index = GetScaleIndex(animationTime);
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(mScalings[p0Index].mTime,
			mScalings[p1Index].mTime, animationTime);
		glm::vec3 finalScale = glm::mix(mScalings[p0Index].mScale,
			mScalings[p1Index].mScale, scaleFactor);
		return glm::scale(glm::mat4(1.0f), finalScale);
	}

	float Animation::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float scaleFactor = 0.0f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}
	int Animation::GetPositionIndex(float animationTime)
	{
		for (int index = 0; index < mNumPosition - 1; ++index)
		{
			if (animationTime < mPositions[index + 1].mTime)
				return index;
		}

	}
	int Animation::GetRotationIndex(float animationTime)
	{
		for (int index = 0; index < mNumRotation - 1; ++index)
		{
			if (animationTime < mRotations[index + 1].mTime)
				return index;
		}

	}
	int Animation::GetScaleIndex(float animationTime)
	{
		for (int index = 0; index < mNumScaling - 1; ++index)
		{
			if (animationTime < mScalings[index + 1].mTime)
				return index;
		}

	}
}