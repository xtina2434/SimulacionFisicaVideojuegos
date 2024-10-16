#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.hpp"
#include "RenderUtils.hpp"

//Clase particula
constexpr double doumping = 0.98;
class Particle {
	public:
		Particle();
		Particle(Vector3 Pos, Vector3 Vel, Vector3 Acel, int Size, Vector4 Color, int LifeTime);
		~Particle();

		virtual void integrate(double t);
		physx::PxTransform getPos() { return pose; }
	protected:
		Vector3 vel;			 //velocidad inicial
		Vector3 acel;			 //aceleracion/gravedad
		physx::PxTransform pose; //A render item le pasaremos la direccion de este pose, para que se actualice automaticamente
		RenderItem* renderItem;
		int lifeTime;			//tiempo de vida
		int size;				//tamaño inicial
		Vector4 color;			//color inicial
};

#endif