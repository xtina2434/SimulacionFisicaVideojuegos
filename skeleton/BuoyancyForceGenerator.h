#ifndef BUOYANCYFORCEGENERATOR_H
#define BUOYANCYFORCEGENERATOR_H

#include "ForceGenerator.h"
#include "core.hpp"
class BuoyancyForceGenerator : public ForceGenerator {
public:
	BuoyancyForceGenerator(float d);
	~BuoyancyForceGenerator();

	void updateForce(Particle* p, double t) override;

	void setliquid_particle(Particle* p) { liquid_particle = p; }
protected:
	float density ;

	Particle* liquid_particle;

};

#endif

