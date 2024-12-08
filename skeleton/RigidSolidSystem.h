#ifndef RIGIDSOLIDSYSTEM_H
#define RIGIDSOLIDSYSTEM_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include "RigidSolid.h"
#include <list>
#include <random>
#include <string>
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
constexpr int MAX_SOLIDS = 200;
using namespace std;
class RigidSolidSystem {

public:
	RigidSolidSystem();
	RigidSolidSystem(PxPhysics* _gPhysics, PxScene* _scene,
		Vector3 ini_pos, Vector3 mean_vel, Vector3 mean_vel_ang, Vector3 size, Vector4 color, 
		int num, float prob, float density, std::string shape);
	~RigidSolidSystem();

	//si dist es true se usara la distribucion uniforme para la posicion y velocidad
	//si es false se usara la distribucion normal
	void set_u_Distribution(bool dist) noexcept { use_uni_distrib = dist; }

	void setNormalDistribLinealVel(double mean, double desv) noexcept {
		mean_v = mean;
		desv_v = desv;
	}
	void setNormalDistribAngularVel(double mean, double desv) noexcept {
		mean_w = mean;
		desv_w = desv;
	}
	void setNormalDistribPos(double mean, double desv) noexcept {
		mean_pos = mean;
		desv_pos = desv;
	}
	void setNormalDistribLifeTime(double mean, double desv) noexcept {

		if (desv < 0) desv = 0;
		mean_life_time = mean;
		desv_life_time = desv;
	}
	void setUniformDistribLinearVel(double min, double max) noexcept {
		random_vel_lineal_max = max;
		random_vel_lineal_min = min;

	}
	void setUniformDistribAngularVel(double min, double max) noexcept {
		random_vel_angular_max = max;
		random_vel_angular_min = min;

	}
	void setUniformDistribPos(double min, double max)noexcept {
		random_pos_max = max;
		random_pos_min = min;
	}
	void update(double t);
	void setMaterial(float fricc_st, float fricc_dyn, float elast);
	void setGravityForce() noexcept
	{
		has_gravityForce = true;
	}
	void setWindForce(Vector3& vel, float k1);

	void setWhirlWindForce(float k, float k1);
	void addForceGenerator(ForceGenerator* fg);
protected:
	list<RigidSolid*> solids;				//coleccion de solidos rigidos
	PxPhysics* gPhysics = nullptr;
	PxScene* scene = nullptr;
	PxMaterial* material = nullptr;
	//propiedades de cada solido
	Vector3			ini_pos;
	Vector3			mean_lineal_vel;
	Vector3			mean_angular_vel;
	Vector3			size;
	Vector4			color;
	std::string		shape;
	int				num_generator;			//num de solidos generadas por frame
	float density;
	float			 generation_prob;		//probabilidad de generacion de solidos
	std::random_device rd;
	std::mt19937 mt;						//genera numeros aleatorios
	bool use_uni_distrib;					//saber si usar distribucion uniforme 
	double mean_v, desv_v,					//medias, desviaciones, minimos o maximos para las distribuciones uniforme o normal
		mean_w, desv_w,
		mean_pos, desv_pos,
		mean_life_time, desv_life_time,
		random_pos_max, random_pos_min,
		random_vel_lineal_max, random_vel_lineal_min,
		random_vel_angular_max, random_vel_angular_min;

	std::uniform_real_distribution<double> distrib; //distribucion uniforme para la probabilidad de generacion de solidos

	void addSolids(int num); //metodo para agregar solidos nuevos


	GravityForceGenerator* gravity_generator = nullptr;
	bool has_gravityForce = false;

	WindForceGenerator* wind_generator = nullptr;
	bool has_windForce = false;

	WhirlwindForceGenerator* whirlwind_generator = nullptr;
	bool has_whirlwindForce = false;
};

#endif
