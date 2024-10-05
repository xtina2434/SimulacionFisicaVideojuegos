#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.hpp"
#include "RenderUtils.hpp"

//Clase particula
constexpr double doumping = 0.98;
class Particle {
	public:
		Particle();
		Particle(Vector3 Pos, Vector3 Vel, Vector3 Acel);
		~Particle();

		virtual void integrate(double t);

	protected:
		Vector3 vel;
		Vector3 acel;
		physx::PxTransform pose; //A render item le pasaremos la direccion de este pose, para que se actualice automaticamente
		RenderItem* renderItem;
};

#endif