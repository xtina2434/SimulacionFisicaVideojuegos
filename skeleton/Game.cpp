#include "Game.h"

Game::Game(PxPhysics* _gPhysics, PxMaterial* _gMaterial, PxScene* _gScene) :
	gPhysics(_gPhysics), gMaterial(_gMaterial), gScene(_gScene), mt(rd())
{
	current = START;
	elapsedTime = 0;


	titleScene();
}

Game::~Game()
{
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	systems.clear();
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	particles.clear();
}

void Game::update(double t)
{
	elapsedTime += t;
	if (current == SNOW && elapsedTime >= SPAWN_DIANA) {
		setDiana();
		elapsedTime = 0;
	}
	if (!can_shoot) {
		cooldownTime += t;
		if (cooldownTime >= COOLDOWN) {
			can_shoot = true;
		}
	}
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			(*it)->update(t);
		}
	}

	for (auto it = particles.begin(); it != particles.end();) {
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
	for (auto it = rigid_solids.begin(); it != rigid_solids.end();) {
		(*it)->integrate(t);
		(*it)->update(t);
		
		if (!(*it)->isAlive()) {
			delete (*it);
			it = rigid_solids.erase(it);
		}
		else {
			++it;
		}
	}
}
void Game::titleScene()
{
	title_text = "CHAOS RUN";
	next_text = "Pulse c para continuar";
}
void Game::introScene()
{
	intro_text = "Enhorabuena eres el/la finalista de CHAOS RUN. Seras capaz de superar la prueba final para ganar los 6 creditos";
	intro_text2 = "Para determinar tus vidas para la prueba final tienes que acertar el mayor numero de dianas. Por cada 10 puntos conseguiras una vida.";
}
void Game::snowScene() {

	points_text = "Puntos: " + to_string(points);

	//posicion inicial de la camara es 50 50 50
	Camera* cam = GetCamera();
	if (cam != nullptr) {
		cam->setTransform(PxTransform({ 100, 54, 2 }));
	}
	RigidStatic* suelo = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 1.0, 1.0, 1.0), "BOX");
	rigid_statics.push_back(suelo);

	ParticlesSystem* snow_system = new ParticlesSystem(Vector4(1.0, 1.0, 1.0, 1.0), Vector3(0.0, 80.0, 0), Vector3(0.0, -2.0, 0.0), 1, 1.0f, 0.5f, 0.0f, 1.0f);

	snow_system->set_u_Distribution(true);

	snow_system->setUniformDistribPos(-100.0, 100.0);
	snow_system->setUniformDistribVel(1.0, 3.0);
	snow_system->setNormalDistribLifeTime(5.0, 3.0);
	snow_system->setGravityForce();
	systems.push_back(snow_system);
}
void Game::intro2Scene()
{
	lives = points / 10;
	intro_text3 = "Lo has hecho genial, por hacer " + to_string(points) + " puntos tienes " + to_string(lives) + " vidas";
	intro_text4 = "Ahora tienes que superar los 3 niveles finales. Esquiva los obstaculos y evita caer a la lava";
	next_text = "Pulse c para continuar";
}
void Game::clearScene()
{
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	systems.clear();
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	particles.clear();
	for (auto it = rigid_solids.begin(); it != rigid_solids.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_solids.clear();
	for (auto it = rigid_statics.begin(); it != rigid_statics.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_statics.clear();
}
void Game::changeScene()
{
	clearScene();
	switch (current)
	{

	case START:
	{
		title_text = "";
		current = INTRO;
		introScene();
		break;
	}
	case INTRO:
	{
		intro_text = "";
		intro_text2 = "";
		next_text = "";
		current = SNOW;
		snowScene();
		break;
	}
	case SNOW:
	{
		points_text = "";
		if (diana) {
			delete diana;
			diana = nullptr;
		}
		current = INTRO2;
		intro2Scene();
		break;
	}
	default:
		break;
	}
}
void Game::setDiana()
{
	if (cont_dianas < TOTAL_DIANAS) {
		std::uniform_real_distribution<double> pos_z(MIN_Z, MAX_Z);
		std::uniform_real_distribution<double> pos_y(MIN_Y, MAX_Y);

		Vector3 pos = Vector3(-20, pos_y(mt), pos_z(mt));

		//si es la primera se crea
		if (diana == nullptr) {
			diana = new Diana(gPhysics, gScene, pos);
		}
		//las siguientes veces solo cambia de posicion
		else {
			diana->setTransform(pos);
		}
		cont_dianas++;
	}
	else {
		changeScene();
	}
	
}
void Game::handleMouse(int button, int state, int x, int y)
{
	if (can_shoot && current == SNOW && button == 0) {
		Camera* cam = GetCamera();
		if (cam != nullptr) {

			Vector3 pos = cam->getEye();
			Vector3 dir = cam->getMousePos(x,y);

			float speed = 200.0f;

			physx::PxVec3 vel = dir * speed;
			RigidSolid* s = new RigidSolid(
				gPhysics, gScene, gMaterial,
				pos, vel, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 1.0f, "SPHERE");
			rigid_solids.push_back(s);
			can_shoot = false;
			cooldownTime = 0;
		}
	}
}

void Game::keyPress(unsigned char key)
{
	switch (toupper(key))
	{
		
	case 'C':
	{
		changeScene();
		break;
	}
	case 'P':
	{
		Camera* cam = GetCamera();
		if (cam != nullptr) {
			
			PxTransform p = cam->getTransform();

			std::cout << p.p.x << " " << p.p.y << " " << p.p.z << std::endl;
			std::cout << p.q.x << " " << p.q.y << " " << p.q.z << std::endl;
		}
		break;
	}
	default:
		break;
	}

}
void Game::onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	const char* name1 = actor1->getName();
	const char* name2 = actor2->getName();

	
	if ((name1 && strcmp(name1, "solid") == 0 && name2 && strcmp(name2, "Diana") == 0) ||
		(name1 && strcmp(name1, "Diana") == 0 && name2 && strcmp(name2, "solid") == 0) ||
		(name1 && strcmp(name1, "solid") == 0 && name2 && strcmp(name2, "Diana_center") == 0) ||
		(name1 && strcmp(name1, "Diana_center") == 0 && name2 && strcmp(name2, "solid") == 0)) {
		std::cout << "Collision";
		
		if (name1 && strcmp(name1, "solid") == 0) {
			RigidSolid* s1 = static_cast<RigidSolid*>(actor1->userData);
			if (s1) s1->die();
		}
		if (name2 && strcmp(name2, "solid") == 0) {
			RigidSolid* s2 = static_cast<RigidSolid*>(actor2->userData);
			if (s2) s2->die();
		}

		points += DIANA_POINTS;
		points_text = "Puntos: " + to_string(points);
	}
}