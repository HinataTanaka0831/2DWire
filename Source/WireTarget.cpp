#include "WireTarget.h"

WireTarget::WireTarget(std::string filename, VECTOR initPos)
	: Object2D(filename, initPos)
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