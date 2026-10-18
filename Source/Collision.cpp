#include "Collision.h"
#include <cmath>

bool Collision::CheckCircleToCircle(
	const VECTOR& centerPositionA,
	const float& radiusA,
	const VECTOR& centerPositionB,
	const float& radiusB
)
{
	float distanceX = centerPositionA.x - centerPositionB.x;
	float distanceY = centerPositionA.y - centerPositionB.y;

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

bool Collision::CheckPointToCircle(
	const VECTOR& pointPosition,
	const VECTOR& centerPosition,
	const float& radius
)
{
	float distanceX = centerPosition.x - pointPosition.x;
	float distanceY = centerPosition.y - pointPosition.y;

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

bool Collision::CheckRectToRect(
	const VECTOR& leftTopPositionA,
	const float& widthA,
	const float& heightA,
	const VECTOR& leftTopPositionB,
	const float& widthB,
	const float& heightB
)
{
	VECTOR leftTopA = leftTopPositionA;
	VECTOR rightBottomA = VGet(leftTopPositionA.x + widthA, leftTopPositionA.y + heightA, 0.0f);

	VECTOR leftTopB = leftTopPositionB;
	VECTOR rightBottomB = VGet(leftTopPositionB.x + widthB, leftTopPositionB.y + heightB, 0.0f);

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