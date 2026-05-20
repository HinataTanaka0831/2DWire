#include "Collision.h"

bool Collision::CheckCircleToCircle(
	const VECTOR& centerPosA,
	const float& radiusA,
	const VECTOR& centerPosB,
	const float& radiusB)
{
	// “–‚½‚è”»’è‚ğ‘‚­@O•½•û‚Ì’è—(a^2 + b^2 = c^2)

	// X²‚Ì‹——£i—Î‚Ì‰¡üj
	float distanceX = centerPosA.x - centerPosB.x;
	// Y²‚Ì‹——£i—Î‚Ìcüj
	float distanceY = centerPosA.y - centerPosB.y;

	// ğŒ
	float powX = distanceX * distanceX; // ‰¡ü‚Ì2æ
	float powY = distanceY * distanceY; // cü‚Ì‚Qæ
	float powR = (radiusA + radiusB) * (radiusA + radiusB);  // ‚»‚ê‚¼‚ê‚Ì”¼Œa‚ğ‘«‚µ‚½‚à‚Ì‚Ì‚Qæ
	if ((powX + powY) <= powR)
	{
		return true;       // ¬Œ÷
	}

	// ğŒ‚ğ–‚½‚³‚È‚¢‚Ì‚Å‚ ‚ê‚Î¸”s
	return false;
}

bool Collision::CheckPointToCircle(
	const VECTOR& pointPos,
	const VECTOR& centerPos,
	const float& radius
)
{
	// X²‚Ì‹——£
	float distanceX = centerPos.x - pointPos.x;
	// Y²‚Ì‹——£
	float distanceY = centerPos.y - pointPos.y;

	// ”»’è
	float powX = distanceX * distanceX;   // ‰¡ü‚Ì‚Qæ
	float powY = distanceY * distanceY;   // cü‚Ì‚Qæ
	float powR = radius * radius;         // ”¼Œa‚Ì‚Qæ

	if ((powX + powY) <= powR)
	{
		return true;    // “–‚½‚Á‚Ä‚¢‚é
	}

	// ğŒ‚ğ–‚½‚³‚È‚¢‚Ì‚Å‚ ‚ê‚Î¸”s
	return false;
}
