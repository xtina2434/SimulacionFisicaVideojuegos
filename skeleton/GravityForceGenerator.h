#ifndef GRAVITYFORCEGENERATOR_H
#define GRAVITYFORCEGENERATOR_H

#include "ForceGenerator.h"

//generador de fuerzas gravitatorio que aplica la fuerza gravitatoria en funcion de la masa

class GravityForceGenerator : public ForceGenerator {
	
	private:
		Vector3 gravity;
	public:
		GravityForceGenerator(const Vector3& g);
		~GravityForceGenerator();

		void updateForce(Particle* p, double t) override;
};


#endif

