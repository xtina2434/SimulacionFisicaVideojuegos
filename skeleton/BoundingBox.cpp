#include "BoundingBox.h"

// Todos los numeros del punto 1 tienen que ser menos al del 2 para poder crear bien la caja
BoundingBox::BoundingBox(Vector3 point1, Vector3 point2) :Bounding()
{
    p1 = point1;
    p2 = point2;

    tamanyo = p2 - p1;

    shapeTransform = physx::PxTransform(p1 + tamanyo / 2);
    shapeItemRender = new RenderItem(CreateShape(physx::PxBoxGeometry(tamanyo)), &shapeTransform, { 1.0f, 1.0f, 0.0f, 0.0f });
}

bool BoundingBox::isInside(Vector3 point)
{
    // Compruebo que el punto este dentro de los limites
    return point.x >= p1.x && point.x <= p2.x && point.y >= p1.y && point.y <= p2.y && point.z >= p1.z && point.z <= p2.z;
}