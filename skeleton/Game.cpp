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

	//posicion inicial de la camara es 50 50 50
	Camera* cam = GetCamera();
	if (cam != nullptr) {
		cam->setTransform(PxTransform({ 100, 54, 2 }));
	}
	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(200, 0.1, 200));
	suelo->attachShape(*shape);
	gScene->addActor(*suelo); 
	RenderItem* item = new RenderItem(shape, suelo, { 1.0,1.0,1.0,1 });

	ParticlesSystem* snow_system = new ParticlesSystem(Vector4(1.0, 1.0, 1.0, 1.0), Vector3(0.0, 80.0, 0), Vector3(0.0, -2.0, 0.0), 1, 1.0f, 0.5f, 0.0f, 1.0f);

	snow_system->set_u_Distribution(true);

	snow_system->setUniformDistribPos(-100.0, 100.0);
	snow_system->setUniformDistribVel(1.0, 3.0);
	snow_system->setNormalDistribLifeTime(5.0, 3.0);
	snow_system->setGravityForce();
	systems.push_back(snow_system);
}
void Game::clearScene()
{

	/*for (int i = 0; i < texts_scene.size(); i++) {
		texts_scene[i] = "";
	}*/

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
	default:
		break;
	}
}
void Game::setDiana()
{
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

			Particle* p = new Particle
			(pos, vel, 1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 1.0f, "SPHERE");
			particles.push_back(p);

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