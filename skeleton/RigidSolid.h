#ifndef RIGIDSOLID_H
#define RIGIDSOLID_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include <string>

using namespace physx;

class RigidSolid {
public:
	RigidSolid();
	RigidSolid(PxPhysics* _gPhysics, PxScene* _scene, PxMaterial* _mat,
				Vector3 _pose, Vector3 _l_v, Vector3 _w_v, Vector3 _size, Vector4 _color,
				float _d, float _t, std::string shape);
	~RigidSolid();


	//getters
	Vector3 getPosition() const{ 
		if (solid) return solid->getGlobalPose().p;
	}
	bool isAlive() const{ return is_alive; }

	//setters
	/*void setInertia() { 
		solid->setMassSpaceInertiaTensor({ size.y * size.z, size.x * size.z, size.x * size.y });
	}*/
	void setInertia(Vector3 inertia) {
		solid->setMassSpaceInertiaTensor(inertia);
	}
	void setPosition(Vector3 pos) { 
		pose.p = pos; 
		solid->setGlobalPose(pose);
	}
	void setLinearVel(Vector3 lin_vel) { 
		lineal_vel = lin_vel; 
		solid->setLinearVelocity(lineal_vel);
	}
	void setAngularVel(Vector3 ang_vel) {
		angular_vel = ang_vel; 
		solid->setAngularVelocity(angular_vel);
	}
	void setMass(float m) {
		mass = m;
		solid->setMass(m);
	}
	void setMaterialProperties(float restitution, float static_friction, float dynamic_friction) {
		material->setRestitution(restitution);
		material->setStaticFriction(static_friction);
		material->setDynamicFriction(dynamic_friction);
	}
	void integrate(double t);

	
protected:
	PxRigidDynamic* solid = nullptr;
	RenderItem*		item  = nullptr;
	PxMaterial* material = nullptr;

	PxTransform		pose;
	Vector3			lineal_vel;
	Vector3			angular_vel;
	Vector3			size;
	Vector4			color;

	float			density;
	float			mass;
	float			life_time;
	bool			is_alive;
};

#endif