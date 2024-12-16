#ifndef DIANA_H
#define DIANA_H

#include "core.hpp"
#include "RenderUtils.hpp"

//clase para crear la diana con dos solidos rigidos estaticos con forma de esfera, 
// una mas pequenia que simula el centro de la diana
using namespace physx;

class Diana {
public:
	Diana();
	Diana(PxPhysics* _gPhysics, PxScene* _scene,Vector3 _pose);
	~Diana();

	void setTransform(const Vector3& transform);

protected:
	
	PxTransform		pose;
	PxRigidStatic* rigid = nullptr;
	PxRigidStatic* center = nullptr;
	RenderItem* item1 = nullptr;
	RenderItem* item2 = nullptr;
};

#endif