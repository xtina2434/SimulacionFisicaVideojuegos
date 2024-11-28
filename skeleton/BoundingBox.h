#pragma once
#include "Bounding.h"
class BoundingBox : public Bounding
{
protected:
	// Puntos para calcular la caja
	Vector3 p1;
	Vector3 p2;

	// Ancho(x) Alto(y) Fondo(z)
	Vector3 tamanyo;

	//RenderItem* shapeItemRender;
	//physx::PxTransform shapeTransform;

public:
	BoundingBox(Vector3 point1, Vector3 point2);
	virtual bool isInside(Vector3 point);
};
