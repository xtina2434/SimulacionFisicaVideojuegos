#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(Vector3 p1, Vector3 p2, float r)
{
	Vector3 zeroVector = { 0.0f,0.0f,0.0f };

	if (!(p1 == zeroVector && p2 == zeroVector && r == 0.0f)) {
		use_bounding = true;

		//si radios no es 0 se crea una esfera, sino un cubo
		if (r) {
			bounding_shape = new BoundingSphere(p1, r);
		}
		else {
			bounding_shape = new BoundingBox(p1, p2);
		}
	}
	else use_bounding = false;
}

ForceGenerator::~ForceGenerator()
{
	if (bounding_shape != nullptr) {
		delete bounding_shape;
		bounding_shape = nullptr;
	}
}
