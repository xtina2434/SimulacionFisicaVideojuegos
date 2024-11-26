#ifndef SPRINGFORCEGENERATOR_H
#define SPRINGFORCEGENERATOR_H

#include "ForceGenerator.h"
#include "core.hpp"

class SpringForceGenerator : public ForceGenerator {
	public:
		SpringForceGenerator(double k, double resting_length, Particle* other);
		SpringForceGenerator(double k, double resting_length, const Vector3& anchor);
		~SpringForceGenerator();

		void updateForce(Particle* p, double t) override;

		void setK(double _k) noexcept { k = _k; }

		void changeK(double _k) { 
			k += _k; 
			if (k < 0.0) k = 0.0;
		}
	protected:
		double k; //coeficiente de elasticidad
		double resting_length;
		Particle* other;
		bool use_anchor;
		Vector3 anchor;
};


#endif
