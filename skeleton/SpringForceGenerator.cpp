#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double _k, double _r_l, Particle* _other) :
	k(_k), resting_length(_r_l), other(_other)
{
	anchor = Vector3(0, 0, 0);
	use_anchor = false;
}
SpringForceGenerator::SpringForceGenerator(double _k, double _r_l, const Vector3& _anchor) :
	k(_k), resting_length(_r_l), anchor(_anchor)
{
	other = nullptr;
	use_anchor = true;
}

SpringForceGenerator::~SpringForceGenerator() {
	delete other;
}

void
SpringForceGenerator::updateForce(Particle* p, double t) {

	//vector que conecta la particula p con el otro extremo del muelle (otra particula, punto fijo)
	Vector3 relative_pos_vector;
	if (use_anchor) {
		relative_pos_vector = anchor  - p->getPos();
	} 
	else if (other) {
		relative_pos_vector = other->getPos() - p->getPos();
	}
	Vector3 spring_force;

	//normalizar la posicion relativa y devolver su longitud
	float length = relative_pos_vector.normalize();	//distancia entre los dos extremos del muelle
	float diff = length - resting_length;		//diferencia entre la longitud actual del muelle y su longitud en reposo

	spring_force = relative_pos_vector * diff * k; //ley de hook

	p->addForce(spring_force);
}

void SpringForceGenerator::updateForce(RigidSolid* s, double t)
{
	//vector que conecta la particula p con el otro extremo del muelle (otra particula, punto fijo)
	Vector3 relative_pos_vector;
	if (use_anchor) {
		relative_pos_vector = anchor - s->getPosition();
	}
	else if (other) {
		relative_pos_vector = other->getPos() - s->getPosition();
	}
	Vector3 spring_force;

	//normalizar la posicion relativa y devolver su longitud
	float length = relative_pos_vector.normalize();	//distancia entre los dos extremos del muelle
	float diff = length - resting_length;		//diferencia entre la longitud actual del muelle y su longitud en reposo

	spring_force = relative_pos_vector * diff * k; //ley de hook

	s->addForce(spring_force);
}
