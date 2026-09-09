#include "ObjectManager.h"
#include <algorithm>

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

// 管理下の全2Dオブジェクトの一括更新
// 入力: なし / 出力: なし / 副作用: 各オブジェクトのUpdate呼び出し
void ObjectManager::Update()
{
	for (std::list<Object2D*>::iterator itr = m_object2DList.begin(); itr != m_object2DList.end(); itr++)
	{
		(*itr)->Update();
	}
}

// 管理下の全2Dオブジェクトの一括描画
// 入力: なし / 出力: なし / 副作用: 各オブジェクトのDraw呼び出し
void ObjectManager::Draw()
{
	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); itr++)
	{
		(*itr)->Draw();
	}
}

// オブジェクトの登録
// 入力: object2D(追加するオブジェクト) / 出力: なし / 副作用: 管理リストへのポインタ追加
void ObjectManager::AddObject(Object2D* object2D)
{
	m_object2DList.push_back(object2D);
}

// シーン切り替え時等における全オブジェクトの即時破棄
// 入力: なし / 出力: なし / 副作用: 全オブジェクトのdeleteおよびリストクリア
void ObjectManager::DeleteAll2D()
{
	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); )
	{
		Object2D* temp = *itr;
		// イテレータ無効化を防ぐためeraseの戻り値でイテレータを更新
		itr = m_object2DList.erase(itr);
		delete temp;
		temp = nullptr;
	}
}

// 削除フラグが立った不要オブジェクトをフレーム終了時に安全に一括削除
// 入力: なし / 出力: なし / 副作用: 該当オブジェクトのdeleteおよびリストからの除外
void ObjectManager::DeleteAll2DIfNeeded()
{
	// Updateループ中の直接削除によるクラッシュ（不正メモリアクセス）を防ぐための遅延クリーンアップ
	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); )
	{
		if ((*itr)->IsDeleteFlag())
		{
			Object2D* temp = *itr;
			itr = m_object2DList.erase(itr);
			delete temp;
			temp = nullptr;
		}
		else
		{
			itr++;
		}
	}
}

// 指定タグを持つ先頭のオブジェクトを検索
// 入力: tag(検索対象タグ) / 出力: 一致するObject2Dポインタ(存在しない場合はnullptr) / 副作用: なし
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag tag)
{
	auto itr = std::find_if(
		m_object2DList.begin(),
		m_object2DList.end(),
		[&](Object2D* obj) { return obj->GetTag() == tag; }
	);

	if (itr != m_object2DList.end())
	{
		return (*itr);
	}

	return nullptr;
}

// 指定タグを持つ全オブジェクトのリストを取得
// 入力: tag(検索対象タグ) / 出力: 一致する全Object2Dポインタの配列 / 副作用: なし
std::vector<Object2D*> ObjectManager::GetObject2DListByTag(Object2D::Tag tag)
{
	std::vector<Object2D*> ret;
	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); itr++)
	{
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back((*itr));
		}
	}
	return ret;
}