#pragma once
#include "Scene.h"


class GameOverScene : public Scene
{


public:
	GameOverScene();
	~GameOverScene();

	// ‰Šú‰»
	void Initialize() override;
	// XV
	void Update() override;
	// •`‰æ
	void Draw() override;
	// I—¹ˆ—
	void Finalize() override;

};