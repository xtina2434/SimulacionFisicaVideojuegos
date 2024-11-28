#pragma once
#include "Bounding.h"
class BoundingSphere : public Bounding
{
protected:
	//  Centro
	Vector3 center;
	// Radio
	float r;

public:
	BoundingSphere(Vector3 centerPoint, float radius);
	bool isInside(Vector3 point) override;
};
