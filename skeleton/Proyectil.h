#pragma once
#include "Particle.h"
class Proyectil : public Particle
{
public:
	Proyectil() : Particle() {}
	Proyectil(Vector3 Pos, Vector3 Vel, Vector3 Acel, int masa);
	~Proyectil();

	void integrate(double t) override;

private:
	int masa;
};

