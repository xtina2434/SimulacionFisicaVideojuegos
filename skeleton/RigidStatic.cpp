#include "RigidStatic.h"

RigidStatic::RigidStatic(PxPhysics* _gPhysics, PxScene* _scene, Vector3 _pose, Vector3 _size, Vector4 _color, std::string SHAPE, const char* name) :
	pose(_pose)
{
	solid = _gPhysics->createRigidStatic(pose);

	PxShape* shape = nullptr;
	if (SHAPE == "BOX") {
		shape = CreateShape(PxBoxGeometry(_size));
	}
	else if (SHAPE == "SPHERE") {
		shape = CreateShape(PxSphereGeometry(_size.x));
	}
	
	solid->attachShape(*shape);
	_scene->addActor(*solid);
	item = new RenderItem(shape, solid, _color);

	solid->setName(name);
	solid->userData = this;
}

RigidStatic::~RigidStatic()
{
	if (item) {
		DeregisterRenderItem(item);
	}
	if (solid) {
		solid->release();
	}
}
