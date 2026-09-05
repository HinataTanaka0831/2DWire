#pragma once
#include <map>
#include <memory>
#include "TextureAnimation.h"

// アニメーションを切り替えるためのキャラクター状態定義
enum class CharacterState {
    Idle,
    Moving,
    Attacking,
    Damaged,
    Dead
};

// 状態に応じたTextureAnimationの切り替え・同期制御を行うコントローラー
class AnimationController {
public:
    AnimationController();
    ~AnimationController();

    // 状態に対応するアニメーションを登録
    // 入力: state(状態), anim(アニメーションポインタ) / 出力: なし / 副作用: 内部マップへの所有権移動
    void RegisterAnimation(CharacterState state, std::unique_ptr<TextureAnimation> anim);

    // キャラクター状態を変更し、アニメーションを切り替え
    // 入力: newState(新規状態) / 出力: なし / 副作用: 現在状態の更新
    void ChangeState(CharacterState newState);

    // 登録された全アニメーションに対して向き（反転フラグ）を一括適用
    // 入力: rev(反転フラグ) / 出力: なし / 副作用: 全TextureAnimationの反転設定更新
    void SetEnemyReverse(bool rev);

    // 現在アクティブなアニメーションの座標同期とフレーム更新
    // 入力: x, y(中心座標) / 出力: なし / 副作用: アニメーション状態の進行
    void Update(float x, float y);

    // 現在アクティブなアニメーションの描画
    // 入力: cameraX, cameraY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
    void Draw(float cameraX, float cameraY);

    CharacterState GetCurrentState() const { return m_currentState; }

private:
    std::map<CharacterState, std::unique_ptr<TextureAnimation>> m_animations;
    CharacterState m_currentState;
    bool m_isInitialized;
};