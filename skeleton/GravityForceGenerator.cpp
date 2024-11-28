#include "GravityForceGenerator.h"
GravityForceGenerator::GravityForceGenerator(const Vector3& g) : 
	ForceGenerator({ 0.0f,0.0f,0.0f }, {0.0f,0.0f,0.0f}, 0.0f), gravity(g) {}

GravityForceGenerator::~GravityForceGenerator(){

}
void GravityForceGenerator::updateForce(Particle* p, double t) {

	//evitar division entre 0
	if (p != nullptr && p->getMass() > 0.0f) {
		p->addForce(gravity * p->getMass());
	}
}