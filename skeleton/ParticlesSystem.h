#ifndef PARTICLESSYSTEM_H
#define PARTICLESSYSTEM_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particle.h"
#include <list>
#include <random>
using namespace std;
class ParticlesSystem {

public:
	ParticlesSystem();
	ParticlesSystem(Vector4 color, Vector3 ini_pos, Vector3 mean_vel,int num, float size, float prob, float gravity);
	~ParticlesSystem();

	void set_u_Distribution(bool dist) noexcept { use_uni_distrib = dist; }
	void setNormalDistribVel(double mean, double desv) noexcept {
		mean_v = mean;
		desv_v = desv;
	}
	void setNormalDistribPos(double mean, double desv) noexcept {
		mean_pos = mean;
		desv_pos = desv;
	}
	void setNormalDistribLifeTime(double mean, double desv) noexcept {
		mean_life_time = mean;
		desv_life_time = desv;
	}
	void setUniformDistribVel(double min,double max) noexcept {
		random_vel_max = max;
		random_vel_min = min;

	}
	void setUniformDistribPos(double min, double max)noexcept {
		random_pos_max = max;
		random_pos_min = min;
	}
	void update(double t);

protected:
	list<Particle*> particles;				//coleccion de particulas
	Vector4 color;
	float size;
	float gravity;
	Vector3 ini_pos;						//posicion inicial del sistema
	Vector3 mean_vel;						//velocidad media para las particulas
	int num_generator;						//num de particulas generadas por frame
	float generation_prob;					//probabilidad de generacion de particulas
	std::random_device rd;
	std::mt19937 mt;						//genera numeros aleatorios
	bool use_uni_distrib;					//saber si usar distribucion uniforme 
	double mean_v, desv_v,
		mean_pos, desv_pos,
		mean_life_time, desv_life_time,
		random_pos_max, random_pos_min,
		random_vel_max, random_vel_min;

	std::uniform_real_distribution<double> distrib; //distribucion uniforme para la probabilidad de generacion de particulas

	void addParticles(int num); //metodo para agregar particulas nuevas
};

#endif
