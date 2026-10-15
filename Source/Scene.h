#pragma once
#include "DxLib.h"

class ObjectManager;

// ゲーム画面（シーン）のライフサイクルおよび固有ObjectManagerを保持する基底クラス
class Scene
{
public:
	// シーン固有のObjectManagerの生成
	// 入力: なし / 出力: なし / 副作用: m_ObjectManagerのインスタンス化
	Scene();
	virtual ~Scene();

	// シーン開始時のリソース読み込みやオブジェクト初期配置
	// 入力: なし / 出力: なし / 副作用: 各具象シーンによるリソース確保
	virtual void Initialize() = 0;

	// シーン内の全オブジェクトの更新
	// 入力: なし / 出力: なし / 副作用: m_ObjectManager->Update()呼び出し
	virtual void Update();

	// シーン内の全オブジェクトの描画
	// 入力: なし / 出力: なし / 副作用: m_ObjectManager->Draw()呼び出し
	virtual void Draw();

	// シーン終了時のリソース破棄およびオブジェクト解放
	// 入力: なし / 出力: なし / 副作用: リソースの解放
	virtual void Finalize() = 0;

	ObjectManager* GetObjectManager() { return m_objectManager; }

protected:
	int m_fontSize20;  // フォントサイズ20のフォントハンドル
	int m_fontSize50;  // フォントサイズ50のフォントハンドル
	int m_fontSize90;  // フォントサイズ90のフォントハンドル

private:
	ObjectManager* m_objectManager; // シーン内のオブジェクトを管理するインスタンス
};