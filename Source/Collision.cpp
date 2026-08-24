#include "Collision.h"
#include <cmath>

// 2つの円同士の交差判定（平方根計算を回避し二乗のまま比較して高速化）
// 入力: centerPosA, radiusA, centerPosB, radiusB / 出力: 衝突していればtrue / 副作用: なし
bool Collision::CheckCircleToCircle(
	const VECTOR& centerPosA,
	const float& radiusA,
	const VECTOR& centerPosB,
	const float& radiusB)
{
	float distanceX = centerPosA.x - centerPosB.x;
	float distanceY = centerPosA.y - centerPosB.y;

	// sqrtコストを避けるため三平方の定理(dx^2 + dy^2 <= (r1+r2)^2)で判定
	float powX = distanceX * distanceX;
	float powY = distanceY * distanceY;
	float powR = (radiusA + radiusB) * (radiusA + radiusB);

	if ((powX + powY) <= powR)
	{
		return true;
	}

	return false;
}

// 点と円の包含判定（クリック位置や座標判定用）
// 入力: pointPos(点座標), centerPos(円の中心), radius(円の半径) / 出力: 内包していればtrue / 副作用: なし
bool Collision::CheckPointToCircle(
	const VECTOR& pointPos,
	const VECTOR& centerPos,
	const float& radius
)
{
	float distanceX = centerPos.x - pointPos.x;
	float distanceY = centerPos.y - pointPos.y;

	// 点と円中心の距離二乗が半径二乗以内か判定
	float powX = distanceX * distanceX;
	float powY = distanceY * distanceY;
	float powR = radius * radius;

	if ((powX + powY) <= powR)
	{
		return true;
	}

	return false;
}

// 2つの軸平行境界ボックス（AABB）同士の矩形重なり判定
// 入力: rectPositionA, rectWidthA, rectHeightA, rectPositionB, rectWidthB, rectHeightB / 出力: 重なっていればtrue / 副作用: なし
bool Collision::CheckRectToRect(
	const VECTOR& rectPositionA,
	const float& rectWidthA,
	const float& rectHeightA,
	const VECTOR& rectPositionB,
	const float& rectWidthB,
	const float& rectHeightB
)
{
	VECTOR leftTopA = rectPositionA;
	VECTOR rightBottomA = VGet(rectPositionA.x + rectWidthA, rectPositionA.y + rectHeightA, 0.0f);

	VECTOR leftTopB = rectPositionB;
	VECTOR rightBottomB = VGet(rectPositionB.x + rectWidthB, rectPositionB.y + rectHeightB, 0.0f);

	// X軸・Y軸の両方で投影区間が交差しているかを評価
	bool isLeftLessRight = (leftTopA.x <= rightBottomB.x);
	bool isRightGreaterLeft = (rightBottomA.x >= leftTopB.x);
	bool isTopLessBottom = (leftTopA.y <= rightBottomB.y);
	bool isBottomGreaterTop = (rightBottomA.y >= leftTopB.y);

	if (isLeftLessRight && isRightGreaterLeft && isTopLessBottom && isBottomGreaterTop)
	{
		return true;
	}

	return false;
}