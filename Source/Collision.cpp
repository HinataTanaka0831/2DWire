#include "Collision.h"
#include <cmath>

bool Collision::CheckCircleToCircle(
	const VECTOR& centerPosA,
	const float& radiusA,
	const VECTOR& centerPosB,
	const float& radiusB)
{
	// �����蔻��������@�O�����̒藝(a^2 + b^2 = c^2)

	// X���̋����i�΂̉����j
	float distanceX = centerPosA.x - centerPosB.x;
	// Y���̋����i�΂̏c���j
	float distanceY = centerPosA.y - centerPosB.y;

	// ����
	float powX = distanceX * distanceX; // ������2��
	float powY = distanceY * distanceY; // �c���̂Q��
	float powR = (radiusA + radiusB) * (radiusA + radiusB);  // ���ꂼ��̔��a�𑫂������̂̂Q��
	if ((powX + powY) <= powR)
	{
		return true;       // ����
	}

	// �����𖞂����Ȃ��̂ł���Ύ��s
	return false;
}

bool Collision::CheckPointToCircle(
	const VECTOR& pointPos,
	const VECTOR& centerPos,
	const float& radius
)
{
	// X���̋���
	float distanceX = centerPos.x - pointPos.x;
	// Y���̋���
	float distanceY = centerPos.y - pointPos.y;

	// ����
	float powX = distanceX * distanceX;   // �����̂Q��
	float powY = distanceY * distanceY;   // �c���̂Q��
	float powR = radius * radius;         // ���a�̂Q��

	if ((powX + powY) <= powR)
	{
		return true;    // �������Ă���
	}

	// �����𖞂����Ȃ��̂ł���Ύ��s
	return false;
}

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

	bool isLeftLessRight = false;
	if (leftTopA.x <= rightBottomB.x)
	{
		isLeftLessRight = true;
	}

	bool isRightGreaterLeft = false;
	if (rightBottomA.x >= leftTopB.x)
	{
		isRightGreaterLeft = true;
	}

	bool isTopLessBottom = false;
	if (leftTopA.y <= rightBottomB.y)
	{
		isTopLessBottom = true;
	}

	bool isBottomGreaterTop = false;
	if (rightBottomA.y >= leftTopB.y)
	{
		isBottomGreaterTop = true;
	}

	if (isLeftLessRight && isRightGreaterLeft && isTopLessBottom && isBottomGreaterTop)
	{
		return true; // �������Ă���
	}

	return false;
}