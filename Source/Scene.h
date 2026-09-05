#pragma once
#include "DxLib.h"

class ObjectManager;

// ゲーム画面（シーン）のライフサイクルおよび固有ObjectManagerを保持する基底クラス
class Scene
{
public:
	// シーン固有のObjectManagerの生成
	// 入力: なし / 出力: なし / 副作用: mpObjectManagerのインスタンス化
	Scene();
	virtual ~Scene();

	// シーン開始時のリソース読み込みやオブジェクト初期配置
	// 入力: なし / 出力: なし / 副作用: 各具象シーンによるリソース確保
	virtual void Initialize() = 0;

	// シーン内の全オブジェクトの更新
	// 入力: なし / 出力: なし / 副作用: mpObjectManager->Update()呼び出し
	virtual void Update();

	// シーン内の全オブジェクトの描画
	// 入力: なし / 出力: なし / 副作用: mpObjectManager->Draw()呼び出し
	virtual void Draw();

	// シーン終了時のリソース破棄およびオブジェクト解放
	// 入力: なし / 出力: なし / 副作用: リソースの解放
	virtual void Finalize() = 0;

	ObjectManager* GetObjectManager() { return mpObjectManager; }

protected:
	int fontSize20 = CreateFontToHandle(NULL, 20, -1, -1);
	int fontSize50 = CreateFontToHandle(NULL, 50, -1, -1);
	int fontSize90 = CreateFontToHandle(NULL, 90, -1, -1);

private:
	ObjectManager* mpObjectManager; // シーン内のオブジェクトを管理するインスタンス
};