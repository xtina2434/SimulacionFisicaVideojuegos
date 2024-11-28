#ifndef WHIRLWINDFORCEGENERATOR_H
#define WHIRLWINDFORCEGENERATOR_H

#include "WindForceGenerator.h"


class WhirlwindForceGenerator : public WindForceGenerator {

public:
	WhirlwindForceGenerator(const Vector3& center, float radius, float k, float k1);
	~WhirlwindForceGenerator() = default;

	void updateForce(Particle* p, double t) override;
private:
	float k;				//cte del torbellino
	Vector3 center;
	float radius;
};


#endif

