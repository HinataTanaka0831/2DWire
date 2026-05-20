#pragma once
#include "DxLib.h"

class Collision
{
public:
	// コンストラクタ
	Collision() 
	{

	}
	//デストラクタ
	~Collision()
	{

	}

	// 円と円の当たり判定をしてくれる関数
	// const VECTOR& centerPosA ... Aの円の中心座標
	// const float& radiusA ... Aの円の半径
	// const VECTOR& centerPosB ... Bの円の中心座標
	// const float& radiusB ... Bの円の半径
	static bool CheckCircleToCircle(
		const VECTOR& centerPosA,
		const float& radiusA,
		const VECTOR& centerPosB,
		const float& radiusB
	);


	// 点と円の当たり判定
	// const VECTOR& pointPos ... 点の中心座標
	// const VECTOR& centerPos ... 円の中心座標
	// const float& radius ... 円の半径
	static bool CheckPointToCircle(
		const VECTOR& pointPos,
		const VECTOR& centerPos,
		const float& radius
	);
};
