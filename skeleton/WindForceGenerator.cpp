#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(Vector3& v, float _k1, float _k2) :
	wind_vel(v) , k1(_k1), k2(_k2)

{
}

//WindForceGenerator::WindForceGenerator(const Vector3& v, float _k1, float k2 , Vector3 c, float r) :
//	wind_vel(v), k1(_k1), center(c), radius(r)
//{
//	sphere = new RenderItem(CreateShape(physx::PxSphereGeometry(radius)),
//		new physx::PxTransform(center),Vector4(0.6, 0.6, 0.6, 0.5));
//}
//WindForceGenerator::WindForceGenerator(float d, float coef, const Vector3& v,  Vector3 c, float r) :
//	air_density(d), drag_coeff(coef), wind_vel(v), sphereCenter(c), sphereRadius(r)
//{
//	sphere = new RenderItem(CreateShape(physx::PxSphereGeometry(sphereRadius)),
//		new physx::PxTransform(sphereCenter), Vector4(0.6, 0.6, 0.6, 0.5));
//}
WindForceGenerator::~WindForceGenerator() {
	
}
void
WindForceGenerator::updateForce(Particle* p,double t) {

	//evitar division entre 0
	if (p != nullptr && p->getMass() > 0.0f) {
		
		Vector3 vel_diff = wind_vel - p->getVel();
		Vector3 wind_force = k1 * vel_diff;
		if (k2 > 0.0f) {
			wind_force += k2 * vel_diff.magnitude() * vel_diff;
		}
		p->addForce(wind_force);
	}
	/*//evitar division entre 0
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
	*/
	
}

void WindForceGenerator::updateForce(RigidSolid* s, double t)
{
	if (s != nullptr && s->getMass() > 0.0f) {

		Vector3 vel_diff = wind_vel - s->getLinealVel();
		Vector3 wind_force = k1 * vel_diff;
		if (k2 > 0.0f) {
			wind_force += k2 * vel_diff.magnitude() * vel_diff;
		}
		s->addForce(wind_force);

		//aplicar torque por si el viento genera una fuerza fuera del centro de masa
		Vector3 offset = Vector3(0, 0, 0); //punto de aplicacion de la fuerza
		if (offset != Vector3(0, 0, 0)) {
			Vector3 torque = offset.cross(wind_force); //torque = offset x force
			s->addTorque(torque);
		}
	}
}

