#ifndef PROYECTIL_H
#define PROYECTIL_H
#include "Particle.h"
class Proyectil : public Particle
{
public:
	Proyectil() : Particle() {}
	Proyectil(Vector3 Pos, Vector3 Vel, Vector3 Acel, int Size, Vector4 Color, int LifeTime, int masa);
	~Proyectil();

	//void integrate(double t) override;

private:
	int masa;
};

#endif