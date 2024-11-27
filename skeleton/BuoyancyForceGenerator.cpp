#include "BuoyancyForceGenerator.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float v, float d) :
	height(h), volume(v), density(d) {

}
BuoyancyForceGenerator::~BuoyancyForceGenerator() {

}
void
BuoyancyForceGenerator::updateForce(Particle* p, double t) {
	if (p != nullptr || liquid_particle == nullptr) {
		float h = p->getPos().y;
		float h0 = liquid_particle->getPos().y;

		Vector3 force(0, 0, 0);
		float immersed = 0.0;

		if (h - h0 > height * 0.5) {
			immersed = 0.0;
		}
		else if (h0 - h > height * 0.5) {
			immersed = 1.0;
		}
		else {
			immersed = (h0 - h) / height + 0.5;
		}
		force.y = density * volume * immersed * gravity;
		p->addForce(force);
	}
	
}