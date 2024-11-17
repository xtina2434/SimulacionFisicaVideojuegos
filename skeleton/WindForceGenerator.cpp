#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(const Vector3& v, float _k1, float k2 , Vector3 c, float r) :
	wind_vel(v), k1(_k1), sphereCenter(c), sphereRadius(r)
{
	sphere = new RenderItem(CreateShape(physx::PxSphereGeometry(sphereRadius)),
		new physx::PxTransform(sphereCenter),Vector4(0.6, 0.6, 0.6, 0.5));
}
WindForceGenerator::~WindForceGenerator() {
	if (sphere) DeregisterRenderItem(sphere);
}
void
WindForceGenerator::updateForce(Particle* p,double t) {

	//evitar division entre 0
	if (p != nullptr && p->getMass() > 0.0f) {
		//calcular distancia particula dentro de la esfera
		Vector3 particlePos = p->getPos();
		float distance = (particlePos - sphereCenter).magnitude();

		//aplicar la fuerza del viento solo si la particula se encuentra dentro de la esfera
		if (distance <= sphereRadius) {
			//Diferencia de velocidad entre el viento y la particula
			Vector3 vel_diff = wind_vel - p->getVel();

			//Fuerza del viento
			Vector3 wind_force = k1 * vel_diff;

			//Si k2 != 0, agregar el factor turbulento
			if (k2 != 0.0f) {
				wind_force += k2 * vel_diff.magnitude() * vel_diff;
			}

			//aplicar la fuerza a la particula
			p->addForce(wind_force);
		}
	}
	
}

