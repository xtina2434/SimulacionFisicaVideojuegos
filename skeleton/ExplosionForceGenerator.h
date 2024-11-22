#ifndef EXPLOSIONFORCEGENERATOR_H
#define EXPLOSIONFORCEGENERATOR_H

#include "ForceGenerator.h"
#include <cmath>

class ExplosionForceGenerator : public ForceGenerator {

public:
	ExplosionForceGenerator(float Radius, float k, float t, Vector3 center);
	~ExplosionForceGenerator();

	void updateForce(Particle* p, double t) override;
private:

	float Radius;	//radio de la explosion
	float k;		//intensidad de la explosion
	float t;		//cte de tiempo de la explosion
	float r;		//distancia a la explosion
	Vector3 center;	//centro de la explosion

};


#endif

#pragma once
