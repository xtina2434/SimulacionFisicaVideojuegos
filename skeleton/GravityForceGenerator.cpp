#include "GravityForceGenerator.h"
GravityForceGenerator::GravityForceGenerator(const Vector3& g) : 
		gravity(g) {}

GravityForceGenerator::~GravityForceGenerator(){

}
void GravityForceGenerator::updateForce(Particle* p, double t) {
	//if (p != nullptr && p->getMass() > 0.0f) {
	//	Vector3 g = gravity * p->getMass();
	//	p->addForce(g); //F = m * a
	//}

	if (p != nullptr) {
		p->addForce(gravity * p->getMass());
	}
}