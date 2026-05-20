#pragma once
#include "Object2D.h"

class WireTarget : public Object2D
{
public:
	WireTarget(std::string filename, VECTOR initPos);
	virtual ~WireTarget();

	void Update() override;
	void Draw() override;
};
