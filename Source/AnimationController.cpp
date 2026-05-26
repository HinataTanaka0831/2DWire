#include "AnimationController.h"

AnimationController::AnimationController() 
    : mCurrentState(CharacterState::Idle)
    , mIsInitialized(false) 
{
}

AnimationController::~AnimationController() {
    for (auto& pair : mAnimations) {
        delete pair.second;
    }
    mAnimations.clear();
}

void AnimationController::RegisterAnimation(CharacterState state, Object2D* anim) {
    mAnimations[state] = anim;
}

void AnimationController::ChangeState(CharacterState newState) {
    if (mIsInitialized && mCurrentState == newState) return;

	mAnimations[mCurrentState]->SetDeleteFlag(true);

    mCurrentState = newState;
    mIsInitialized = true;

    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Reset(); 
    }
}

void AnimationController::Update() {
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Update();
    }
}

void AnimationController::Draw(float x, float y) {
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->SetPosition(VGet(x, y, 0.0f)); 
        mAnimations[mCurrentState]->Draw();
    }
}
