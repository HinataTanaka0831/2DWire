#include "AnimationController.h"

AnimationController::AnimationController()
    : mCurrentState(CharacterState::Idle)
    , mPrevState(CharacterState::Idle)
    , mIsInitialized(false)
{
}

AnimationController::~AnimationController()
{
    for (auto& pair : mAnimations) {
        delete pair.second;
    }
    mAnimations.clear();
}

void AnimationController::RegisterAnimation(CharacterState state, TextureAnimation* anim)
{
    mAnimations[state] = anim;
}


void AnimationController::Update()
{
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Update();
    }
}


void AnimationController::Draw(float x, float y, float cameraX, float cameraY)
{
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->SetPosition(VGet(x, y, 0.0f));
        mAnimations[mCurrentState]->Draw(cameraX, cameraY);
    }
}


void AnimationController::ChangeState(CharacterState newState)
{
    // 同一状態でのアニメーション再初期化によるちらつきを防止
    if (mIsInitialized && mCurrentState == newState) return;

    mPrevState = mCurrentState;
    mCurrentState = newState;
    mIsInitialized = true;
}


void AnimationController::SetEnemyReverse(bool rev)
{
    // Idle / Moving / Attacking など、登録済みの全アニメに設定
	// 状態が切り替わっても向きが引き継がれる
    for (auto& pair : mAnimations)
    {
        pair.second->SetReverse(rev);
    }
}
