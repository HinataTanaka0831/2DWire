#include "OriginalBullet.h"
#include "Enemy.h"

OriginalBullet::OriginalBullet(VECTOR initPos, std::string filename)
	:Bullet(initPos, filename)
{
	mpTexture = new Texture(
		filename,
		initPos,
		true
	);
}

OriginalBullet::~OriginalBullet()
{

}

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

//bool OriginalBullet::IsScreenOut()


