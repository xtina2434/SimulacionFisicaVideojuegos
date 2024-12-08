#include "GravityForceGenerator.h"
GravityForceGenerator::GravityForceGenerator(const Vector3& g) : 
	gravity(g) {}

GravityForceGenerator::~GravityForceGenerator(){

}
void GravityForceGenerator::updateForce(Particle* p, double t) {

	//evitar division entre 0
	if (p != nullptr && p->getMass() > 0.0f) {
		p->addForce(gravity * p->getMass());
	}
}

void GravityForceGenerator::updateForce(RigidSolid* s, double t)
{
	if (s != nullptr && s->getMass() > 0.0f) {
		s->addForce(gravity * s->getMass());
	}

}
