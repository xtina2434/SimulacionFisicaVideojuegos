#include "ElasticBandForceGenerator.h"

ElasticBandForceGenerator::ElasticBandForceGenerator(double _k, double _r_l, Particle* _other) :
	SpringForceGenerator(_k,_r_l,_other)
{
}
ElasticBandForceGenerator::~ElasticBandForceGenerator() {

}
void
ElasticBandForceGenerator::updateForce(Particle* p, double t) {
	
	if (other) {
		//vector que conecta las particulas
		Vector3 relative_pos_vector = other->getPos() - p->getPos();
		
		float length = relative_pos_vector.normalize();	//distancia entre las dos particulas
		
		//la fuerza solo se aplica si la distancia supera la longitud en reposo
		if (length > resting_length) {
			float delta_x = length - resting_length;				 //deformacion goma
			Vector3 elastic_force = relative_pos_vector* delta_x* k; //ley de hook;
			p->addForce(elastic_force);
		}

	}
}