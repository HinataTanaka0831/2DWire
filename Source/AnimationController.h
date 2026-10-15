#pragma once
#include <map>
#include <memory>

class TextureAnimation;

// アニメーションを切り替えるためのキャラクター状態定義
enum class CharacterState {
    Idle,
    Moving,
    Attacking,
    Dead
};

// 状態に応じたTextureAnimationの切り替え・同期制御を行うコントローラー
class AnimationController 
{
public:
    AnimationController();
    ~AnimationController();

    // 状態に対応するアニメーションを登録
    // 入力: state(状態), anim(アニメーションポインタ) / 出力: なし / 副作用: 内部マップへの所有権移動
    void RegisterAnimation(CharacterState state, TextureAnimation* anim);

    // 現在アクティブなアニメーションの座標同期とフレーム更新
    // 入力: x, y(中心座標) / 出力: なし / 副作用: アニメーション状態の進行
    void Update(float x, float y);

    // 現在アクティブなアニメーションの描画
    // 入力: cameraX, cameraY(カメラ座標) / 出力: なし / 副作用: バックバッファへの描画
    void Draw(float cameraX, float cameraY);

    // キャラクター状態を変更し、アニメーションを切り替え
    // 入力: newState(新規状態) / 出力: なし / 副作用: 現在状態の更新
    void ChangeState(CharacterState newState);

    // 登録された全アニメーションに対して向き（反転フラグ）を一括適用
    // 入力: reverse(反転フラグ) / 出力: なし / 副作用: 全TextureAnimationの反転設定更新
    void SetEnemyReverse(bool reverse);

    // アニメーションをループ再生せずに1回だけ再生する
    // 入力: なし / 出力: trueならEndAnimation()を呼び出す、falseなら終了 / 副作用: アニメーションのループ制御
    bool EndAnimation();

    CharacterState GetCurrentState() const { return m_currentState; }

private:
    std::map<CharacterState, TextureAnimation*> m_animations;
    CharacterState m_currentState;
    bool m_isInitialized;
};