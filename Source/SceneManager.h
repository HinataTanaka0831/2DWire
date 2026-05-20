#pragma once

// クラスの前方宣言
class Scene;

class SceneManager
{
public:  // enum, struct の定義

	// シーンの種類
	// note: シーンを増やす必要があれば、ここも追加していくこと
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,  // 定義なし
		SCENE_TITLE,     // タイトル
		SCENE_GAME,      // ゲーム
		SCENE_RESULT,   // リザルト
		SCENE_SELECT,  // セレクト
		SCENE_GAME_RULE,
		SCENE_GAMEOVER,
		// SCENE_OPTION,  // オプション
	};


public:  // メンバ関数の定義
	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();


	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了処理
	void Finalize();


	// シーン遷移（切り替え処理）が必要な状態なら遷移処理をする
	void ChangeSceneIfNeeded();

	// 次に遷移するシーンの設定
	// note: シーン遷移をしたい場合は、必ずこの処理を経由して遷移させる
	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }

	// 現在シーンの取得
	Scene* GetCurrentScene() { return mpCurrentScene; }

private:
	SCENE_TYPE mnSceneType;            // 現在シーンのタイプ
	SCENE_TYPE mnNextSceneType;        // 次シーンのタイプ
	Scene* mpCurrentScene;             // 現在シーンのポインタ
};
