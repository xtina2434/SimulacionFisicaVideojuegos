#include "WhirlwindForceGenerator.h"

WhirlwindForceGenerator::WhirlwindForceGenerator(const Vector3& c, float r, float _k, float k1) :
	WindForceGenerator(Vector3(0,0,0), k1, 0.0f, c, r),  k(_k)
{
	if(sphere) DeregisterRenderItem(sphere);
}

void
WhirlwindForceGenerator::updateForce(Particle* p, double t) {

	//evitar division entre 0
	if (p != nullptr && p->getMass() > 0.0f) {

		const Vector3 particle_pos = p->getPos();

		//distancia de la particula al centro del torbellino
		const Vector3 particle_to_center = particle_pos - center;//vector desde la pos de la particula al centro del torbellino

		//velocidad tangencial perpendicular al vector de la diferencia en 2D
		const Vector3 tangential_vel = Vector3(-particle_to_center.z, 0, particle_to_center.x);  //gira 90º en 2D (X-Z)

		const Vector3 whirlwind_vel = k * tangential_vel;

		const Vector3 vel_diff = whirlwind_vel - p->getVel();

		//Fuerza del torbellino
		const Vector3 whirlwind_force = k1 * vel_diff;
		p->addForce(whirlwind_force);
		
	}

}

