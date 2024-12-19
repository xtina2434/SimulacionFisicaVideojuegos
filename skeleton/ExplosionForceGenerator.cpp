#include "ExplosionForceGenerator.h"


ExplosionForceGenerator::ExplosionForceGenerator(float _R, double _k, float _t, Vector3 _c) :
	Radius(_R), k(_k), t(_t), center(_c) {

}

ExplosionForceGenerator::~ExplosionForceGenerator() {

}

void
ExplosionForceGenerator::updateForce(Particle* p, double time) {
	if (p != nullptr && p->getMass() > 0.0f) {
		Vector3 particle_pos = p->getPos();

		//distancia de la particula al centro de la explosion
		Vector3 direction = particle_pos - center; //vector en direccion de la fuerza
		r = direction.magnitude(); //distancia a la explosion
		
		//solo se aplica la fuerza si la particula esta dentro del radio de la explosion
		if (r < Radius) {

			direction.normalize(); //normalizar obtener solo la direccion de la fuerza

			Vector3 explosion_force = (k / (r * r)) * direction * std::exp(-time / t);

			p->addForce(explosion_force);
		}
	}
}

void ExplosionForceGenerator::updateForce(RigidSolid* s, double time)
{
	if (s != nullptr && s->isAlive()) {
		Vector3 solid_pos = s->getPosition();
		Vector3 direction = solid_pos - center; //direccion fuerza hacia el solido
		r = direction.magnitude(); //distancia entre el centro de la explosion y el solido
		
		//solo se aplica la fuerza si la particula esta dentro del radio de la explosion
		if (r < Radius) {

			direction.normalize(); //normalizar obtener solo la direccion de la fuerza
			Vector3 explosion_force = (k / (r * r)) * direction * std::exp(-time / t);

			//aplicar fuerza en el centro de masas del solido rigido
			s->addForce(explosion_force);

			//aplicar el par, si la fuerza no es aplicada en el centro de masa
			//se puede aplicar la fuerza en un punto distinto al centro de masa
			Vector3 offset = solid_pos - center; //vector desde el centro de la explosion al centro de masas
			Vector3 torque = offset.cross(explosion_force); //generacion de par
			s->addTorque(torque);
		}
	}
}
