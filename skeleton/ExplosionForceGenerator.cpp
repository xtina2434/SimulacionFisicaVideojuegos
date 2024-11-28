#include "ExplosionForceGenerator.h"


ExplosionForceGenerator::ExplosionForceGenerator(float _R, float _k, float _t, Vector3 _c) :
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