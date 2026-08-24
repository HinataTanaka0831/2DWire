#pragma once

#include <list>
#include <vector>
#include "Object2D.h"

// シーン内の全2Dオブジェクトのライフサイクル・更新・描画・タグ検索を一括管理するクラス
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	// 管理下の全2Dオブジェクトの一括更新
	// 入力: なし / 出力: なし / 副作用: 各オブジェクトのUpdate呼び出し
	void Update();

	// 管理下の全2Dオブジェクトの一括描画
	// 入力: なし / 出力: なし / 副作用: 各オブジェクトのDraw呼び出し
	void Draw();

public:
	// オブジェクトの登録
	// 入力: object2D(追加するオブジェクト) / 出力: なし / 副作用: 管理リストへのポインタ追加
	void AddObject(Object2D* object2D);

	// シーン切り替え時等における全オブジェクトの即時破棄
	// 入力: なし / 出力: なし / 副作用: 全オブジェクトのdeleteおよびリストクリア
	void DeleteAll2D();

	// 削除フラグが立った不要オブジェクトをフレーム終了時に安全に一括削除
	// 入力: なし / 出力: なし / 副作用: 該当オブジェクトのdeleteおよびリストからの除外
	void DeleteAll2DIfNeeded();

	// 指定タグを持つ先頭のオブジェクトを検索
	// 入力: tag(検索対象タグ) / 出力: 一致するObject2Dポインタ(存在しない場合はnullptr) / 副作用: なし
	Object2D* GetObject2DByTag(Object2D::Tag tag);

	// 指定タグを持つ全オブジェクトのリストを取得
	// 入力: tag(検索対象タグ) / 出力: 一致する全Object2Dポインタの配列 / 副作用: なし
	std::vector<Object2D*> GetObject2DListByTag(Object2D::Tag tag);

private:
	std::list<Object2D*> mObject2DList;         // オブジェクト管理用双方向リスト
};