#include "RigidSolid.h"

RigidSolid::RigidSolid()
{
}

RigidSolid::RigidSolid(PxPhysics* _gPhysics, PxScene* _scene,
					   Vector3 _pose, Vector3 _l_v, Vector3 _w_v, Vector3 _size, Vector4 _color,
						float _d, float _t) :
	 pose(_pose), lineal_vel(_l_v), angular_vel(_w_v), size(_size), color(_color),
	density(_d), life_time(_t)
{
	solid = _gPhysics->createRigidDynamic(pose);
	solid->setLinearVelocity(lineal_vel);
	solid->setAngularVelocity(angular_vel);

	PxShape* shape = CreateShape(PxBoxGeometry(size));
	solid->attachShape(*shape);
	

	PxRigidBodyExt::updateMassAndInertia(*solid,density);
	mass = solid->getMass();

	_scene->addActor(*solid);

	item = new RenderItem(shape, solid, color);
	is_alive = true;
}

RigidSolid::~RigidSolid()
{
	if (item) {
		DeregisterRenderItem(item);
	}
	if (solid) {
		solid->release();
	}
}

void RigidSolid::integrate(double t)
{
	life_time -= t;

	if (life_time <= 0) is_alive = false;
}
