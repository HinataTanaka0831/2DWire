#include "WireTarget.h"

WireTarget::WireTarget(VECTOR initPos)
	: Object2D("Resource/WireTarget.png", initPos)
{
	SetTag(Object2D::WireTarget2D);
}

WireTarget::~WireTarget()
{
}

void WireTarget::Update()
{
	Object2D::Update();
}

void WireTarget::Draw()
{
	Object2D::Draw();
}
