#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H


#include "Particle.h"

//clase abstracta para los diferentes generadores de fuerzas
class ForceGenerator{

	public:
		virtual ~ForceGenerator() = default;

		virtual void updateForce(Particle* p, double t) = 0;
};

#endif
