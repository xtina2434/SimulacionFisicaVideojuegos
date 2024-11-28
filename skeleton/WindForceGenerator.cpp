#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(const Vector3& v, float _k1, float k2 , Vector3 c, float r) :
	wind_vel(v), k1(_k1), center(c), radius(r)
{
	sphere = new RenderItem(CreateShape(physx::PxSphereGeometry(radius)),
		new physx::PxTransform(center),Vector4(0.6, 0.6, 0.6, 0.5));
}
//WindForceGenerator::WindForceGenerator(float d, float coef, const Vector3& v,  Vector3 c, float r) :
//	air_density(d), drag_coeff(coef), wind_vel(v), sphereCenter(c), sphereRadius(r)
//{
//	sphere = new RenderItem(CreateShape(physx::PxSphereGeometry(sphereRadius)),
//		new physx::PxTransform(sphereCenter), Vector4(0.6, 0.6, 0.6, 0.5));
//}
WindForceGenerator::~WindForceGenerator() {
	if (sphere) DeregisterRenderItem(sphere);
}
void
WindForceGenerator::updateForce(Particle* p,double t) {

	//evitar division entre 0
	if (p != nullptr && p->getMass() > 0.0f) {
		//calcular distancia particula dentro de la esfera
		Vector3 particlePos = p->getPos();
		float distance = (particlePos - center).magnitude();

		//aplicar la fuerza del viento solo si la particula se encuentra dentro de la esfera
		if (distance <= radius) {
			//Diferencia de velocidad entre el viento y la particula
			Vector3 vel_diff = wind_vel - p->getVel();

			//Fuerza del viento
			Vector3 wind_force = k1 * vel_diff;

			//Si k2 != 0, agregar el factor turbulento
			if (k2 != 0.0f) {
				wind_force += k2 * vel_diff.magnitude() * vel_diff;
			}


			////area efectiva de la particula (area esfera : A = pi * r^2)
			//float particle_radius = p->getSize();
			//float area_effec = physx::PxPi * particle_radius * particle_radius;

			////magnitud diferencia velocidades (velocidad relativa entre viento y particula)
			//float drag_force_magnitude = area_effec * drag_coeff * air_density * vel_diff.magnitudeSquared();
			//
			////la direccion de la fuerza de arrastre es la misma que la de la diferencia de velocidades, por eso se normaliza
			//Vector3 wind_force = drag_force_magnitude * vel_diff.getNormalized();
			//aplicar la fuerza a la particula
			p->addForce(wind_force);
		}
	}
	
}

