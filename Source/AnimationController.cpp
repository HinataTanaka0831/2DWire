#include "AnimationController.h"

AnimationController::AnimationController()
    : mCurrentState(CharacterState::Idle)
    , mIsInitialized(false)
{
}

AnimationController::~AnimationController()
{
}

// 状態に対応するアニメーションを登録
// 入力: state(状態), anim(アニメーションポインタ) / 出力: なし / 副作用: 内部マップへの所有権移動
void AnimationController::RegisterAnimation(CharacterState state, std::unique_ptr<TextureAnimation> anim)
{
    mAnimations[state] = std::move(anim);
}

// 現在アクティブなアニメーションの座標同期とフレーム更新
// 入力: x, y(中心座標) / 出力: なし / 副作用: アニメーション状態の進行
void AnimationController::Update(float x, float y)
{
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Update();
        mAnimations[mCurrentState]->SetPosition(VGet(x, y, 0.0f));
    }
}

// 現在アクティブなアニメーションの描画
// 入力: cameraX, cameraY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
void AnimationController::Draw(float cameraX, float cameraY)
{
    if (mAnimations.count(mCurrentState)) {
        mAnimations[mCurrentState]->Draw(cameraX, cameraY);
    }
}

// キャラクター状態を変更し、アニメーションを切り替え
// 入力: newState(新規状態) / 出力: なし / 副作用: 現在状態の更新
void AnimationController::ChangeState(CharacterState newState)
{
    // 同一状態遷移時のアニメーション再初期化によるコマのチラつきを防止
    if (mIsInitialized && mCurrentState == newState) return;

    mCurrentState = newState;
    mIsInitialized = true;
}

// 登録された全アニメーションに対して向き（反転フラグ）を一括適用
// 入力: rev(反転フラグ) / 出力: なし / 副作用: 全TextureAnimationの反転設定更新
void AnimationController::SetEnemyReverse(bool rev)
{
    // 状態遷移をまたいでもキャラクターの左右の向きが破綻しないよう全ステートに適用
    for (auto& pair : mAnimations)
    {
        pair.second->SetReverse(rev);
    }
}