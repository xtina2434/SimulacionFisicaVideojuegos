#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H


#include "Particle.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

//clase abstracta para los diferentes generadores de fuerzas
class ForceGenerator{

	public:
		ForceGenerator(Vector3 p1 = { 0.0f,0.0f,0.0f }, Vector3 p2 = { 0.0f,0.0f,0.0f }, float r = 0);
		~ForceGenerator();

		virtual void updateForce(Particle* p, double t) = 0;
	protected:
		
		Bounding* bounding_shape = nullptr;
		bool use_bounding;

};

#endif
