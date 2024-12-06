#include "ParticlesSystem.h"


ParticlesSystem::ParticlesSystem(Vector4 c, Vector3 p, Vector3 v, int n, float s, float prob, float g,float m) :
	color(c), size(s), ini_pos(p), mean_vel(v), num_generator(n), generation_prob(prob), gravity(g),mass(m), 
	distrib(0.0, 1.0), mt(rd())
{
	use_uni_distrib = false;
	gravity_generator = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));
}
ParticlesSystem::~ParticlesSystem() {
	for (auto p : particles) {
		delete p;
	}
	particles.clear();

	if (gravity_generator != nullptr) {
		delete gravity_generator;
		gravity_generator = nullptr;
	}
	if (wind_generator != nullptr) {
		delete wind_generator;
		wind_generator = nullptr;
	}
	if (whirlwind_generator != nullptr) {
		delete whirlwind_generator;
		whirlwind_generator = nullptr;
	}
}
void 
ParticlesSystem::setWindForce(Vector3& vel, float k1 /*d*/) {
	has_windForce = true;
	//wind_generator = new WindForceGenerator(d,0.5f,vel, Vector3(0.0f, 50.0f, 0.0f), 15.0f);
	wind_generator = new WindForceGenerator(vel, k1, 0.0f);
}
void
ParticlesSystem::setWhirlWindForce(float k, float k1) {
	has_whirlwindForce = true;

	whirlwind_generator = new WhirlwindForceGenerator(Vector3(0.0f,0.0f,0.0f), 20.0f, k, k1);
}
void
ParticlesSystem::addForceGenerator(ForceGenerator* fg) {
	for (auto p : particles) {
		p->addForceGenerator(fg);
	}
}

void
ParticlesSystem::addParticles(int num) {

	std::normal_distribution<double> normal_dist_time(mean_life_time, desv_life_time);

	for (int i = 0; i < num; i++) {

		Vector3 vel;
		Vector3 pos;

		//generar velocidades y posiciones usando una distribucion uniforme
		if (use_uni_distrib) {
			std::uniform_real_distribution<double> uni_dist_pos(random_pos_min, random_pos_max);
			std::uniform_real_distribution<double> uni_dist_vel(random_vel_min, random_vel_max);

			pos = Vector3(uni_dist_pos(mt), uni_dist_pos(mt), uni_dist_pos(mt));
			pos += ini_pos;

			vel = Vector3(uni_dist_vel(mt), uni_dist_vel(mt), uni_dist_vel(mt));
		}
		//generar velocidades y posiciones usando una distribucion normal (gaussiana)
		//con las distribuciones normales se crean las particulas con un valor cercano a la media (pos, vel, tiempo)
		//pero algunas mas alejadas del origen/mas menos velocidad/ mas menos tiempo de vida
		else {
			std::normal_distribution<double> normal_dist_pos(mean_pos, desv_pos);
			std::normal_distribution<double> normal_dist_vel(mean_v, desv_v);

			pos = Vector3(normal_dist_pos(mt), normal_dist_pos(mt), normal_dist_pos(mt));
			pos += ini_pos;

			//multiplicando se consigue que la velocidad se escale en funcion de la velocidad media
			//vel = Vector3(normal_dist_vel(mt) * mean_vel.x, normal_dist_vel(mt) * mean_vel.y, normal_dist_vel(mt) * mean_vel.z);
				
			//sumando se consigue que la velocidad este basada en la velocidad media con pequeñas variaciones aleatorias alrededor de la media
			vel = Vector3(normal_dist_vel(mt) + mean_vel.x, normal_dist_vel(mt) + mean_vel.y, normal_dist_vel(mt) + mean_vel.z);
				
		}
			
		double life_time = normal_dist_time(mt);

		if (life_time < 0)
			life_time = 0;
		//crear particula con propiedades aleatorias
		Particle* new_particle = new Particle( pos, vel, size, Vector3(0.0, 0.0, 0.0), color, life_time, mass, "SPHERE");
		new_particle->setAcel(Vector3(0.0f, gravity, 0.0f));

		if (has_gravityForce) 
			new_particle->addForceGenerator(gravity_generator);

		if (has_windForce)
			new_particle->addForceGenerator(wind_generator);

		if (has_whirlwindForce)
			new_particle->addForceGenerator(whirlwind_generator);

		particles.push_back(new_particle);
	}
}
void
ParticlesSystem::update(double t) {

	//recorrer la lista de particulas
	for (auto it = particles.begin(); it != particles.end();) {
		//llamar update de cada particula
		(*it)->integrate(t);
		(*it)->update(t);
		//comprobar si siguen vivas, si no es asi se elimina
		if (!(*it)->isAlive()) {
			delete (*it);
			it = particles.erase(it);
		}
		else {
			++it;
		}
	}
	//generar particulas nuevas solo si se cumple la probabilidad
	if (distrib(mt) < generation_prob) {
		addParticles(num_generator);
	}
}