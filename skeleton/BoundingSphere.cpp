#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(Vector3 centerPoint, float radius)
{
	center = centerPoint;
	r = radius;

	shapeTransform = physx::PxTransform(centerPoint);
	shapeItemRender = new RenderItem(CreateShape(physx::PxSphereGeometry(r)), &shapeTransform, { 1.0f, 1.0f, 0.0f, 0.0f });
}

bool BoundingSphere::isInside(Vector3 point)
{
	Vector3 m = point - center;
	return m.magnitude() <= r;
}