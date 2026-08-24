#include "OriginalBullet.h"
#include "Enemy.h"

OriginalBullet::OriginalBullet(VECTOR initPos, std::string filename)
	: Bullet(initPos, filename)
{
	mpTexture = new Texture(filename, initPos, true);
}

OriginalBullet::~OriginalBullet()
{
}

// 独自弾道の更新および基底Update処理の実行
// 入力: なし / 出力: なし / 副作用: 座標・衝突判定の更新
void OriginalBullet::Update()
{
	Move();
	Bullet::Update();
}

void OriginalBullet::Draw()
{
	Bullet::Draw();
}

void OriginalBullet::Move()
{
}