#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H


#include "Particle.h"
#include "RigidSolid.h"
//clase abstracta para los diferentes generadores de fuerzas
class ForceGenerator{

	public:
		virtual ~ForceGenerator() = default;

		virtual void updateForce(Particle* p, double t) = 0;	 //actualizar fuerzas en particulas
		virtual void updateForce(RigidSolid* s, double t) = 0;	 //actualizar fuerzas en solidos rigidos
};

#endif
