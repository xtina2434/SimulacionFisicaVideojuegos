#ifndef DIANA_H
#define DIANA_H

#include "core.hpp"
#include "RenderUtils.hpp"


using namespace physx;

class Diana {
public:
	Diana();
	Diana(PxPhysics* _gPhysics, PxScene* _scene,Vector3 _pose);
	~Diana();

	void die();
	bool isAlive() const { return alive; }
	void setTransform(const Vector3& transform);

protected:
	
	bool alive = true;
	PxTransform		pose;
	PxRigidStatic* rigid = nullptr;
	PxRigidStatic* center = nullptr;
};

#endif