#include "Object2D.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Utility.h"

// 単一静止画テクスチャを持つオブジェクトの生成
// 入力: fileName(画像パス), initPosition(初期座標) / 出力: なし / 副作用: ObjectManagerへの自動登録
Object2D::Object2D(std::string fileName, VECTOR initPosition)
	: m_position(initPosition)
	, m_deleteFlag(false)
	, m_textureAnimation(nullptr)
{
	// シーン管理下へ自身を登録し、一括更新・描画の対象にする
	Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
	m_texture = new Texture(fileName, initPosition, true);
}

// スプライトシートアニメーションを持つオブジェクトの生成
// 入力: filename, initPos, allNum, numX, numY, interval, scale, type / 出力: なし / 副作用: ObjectManagerへの自動登録
Object2D::Object2D(std::string fileName, VECTOR initPosition, int allNum, int numX, int numY, int interval, float scale, bool type)
	: m_position(initPosition)
	, m_deleteFlag(false)
	, m_texture(nullptr)
{
	Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
	m_textureAnimation = new TextureAnimation(fileName, initPosition, allNum, numX, numY, interval, scale, type);
}

// 保持する画像リソースの破棄
// 入力: なし / 出力: なし / 副作用: テクスチャメモリの解放
Object2D::~Object2D()
{
	if (m_texture != nullptr)
	{
		delete m_texture;
	}

	if (m_textureAnimation != nullptr)
	{
		delete m_textureAnimation;
	}
}

// 毎フレームの座標更新や内部状態の進行
// 入力: なし / 出力: なし / 副作用: 座標やテクスチャの更新
void Object2D::Update()
{
	if (m_texture != nullptr)
	{
		m_texture->Update();
		m_texture->SetPosition(m_position);
	}

	if (m_textureAnimation != nullptr)
	{
		m_textureAnimation->Update();
		m_textureAnimation->SetPosition(m_position);	
	}
}

// カメラ座標を加味した画面描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Object2D::Draw()
{
	if (m_texture != nullptr)
	{
		m_texture->Draw(g_cameraX, g_cameraY);
	}

	if (m_textureAnimation != nullptr)
	{
		m_textureAnimation->Draw(g_cameraX, g_cameraY);
	}
}

void Object2D::Reset()
{
	if (m_textureAnimation != nullptr)
	{
		m_textureAnimation->Reset();
	}
}

// 当たり判定等で使用する近似半径の取得
// 入力: なし / 出力: 半径(px) / 副作用: なし
float Object2D::GetRadius()
{
	if (m_texture != nullptr)
	{
		return m_texture->GetRadius();
	}

	if (m_textureAnimation != nullptr)
	{
		return m_textureAnimation->GetRadius();
	}

	return 0.0f;
}

int Object2D::GetSizeX()
{
	if (m_texture != nullptr)
	{
		return m_texture->GetSizeX();
	}

	if (m_textureAnimation != nullptr)
	{
		return m_textureAnimation->GetSizeX();
	}

	return 0;
}

int Object2D::GetSizeY()
{
	if (m_texture != nullptr)
	{
		return m_texture->GetSizeY();
	}

	if (m_textureAnimation != nullptr)
	{
		return m_textureAnimation->GetSizeY();
	}

	return 0;
}

