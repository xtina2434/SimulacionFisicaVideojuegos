#ifndef PROYECTIL_H
#define PROYECTIL_H
#include "Particle.h"
class Proyectil : public Particle
{
public:
	Proyectil() : Particle() {}
	Proyectil(Vector3 Pos, Vector3 Vel, Vector3 Acel,float Size, Vector4 Color, double LifeTime, float mass);
	~Proyectil();

private:
	Vector3 acel;
};

#endif