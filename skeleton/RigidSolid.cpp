#include "RigidSolid.h"

RigidSolid::RigidSolid()
{
}

RigidSolid::RigidSolid(PxPhysics* _gPhysics, PxScene* _scene, PxMaterial* _material,
					   Vector3 _pose, Vector3 _l_v, Vector3 _w_v, Vector3 _size, Vector4 _color,
						float _d, float _t, std::string SHAPE) :
	 material(_material),pose(_pose), lineal_vel(_l_v), angular_vel(_w_v), size(_size), color(_color),
	density(_d), life_time(_t)
{
	solid = _gPhysics->createRigidDynamic(pose);
	solid->setLinearVelocity(lineal_vel);
	solid->setAngularVelocity(angular_vel);

	PxShape* shape;
	if (SHAPE == "BOX") {
		shape = CreateShape(PxBoxGeometry(size));
	}
	else if (SHAPE == "SPHERE") {
		shape = CreateShape(PxSphereGeometry(size.x));
	}
	solid->attachShape(*shape);

	shape->setMaterials(&material, 1);	//asignar material al shape
	
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
	/*if (material) {
		material->release();
	}*/
}

void RigidSolid::integrate(double t)
{
	life_time -= t;

	if (life_time <= 0) is_alive = false;
}
