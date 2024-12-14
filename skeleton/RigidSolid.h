#ifndef RIGIDSOLID_H
#define RIGIDSOLID_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include <string>

using namespace physx;
class ForceGenerator;
class RigidSolid {
public:
	RigidSolid();
	RigidSolid(PxPhysics* _gPhysics, PxScene* _scene, PxMaterial* _mat,
				Vector3 _pose, Vector3 _l_v, Vector3 _w_v, Vector3 _size, Vector4 _color,
				float _d, float _t, std::string shape, const char* name);
	~RigidSolid();


	//getters
	Vector3 getPosition() const{ 
		if (solid) return solid->getGlobalPose().p;
	}
	bool isAlive() const{ return alive && life_time > 0 && pose.p.y > 0; }
	float getMass() const { return solid->getMass(); }
	Vector3 getLinealVel() const { return lineal_vel; }
	float getSize() const { return size.x; }
	Vector3 getVolumen() const { return size; }
	PxRigidDynamic* getSolid() const { return solid; }
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
	void addForce(Vector3 f) {
		if (solid)
			solid->addForce(PxVec3(f.x, f.y, f.z), PxForceMode::eFORCE);
	}
	void addTorque(Vector3 t) {
		if (solid)
			solid->addTorque(PxVec3(t.x, t.y, t.z));
	}
	void addForceGenerator(ForceGenerator* fg);
	void integrate(double t);
	void update(double t);
	void die() { alive = false; }
	
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
	bool			alive = true;
	std::vector<ForceGenerator*> generators;


};

#endif