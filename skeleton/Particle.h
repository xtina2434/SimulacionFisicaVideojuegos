#ifndef PARTICLE_H
#define PARTICLE_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include <vector>
class ForceGenerator;
//Clase particula
constexpr double doumping = 0.98;
class Particle {
	public:
		Particle();
		Particle(Vector3 Pos, Vector3 Vel, /*Vector3 Acel,*/ float Size, Vector4 Color, double LifeTime, float mass);
		~Particle();

		virtual void integrate(double t);
		void update(double t);

		void addForce(Vector3 f);
		void addForceGenerator(ForceGenerator* fg);
		//getters
		bool isAlive() { return life_time > 0 && pose.p.y > 0; }
		float getMass() const { return mass; }
		//setters
		void setAcel(const Vector3& a) { acel = a; }
	protected:
		Vector3 vel;			 //velocidad inicial
		Vector3 acel;			 //aceleracion/gravedad
		Vector3 accF;
		Vector4 color;			//color inicial
		physx::PxTransform pose; //A render item le pasaremos la direccion de este pose, para que se actualice automaticamente
		RenderItem* renderItem;
		double life_time;			//tiempo de vida
		float size;				//tama�o inicial
		float mass;

		std::vector<ForceGenerator*> generators;
		void clearForces();
};

#endif