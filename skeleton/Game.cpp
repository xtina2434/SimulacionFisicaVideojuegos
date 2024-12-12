#include "Game.h"

Game::Game(PxPhysics* _gPhysics, PxMaterial* _gMaterial, PxScene* _gScene) :
	gPhysics(_gPhysics), gMaterial(_gMaterial), gScene(_gScene)
{
	current = START;
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
}

void Game::update(double t)
{
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			(*it)->update(t);
		}
	}
}

void Game::titleScene()
{
	texts_scene.clear();
	title_text = "CHAOS RUN";
	next_text = "Pulse c para continuar";
	texts_scene.push_back(title_text);
	texts_scene.push_back(next_text);
}

void Game::introScene()
{
	intro_text = "Enhorabuena eres el/la finalista de CHAOS RUN. Seras capaz de superar la prueba final para ganar los 6 creditos";
	intro_text2 = "Para determinar tus vidas para la prueba final tienes que acertar el mayor numero de dianas. Por cada 10 puntos conseguiras una vida.";
	intro_text3 = "ADVERTENCIA: cuidado con la nieve";
}

void Game::snowScene() {
	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
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
		intro_text3 = "";
		next_text = "";
		current = SNOW;
		snowScene();
		break;
	}
	default:
		break;
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
	default:
		break;
	}

}