#include "AnimationController.h"
#include "TextureAnimation.h"

AnimationController::AnimationController()
    : m_currentState(CharacterState::Idle)
    , m_isInitialized(false)

{
}

AnimationController::~AnimationController()
{
    for (auto& pair : m_animations)
    {
        delete pair.second;
    }

    m_animations.clear();
}

void AnimationController::RegisterAnimation(CharacterState state, TextureAnimation* anim)
{
    m_animations[state] = anim;
}

void AnimationController::Update(float x, float y)
{
    if (m_animations.count(m_currentState)) {
        m_animations[m_currentState]->Update();
        m_animations[m_currentState]->SetPosition(VGet(x, y, 0.0f));
    }
}

void AnimationController::Draw(float cameraX, float cameraY)
{
    if (m_animations.count(m_currentState)) {
        m_animations[m_currentState]->Draw(cameraX, cameraY);
    }
}

void AnimationController::ChangeState(CharacterState newState)
{
    // 同一状態遷移時のアニメーション再初期化によるコマのチラつきを防止
    if (m_isInitialized && m_currentState == newState) return;

    m_currentState = newState;
    m_isInitialized = true;
}

void AnimationController::SetEnemyReverse(bool reverse)
{
    // 状態遷移をまたいでもキャラクターの左右の向きが破綻しないよう全ステートに適用
    for (auto& pair : m_animations)
    {
        pair.second->SetReverse(reverse);
    }
}

bool AnimationController::EndAnimation() {
    if (m_animations.count(m_currentState)) 
    {
        return m_animations[m_currentState]->EndAnimation();
    }

    return false;
}
