#ifndef PARTICLESSYSTEM_H
#define PARTICLESSYSTEM_H

#include "core.hpp"
#include "RenderUtils.hpp"
#include "Particle.h"
#include <list>

using namespace std;
class ParticlesSystem{

public:
	ParticlesSystem();
	~ParticlesSystem();

	//generadores son clases
	void update();
protected:
	list<Particle*> particles;	//coleccion de particulas
	Vector3 originPos;
	Vector3 velMedia;
	int num_generator;
	//direccion
};

#endif

//set origin
/*set mean velocity
getmean vel
setmean duration

setparticle
setNparticles

setProb
getProb

int nparticles
double generation prob
Vector3 origin, meanvel;

mt19937 mt
std::uniform_real_distribution<double> _u{0,1};


distintos generadores



*/
