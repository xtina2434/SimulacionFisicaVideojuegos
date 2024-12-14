#ifndef RIGIDSTATIC_H
#define RIGIDSTATIC_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include <string>

using namespace physx;

class RigidStatic {
public:
	RigidStatic();
	RigidStatic(PxPhysics* _gPhysics, PxScene* _scene,
		Vector3 _pose, Vector3 _size, Vector4 _color,  std::string shape, const char* name);
	~RigidStatic();


protected:
	PxRigidStatic* solid = nullptr;
	PxTransform		pose;
	RenderItem* item = nullptr;
};


#endif