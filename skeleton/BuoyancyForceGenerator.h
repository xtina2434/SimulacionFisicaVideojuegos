#ifndef BUOYANCYFORCEGENERATOR_H
#define BUOYANCYFORCEGENERATOR_H

#include "ForceGenerator.h"
#include "core.hpp"
class BuoyancyForceGenerator : public ForceGenerator {
public:
	BuoyancyForceGenerator(float h, float v, float d);
	~BuoyancyForceGenerator();

	void updateForce(Particle* p, double t) override;
protected:
	float height;
	float volume;
	float density;
	float gravity = 9.8;

	Particle* liquid_particle;

};

#endif

