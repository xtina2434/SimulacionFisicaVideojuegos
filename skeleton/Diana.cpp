#include "Diana.h"

Diana::Diana(PxPhysics* _gPhysics, PxScene* _scene, Vector3 _pose) :
	pose(_pose)
{
	rigid = _gPhysics->createRigidStatic(pose);
	PxShape* shape1 = CreateShape(PxSphereGeometry(4));
	rigid->attachShape(*shape1);
	_scene->addActor(*rigid);
	item1 = new RenderItem(shape1, rigid, { 1.0,0.0,0.0,1.0 });
	rigid->setName("Diana");
	
	PxTransform pose2 = { pose.p.x += 4, pose.p.y, pose.p.z };
	center = _gPhysics->createRigidStatic(pose2);
	PxShape* shape2 = CreateShape(PxSphereGeometry(2));
	center->attachShape(*shape1);
	_scene->addActor(*center);
	item2 = new RenderItem(shape2, center, { 1.0,1.0,1.0,1.0 });
	center->setName("Diana_center");
}

Diana::~Diana()
{
	if (item1) {
		DeregisterRenderItem(item1);
	}
	if (item2) {
		DeregisterRenderItem(item2);
	}
	if (rigid) {
		rigid->release();
	}
	if (center) {
		center->release();
	}
}

void Diana::setTransform(const Vector3& transform)
{
	pose.p = transform;
	rigid->setGlobalPose(pose);
	PxTransform pose2 = { pose.p.x += 4, pose.p.y, pose.p.z };
	center->setGlobalPose(pose);
}
