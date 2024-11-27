#ifndef ELASTICBANDFORCEGENERATOR_H
#define ELASTICBANDFORCEGENERATOR_H

#include "SpringForceGenerator.h"
#include "core.hpp"

class ElasticBandForceGenerator : public SpringForceGenerator {
public:
	ElasticBandForceGenerator(double k, double resting_length, Particle* other);
	
	~ElasticBandForceGenerator();

	void updateForce(Particle* p, double t) override;
};


#endif
