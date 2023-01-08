#include "Animator.hpp"

namespace CGEngine {
	Animator::Animator()
	{
		mRoot = nullptr;
		mAnimName = "";
		mCurrentTime = 0;
	}
	Animator::Animator(Bone* skeleton)
	{
		mRoot = skeleton;
		mAnimName = "";
		mCurrentTime = 0;

		mFinalBoneMatrix.reserve(MAX_BONE_INFLUENCE);
		for (int i = 0; i < 100; i++)
			mFinalBoneMatrix.push_back(glm::mat4(1.0f));

	}
	void Animator::setAnimation(std::string animation)
	{
		mAnimName = animation;
		mCurrentTime = 0;
	}
	void Animator::update(double deltaTime)
	{
		if (!isPlaying) {
			return;
		}
		if (mAnimName == "") {
			return;
		}
		mRoot->updateAnimation(mCurrentTime,mAnimName,glm::mat4(1.f),mFinalBoneMatrix);

		mCurrentTime += deltaTime;

	}
	void Animator::setSkeleton(Bone* skeleton)
	{
		mRoot = skeleton;
		mFinalBoneMatrix.reserve(MAX_BONE_INFLUENCE);
		for (int i = 0; i < 100; i++)
			mFinalBoneMatrix.push_back(glm::mat4(1.0f));
	}
	void Animator::play()
	{
		isPlaying = true;
	}
	void Animator::play(std::string animation)
	{
		isPlaying = true;
		mAnimName = animation;
	}
	void Animator::stop()
	{
		isPlaying = false;
	}
}
