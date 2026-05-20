#include "ObjectManager.h"

// コンストラクタ
ObjectManager::ObjectManager()
{

}

// デストラクタ
ObjectManager::~ObjectManager()
{

}

// 更新
void ObjectManager::Update()
{
	// 2Dオブジェクトの更新
	// mObject2Dlist.begin() ... リストの先頭の要素へのイテレーターを取得する
	// mObject2Dlist.end() ... リストの末尾の要素へのイテレーターを取得する
	// イテレーターとは？ ... std::list の要素のことを指す。
	// リストの利点 ... 要素と要素の間に新しい要素を差し込むことが容易に出来る
	// std::list ... 双方向連結リスト（要素の前後の要素にアクセスすることが簡単）
	for (std::list<Object2D*>::iterator itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Update();
	}
}

// 描画
void ObjectManager::Draw()
{
	// auto ... 型推論。 = より右側の型を推測してくれる便利なやつ
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Draw();
	}

}

// 2Dオブジェクトの追加
void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
}

// 2Dオブジェクトの全削除
void ObjectManager::DeleteAll2D()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); /*ここは空っぽなので注意*/)
	{
		Object2D* temp = *itr;

		// リストから削除
		itr = mObject2DList.erase(itr);

		// オブジェクトそのものを削除
		delete temp;
		temp = nullptr;
	}
}


// 削除する必要のあるオブジェクトがあれば削除する
void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); /*ここは空っぽなので注意*/)
	{
		// 破棄フラグが立っていれば削除する
		if ( (*itr)->IsDeleteFlag() )
		{
			Object2D *temp = *itr;

			// リストから削除
			// erase() は、削除した itr の次の要素を返却してくれる
			itr = mObject2DList.erase(itr);

			// オブジェクトそのものを削除
			delete temp;
			temp = nullptr;
		}
		else
		{
			// 次の要素へ進める
			itr++;
		}
	}

}

// 指定したタグの2Dオブジェクトを取得
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag tag)
{
	// std::find を利用して対象のオブジェクトを探す
	auto itr = std::find_if(
		mObject2DList.begin(),
		mObject2DList.end(),
		[&](Object2D* obj) { return obj->GetTag() == tag; }   // ラムダ式
		// [&] ... 今回の場合、mObject2Dlist の要素を[参照]するという意味合い
		// (Object2D *obj) ... 参照したオブジェクトの型と引数名
		// { ... } ... 処理内容（今回は条件式）
		// [](){ ... } この形がラムダ式の基本
	);


	// 見つかったかどうか判定
	if (itr != mObject2DList.end())
	{
		return (*itr);    // オブジェクトが見つかった
	}

	return nullptr;      // オブジェクトが見つからなかった
}


// 指定したタグの2Dオブジェクトのリストを取得
std::vector<Object2D*> ObjectManager::GetObject2DListByTag(Object2D::Tag tag)
{
	std::vector<Object2D*> ret;

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		// tag と同じタグを持っているオブジェクトがあればvectorに入れる
		if ( (*itr)->GetTag() == tag )
		{
			ret.push_back((*itr));
		}
	}


	return ret;
}
