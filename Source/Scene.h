#pragma once
#include "DxLib.h"
// クラスの前方宣言
class ObjectManager;

// シーンの基底クラス

// タイトル、ゲーム、リザルト画面など、
// 画面を作る際は必ずこのクラスを継承する

class Scene
{
public:
	// コンストラクタ
	Scene();

	// デストラクタ
	~Scene();

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// 終了処理
	virtual void Finalize() = 0;


	// オブジェクトマネージャーの取得
	ObjectManager* GetObjectManager() { return mpObjectManager; }

protected:
	int grHandle = LoadGraph("Resource/haikei.jpg");
	int icHandle = LoadGraph("Resource/Player.png");

private:
	ObjectManager* mpObjectManager;        // オブジェクト管理クラスのポインタ
};
