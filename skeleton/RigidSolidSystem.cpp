#include "RigidSolidSystem.h"

RigidSolidSystem::RigidSolidSystem()
{
}

RigidSolidSystem::RigidSolidSystem(PxPhysics* _gPhysics, PxScene* _scene, 
	Vector3 ini_pos, Vector3 mean_vel, Vector3 mean_vel_ang, Vector3 size, Vector4 color, int num, float prob,float d, std::string SHAPE) :
	ini_pos(ini_pos), mean_lineal_vel(mean_vel), mean_angular_vel(mean_vel_ang), size(size),color(color), num_generator(num), generation_prob(prob),
	distrib(0.0, 1.0), mt(rd()), shape(SHAPE), gPhysics(_gPhysics), scene(_scene), density(d)
{
	use_uni_distrib = false;
	gravity_generator = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));
}

RigidSolidSystem::~RigidSolidSystem()
{
	for (auto s : solids) {
		delete s;
	}
	solids.clear();
}

void RigidSolidSystem::addSolids(int num)
{
	std::normal_distribution<double> normal_dist_time(mean_life_time, desv_life_time);

	for (int i = 0; i < num; i++) {

		if (solids.size() < MAX_SOLIDS) {
			Vector3 lineal_vel;
			Vector3 angular_vel;
			Vector3 pos;

			//generar velocidades y posiciones usando una distribucion uniforme
			if (use_uni_distrib) {
				std::uniform_real_distribution<double> uni_dist_pos(random_pos_min, random_pos_max);
				std::uniform_real_distribution<double> uni_dist_linear_vel(random_vel_lineal_min, random_vel_lineal_max);
				std::uniform_real_distribution<double> uni_dist_angular_vel(random_vel_angular_min, random_vel_angular_max);

				pos = Vector3(uni_dist_pos(mt), uni_dist_pos(mt), uni_dist_pos(mt));
				pos += ini_pos;

				lineal_vel = Vector3(uni_dist_linear_vel(mt), uni_dist_linear_vel(mt), uni_dist_linear_vel(mt));
				angular_vel = Vector3(uni_dist_angular_vel(mt), uni_dist_angular_vel(mt), uni_dist_angular_vel(mt));
			}
			//generar velocidades y posiciones usando una distribucion normal (gaussiana)
			//con las distribuciones normales se crean las particulas con un valor cercano a la media (pos, vel, tiempo)
			//pero algunas mas alejadas del origen/mas menos velocidad/ mas menos tiempo de vida
			else {
				std::normal_distribution<double> normal_dist_pos(mean_pos, desv_pos);
				std::normal_distribution<double> normal_dist_linear_vel(mean_v, desv_v);
				std::normal_distribution<double> normal_dist_angular_vel(mean_w, desv_w);

				pos = Vector3(normal_dist_pos(mt), normal_dist_pos(mt), normal_dist_pos(mt));
				pos += ini_pos;

				//multiplicando se consigue que la velocidad se escale en funcion de la velocidad media
				//vel = Vector3(normal_dist_vel(mt) * mean_vel.x, normal_dist_vel(mt) * mean_vel.y, normal_dist_vel(mt) * mean_vel.z);

				//sumando se consigue que la velocidad este basada en la velocidad media con pequeñas variaciones aleatorias alrededor de la media
				lineal_vel = Vector3(normal_dist_linear_vel(mt) + mean_lineal_vel.x, normal_dist_linear_vel(mt) + mean_lineal_vel.y, normal_dist_linear_vel(mt) + mean_lineal_vel.z);
				angular_vel = Vector3(normal_dist_angular_vel(mt) + mean_angular_vel.x, normal_dist_angular_vel(mt) + mean_angular_vel.y, normal_dist_angular_vel(mt) + mean_angular_vel.z);

			}

			double life_time = normal_dist_time(mt);
			if (life_time < 0)
				life_time = 0;

			//crear solido
			RigidSolid* new_solid = new RigidSolid(gPhysics, scene,material, pos, lineal_vel, angular_vel, size, color, density, life_time, shape, "solid");

			//configurar tensor de inercia personalizado
			float inertia_x = size.y * size.z * distrib(mt) * 0.1f;
			float inertia_y = size.x * size.z * distrib(mt) * 0.1f;
			float inertia_z = size.x * size.y * distrib(mt) * 0.1f;
			Vector3 inertia = Vector3(inertia_x, inertia_y, inertia_z);
			new_solid->setInertia(inertia);

			if (has_gravityForce)
				new_solid->addForceGenerator(gravity_generator);

			if (has_windForce)
				new_solid->addForceGenerator(wind_generator);

			if (has_whirlwindForce)
				new_solid->addForceGenerator(whirlwind_generator);

			solids.push_back(new_solid);
		}
		
	}
}


void RigidSolidSystem::update(double t)
{
	//recorrer la lista de solidos
	for (auto it = solids.begin(); it != solids.end();) {
		//llamar update de cada solid
		(*it)->integrate(t);
		(*it)->update(t);
		//comprobar si siguen vivos, si no es asi se elimina
		if (!(*it)->isAlive()) {
			delete (*it);
			it = solids.erase(it);
		}
		else {
			++it;
		}
	}
	//generar particulas nuevas solo si se cumple la probabilidad
	if (distrib(mt) < generation_prob) {
		addSolids(num_generator);
	}
}

void RigidSolidSystem::setMaterial(float fricc_st, float fricc_dyn, float elast)
{
	//crear material con propiedades aleatorias
	 material = gPhysics->createMaterial(
		distrib(mt) * fricc_st, //friccion estatica: como resiste el movimiento inicial cuando dos superficies estan en contacto
		distrib(mt) * fricc_dyn, //friccion dinamica: como resiste el movimiento continuo mientras las superficies estan deslizandose
		distrib(mt) * elast //elasticidad: que tanto rebota el choque
	);
}

void
RigidSolidSystem::setWindForce(Vector3& vel, float k1) {
	has_windForce = true;
	wind_generator = new WindForceGenerator(vel, k1, 0.0f);
}
void
RigidSolidSystem::setWhirlWindForce(float k, float k1) {
	has_whirlwindForce = true;
	whirlwind_generator = new WhirlwindForceGenerator(Vector3(0.0f, 0.0f, 0.0f), 20.0f, k, k1);
}
void RigidSolidSystem::addForceGenerator(ForceGenerator* fg)
{
	for (auto s : solids) {
		s->addForceGenerator(fg);
	}
}
