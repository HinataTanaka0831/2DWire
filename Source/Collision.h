#pragma once
#include "DxLib.h"

// 2Dゲームにおける幾何学的な当たり判定（円・矩形・点）を提供するユーティリティクラス
class Collision
{
public:
	// AABB矩形判定用構造体
	struct Rect {
		float X;
		float Y;
		float Width;
		float Height;
	};

public:
	Collision() {}
	~Collision() {}

	// 2つの円同士の交差判定（三平方の定理による距離の二乗比較）
	// 入力: centerPosA, radiusA, centerPosB, radiusB / 出力: 衝突していればtrue / 副作用: なし
	static bool CheckCircleToCircle(
		const VECTOR& centerPosA,
		const float& radiusA,
		const VECTOR& centerPosB,
		const float& radiusB
	);

	// 点と円の包含判定（カーソルや着弾点の接触検知）
	// 入力: pointPos(点座標), centerPos(円の中心), radius(円の半径) / 出力: 内包していればtrue / 副作用: なし
	static bool CheckPointToCircle(
		const VECTOR& pointPos,
		const VECTOR& centerPos,
		const float& radius
	);

	// 2つの軸平行境界ボックス（AABB）同士の矩形重なり判定
	// 入力: rectPositionA, rectWidthA, rectHeightA, rectPositionB, rectWidthB, rectHeightB / 出力: 重なっていればtrue / 副作用: なし
	static bool CheckRectToRect(
		const VECTOR& rectPositionA,
		const float& rectWidthA,
		const float& rectHeightA,
		const VECTOR& rectPositionB,
		const float& rectWidthB,
		const float& rectHeightB
	);
};