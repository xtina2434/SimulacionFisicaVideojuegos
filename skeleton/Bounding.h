#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

enum Shape { BOX, SPHERE, NONE };

class Bounding
{

public:
	Bounding() {};
	virtual bool isInside(Vector3 point) = 0;
	~Bounding() {
		if (shapeItemRender != nullptr) {
			DeregisterRenderItem(shapeItemRender);
			shapeItemRender = nullptr;
		}
	};

protected:
	RenderItem* shapeItemRender;
	physx::PxTransform shapeTransform;
};
