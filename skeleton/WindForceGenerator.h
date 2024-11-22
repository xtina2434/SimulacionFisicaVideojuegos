#ifndef WINDFORCEGENERATOR_H
#define WINDFORCEGENERATOR_H

#include "ForceGenerator.h"


class WindForceGenerator : public ForceGenerator {

public:
	WindForceGenerator(const Vector3& windVel, float k1, float k2, Vector3 sphereCenter, float sphereRadius);
	//WindForceGenerator(float air_density, float drag_coeff, const Vector3& windVel, Vector3 sphereCenter, float sphereRadius);
	~WindForceGenerator();

	void updateForce(Particle* p, double t) override;
protected:
	Vector3 wind_vel;	//velocidad del viento
	float k1;			//coeficiente de rozamiento
	float k2;			//coeficiente para flujo turbulento
	Vector3 center;
	float radius;
	RenderItem* sphere;

	float air_density; //densidad del aire
	float drag_coeff; //coeficiente aerodinamico

};


#endif

