#pragma once
#include "DxLib.h"

// 2Dゲームにおける幾何学的な当たり判定（円・矩形・点）を提供するユーティリティクラス
class Collision
{
public:
	Collision() {}
	~Collision() {}

	// 2つの円同士の交差判定（三平方の定理による距離の二乗比較、平方根計算を回避し二乗のまま比較して高速化）
	// 入力: centerPositionA, radiusA, centerPositionB, radiusB / 出力: 衝突していればtrue / 副作用: なし
	static bool CheckCircleToCircle(
		const VECTOR& centerPositionA,
		const float& radiusA,
		const VECTOR& centerPositionB,
		const float& radiusB
	);

	// 点と円の包含判定（カーソルや着弾点の接触検知）
	// 入力: pointPosition(点座標), centerPosition(円の中心), radius(円の半径) / 出力: 内包していればtrue / 副作用: なし
	static bool CheckPointToCircle(
		const VECTOR& pointPosition,
		const VECTOR& centerPosition,
		const float& radius
	);

	// 2つの軸平行境界ボックス（AABB）同士の矩形重なり判定
	// 入力: leftTopPositionA, widthA, heightA, leftTopPositionB, widthB, heightB / 出力: 重なっていればtrue / 副作用: なし
	static bool CheckRectToRect(
		const VECTOR& leftTopPositionA,
		const float& widthA,
		const float& heightA,
		const VECTOR& leftTopPositionB,
		const float& widthB,
		const float& heightB
	);
};