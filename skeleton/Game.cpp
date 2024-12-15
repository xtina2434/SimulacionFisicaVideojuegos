#include "Game.h"

Game::Game(PxPhysics* _gPhysics, PxMaterial* _gMaterial, PxScene* _gScene) :
	gPhysics(_gPhysics), gMaterial(_gMaterial), gScene(_gScene), mt(rd())
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
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	particles.clear();
}

void Game::update(double t)
{
	
	if (current == SNOW) {
		elapsedTime += t;
		if (elapsedTime >= SPAWN_DIANA) {
			setDiana();
			elapsedTime = 0;
		}
	}
	if (!can_shoot) {
		cooldownTime += t;
		if (cooldownTime >= COOLDOWN) {
			can_shoot = true;
		}
	}
	if (dying) {
		respawnTime += t;
		if (respawnTime >= RESPAWN_TIME) {
			dying = false;
			changeScene();
		}

	}
	if (wait_restart) {
		respawnTime += t;
		if (respawnTime >= RESPAWN_TIME) {
			wait_restart = false;
			changeScene();
		}

	}
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			(*it)->update(t);
		}
	}
	for (auto it = rigid_systems.begin(); it != rigid_systems.end(); ++it) {
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
	intro_text = "Enhorabuena eres el/la finalista de CHAOS RUN. Ahora tienes que ser capaz de superar la prueba final para ganar los 6 creditos";
	intro_text2 = "Para determinar tus vidas para la prueba final tienes que acertar el mayor numero de dianas. Por cada 10 puntos conseguiras una vida.";
}
void Game::snowScene() {

	points_text = "Puntos: " + to_string(points);
	cont_dianas = 0;
	elapsedTime = 0;
	if (cam != nullptr) {
		cam->setTransform(PxTransform({ 100, 54, 2 }), PxVec3(-1, 0, 0));
	}
	
	RigidStatic* suelo = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 1.0, 1.0, 1.0), "BOX", "suelo");
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
	if (lives == 0) lives = 1;
	next_text = "Pulse c para continuar";
	intro_text3 = "Lo has hecho genial, por hacer " + to_string(points) + " puntos tienes " + to_string(lives) + " vidas.";
	intro_text4 = "Ahora tienes que superar los 3 niveles finales. Esquiva los obstaculos y evita caer a la lava.";
	input_text = "Con 'W' saltas, con 'D' avanzas y con 'A' retrocedes.";
}
void Game::level1()
{

	lives_text = "Vidas: " + to_string(lives);

	if (cam != nullptr) {
		cam->resetInitialTransform();
	}

	RigidStatic* lava = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 0.1, 0.0, 1.0), "BOX", "lava");
	rigid_statics.push_back(lava);

	createPlayer();
	RigidStatic* plataforma = new RigidStatic(gPhysics, gScene, Vector3(0, 30, 0), Vector3(100, 10, 4), Vector4(0.0, 0.6, 0.8, 1), "BOX", "plataforma");
	rigid_statics.push_back(plataforma);
	RigidStatic* plataforma2 = new RigidStatic(gPhysics, gScene, Vector3(0, 30, -50), Vector3(100, 10, 4), Vector4(0.0, 0.0, 0.0, 1), "BOX", "plataforma");
	rigid_statics.push_back(plataforma2);
	RigidStatic* flag = new RigidStatic(gPhysics, gScene, Vector3(40, 40, -0), Vector3(3, 0.1, 3), Vector4(0.0, 1.0, 0.0, 1), "BOX", "flag");
	rigid_statics.push_back(flag);

	RigidStatic* canion1 = new RigidStatic(gPhysics, gScene, Vector3(-60, 43, -50), Vector3(3, 3, 3), Vector4(0.4, 0.4, 0.4, 1), "BOX", "objeto");
	rigid_statics.push_back(canion1);
	RigidStatic* canion2 = new RigidStatic(gPhysics, gScene, Vector3(-30, 43, -50), Vector3(3, 3, 3), Vector4(0.4, 0.4, 0.4, 1), "BOX", "objeto");
	rigid_statics.push_back(canion2);
	RigidStatic* canion3 = new RigidStatic(gPhysics, gScene, Vector3(0, 43, -50), Vector3(3, 3, 3), Vector4(0.4, 0.4, 0.4, 1), "BOX", "objeto");
	rigid_statics.push_back(canion3);

	RigidSolidSystem* s1 = new RigidSolidSystem(gPhysics, gScene, Vector3(-60, 42, -45), Vector3(0, 0, 50), Vector3(0, 0, 0),
		Vector3(1, 1, 1), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 1, 0.01, 5.0f, "SPHERE");
	s1->quitGravity();
	s1->set_u_Distribution(false);
	s1->setNormalDistribPos(0.01, 0.01);
	s1->setNormalDistribAngularVel(0.01, 0.01);
	s1->setNormalDistribLinealVel(0.01, 0.01);
	s1->setNormalDistribLifeTime(2.0, 0.1);
	rigid_systems.push_back(s1);

	RigidSolidSystem* s2 = new RigidSolidSystem(gPhysics, gScene, Vector3(-30, 42, -45), Vector3(0, 0, 75), Vector3(0, 0, 0),
		Vector3(1, 1, 1), Vector4(0.0f, 1.0f, 0.0f, 1.0f), 1, 0.01, 5.0f, "SPHERE");
	s2->quitGravity();
	s2->set_u_Distribution(false);
	s2->setNormalDistribPos(0.01, 0.01);
	s2->setNormalDistribAngularVel(0.01, 0.01);
	s2->setNormalDistribLinealVel(0.01, 0.01);
	s2->setNormalDistribLifeTime(2.0, 0.1);
	rigid_systems.push_back(s2);

	RigidSolidSystem* s3 = new RigidSolidSystem(gPhysics, gScene, Vector3(0, 42, -45), Vector3(0, 0, 100), Vector3(0, 0, 0),
		Vector3(1, 1, 1), Vector4(0.0f, 0.0f, 1.0f, 1.0f), 1, 0.01, 5.0f, "SPHERE");
	s3->quitGravity();
	s3->set_u_Distribution(false);
	s3->setNormalDistribPos(0.01, 0.01);
	s3->setNormalDistribAngularVel(0.01, 0.01);
	s3->setNormalDistribLinealVel(0.01, 0.01);
	s3->setNormalDistribLifeTime(2.0, 0.1);
	rigid_systems.push_back(s3);
}
void Game::level2()
{
	lives_text = "Vidas: " + to_string(lives);

	RigidStatic* lava = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 0.1, 0.0, 1.0), "BOX", "lava");
	rigid_statics.push_back(lava);

	createPlayer();

	RigidStatic* plataforma = new RigidStatic(gPhysics, gScene, Vector3(-80, 28, 0), Vector3(10, 5, 5), Vector4(1.0, 0.0, 0.0, 1), "SPHERE", "plataforma");
	rigid_statics.push_back(plataforma);
	RigidStatic* plataforma2 = new RigidStatic(gPhysics, gScene, Vector3(-30, 28, 0), Vector3(10, 5, 5), Vector4(1.0, 0.0, 0.0, 1), "SPHERE", "plataforma");
	rigid_statics.push_back(plataforma2);
	RigidStatic* plataforma3 = new RigidStatic(gPhysics, gScene, Vector3(20, 28, 0), Vector3(10, 5, 5), Vector4(1.0, 0.0, 0.0, 1), "SPHERE", "plataforma");
	rigid_statics.push_back(plataforma3);
	RigidStatic* flag = new RigidStatic(gPhysics, gScene, Vector3(20, 38, -0), Vector3(4, 0.1, 4), Vector4(0.0, 1.0, 0.0, 1), "BOX", "flag"); 
	rigid_statics.push_back(flag); 


	RigidSolidSystem* whirlwind_solid_system = new RigidSolidSystem(gPhysics, gScene, 
		Vector3(-50, 30, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0.5, 1, 1), Vector4(1, 1, 0, 1), 
		1, 0.1f, 5.0f, "SPHERE"); 
	whirlwind_solid_system->set_u_Distribution(false); 
	whirlwind_solid_system->quitGravity(); 
	whirlwind_solid_system->setNormalDistribPos(3.0, 1.0); 
	whirlwind_solid_system->setNormalDistribLinealVel(5.0, 3.0); 
	whirlwind_solid_system->setNormalDistribAngularVel(0.0, 0.01); 
	whirlwind_solid_system->setNormalDistribLifeTime(3.0, 2.0); 
	whirlwind_solid_system->setWhirlWindForce(10.0f, 0.5f, 1.0f); 
	rigid_systems.push_back(whirlwind_solid_system); 


	RigidSolidSystem* whirlwind_solid_system2 = new RigidSolidSystem(gPhysics, gScene,
		Vector3(0, 30, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5), Vector4(0, 0, 1, 1),
		1, 0.1f, 5.0f, "BOX");
	whirlwind_solid_system2->set_u_Distribution(false);
	whirlwind_solid_system2->quitGravity();
	whirlwind_solid_system2->setNormalDistribPos(3.0, 1.0);
	whirlwind_solid_system2->setNormalDistribLinealVel(8.0, 3.0);
	whirlwind_solid_system2->setNormalDistribAngularVel(0.0, 0.01);
	whirlwind_solid_system2->setNormalDistribLifeTime(3.0, 2.0);
	whirlwind_solid_system2->setWhirlWindForce(20.0f, 0.5f, 1.0f);
	rigid_systems.push_back(whirlwind_solid_system2);
}
void Game::level3()
{
	lives_text = "Vidas: " + to_string(lives);

	RigidStatic* lava = new RigidStatic(gPhysics, gScene, Vector3(0, 0, 0), Vector3(200, 0.1, 200), Vector4(1.0, 0.1, 0.0, 1.0), "BOX", "lava");
	rigid_statics.push_back(lava);
	RigidStatic* plataforma = new RigidStatic(gPhysics, gScene, Vector3(0, 30, 0), Vector3(100, 10, 6), Vector4(0.0, 0.6, 0.8, 1), "BOX", "plataforma");
	rigid_statics.push_back(plataforma);
	RenderItem* pared = new RenderItem(CreateShape(physx::PxBoxGeometry(200, 50, 0.1)),
		new physx::PxTransform(0, 30, -12), Vector4(0.0, 0.0, 0.0, 1));
	items.push_back(pared);
	RigidStatic* flag = new RigidStatic(gPhysics, gScene, Vector3(40, 40, -0), Vector3(3, 0.1, 3), Vector4(0.0, 1.0, 0.0, 1), "BOX", "flag");
	rigid_statics.push_back(flag);
	createPlayer();

	createDock(Vector3(-64.0, 44.0, -10.0), 300.0, Vector4(1.0, 0.0, 0.0, 1));
	createDock(Vector3(-44.0, 52.0, -10.0), 350.0, Vector4(1.0, 0.5, 0.0, 1));
	createDock(Vector3(-34.0, 44.0, -10.0), 400.0, Vector4(0.0, 1.0, 0.0, 1));
	createDock(Vector3(-14.0, 52.0, -10.0), 450.0, Vector4(0.0, 1.0, 0.5, 1));
	createDock(Vector3(4.0, 44.0, -10.0), 500.0, Vector4(0.0, 0.0, 1.0, 1));
}
void Game::createDock(const Vector3& pos, double k, const Vector4& color)
{
	//const Vector3 anchorPoint(0.0, 30.0, 0.0);
	RenderItem* cube = new RenderItem(CreateShape(physx::PxBoxGeometry(2, 2, 2)),
		new physx::PxTransform(pos), color);
	items.push_back(cube);

	Vector3 pos2 = pos;
	pos2.z += 10;
	RigidSolid* s = new RigidSolid(gPhysics, gScene, gMaterial,
		pos2, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), color,
		1.0, 500, "SPHERE", "muelle");

	rigid_solids.push_back(s);
	SpringForceGenerator* spring_generator = new SpringForceGenerator(k, 1.0, pos);

	s->addForceGenerator(spring_generator);
	//s->setInertia(Vector3(5, 5, 5));
	s->getSolid()->setRigidDynamicLockFlags(
		PxRigidDynamicLockFlag::eLOCK_LINEAR_X |
		PxRigidDynamicLockFlag::eLOCK_LINEAR_Y
	);
}
void Game::respawn()
{
	respawn_text = "Eso ha quemado. No te preocupes, aun te queda(n) " + to_string(lives) + " vida(s).";

	wait_restart = true;
	respawnTime = 0;
}
void Game::lost()
{
	lost_text = "Has perdido todas tus vidas :( No te rindas, seguro que completas la carrera en otra convocatoria.";
	next_text = "Pulse c para repetir";
	exit_text = "Pulse q para salir";
}
void Game::win()
{
	win_text = "Enhorabuena, has completado CHAOS RUN con exito.";
	exit_text = "Pulse q para salir";
}
void Game::clearScene()
{
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	systems.clear();
	for (auto it = rigid_systems.begin(); it != rigid_systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	rigid_systems.clear();
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
	for (auto it = items.begin(); it != items.end(); ++it) {
		if (*it != nullptr) {
			DeregisterRenderItem(*it);
		}
	}
	items.clear();
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
	case INTRO2:
	{
		next_text = "";
		intro_text3 = "";
		intro_text4 = "";
		input_text = "";
		current = LEVEL1;
		level1();
		break;
	}
	case LEVEL1:
	{
		lives_text = "";
		if (lives <= 0) {
			current = LOST;
			lost();
		}
		else if (next_level) {
			next_level = false;
			current = LEVEL2;
			level2();
		}
		else {
			last_scene = LEVEL1;
			current = RESPAWN;
			respawn();
		}
		break;
	}
	case LEVEL2:
	{
		lives_text = "";
		if (lives <= 0) {
			current = LOST;
			lost();
		}
		else if (next_level) {
			next_level = false;
			current = LEVEL3;
			level3();
		}
		else {
			last_scene = LEVEL2;
			current = RESPAWN;
			respawn();
		}
		break;
	}
	case LEVEL3:
	{
		lives_text = "";
		if (lives <= 0) {
			current = LOST;
			lost();
		}
		else if (next_level) {
			next_level = false;
			current = WIN;
			win();
		}
		else {
			last_scene = LEVEL3;
			current = RESPAWN;
			respawn();
		}
		break;
	}
	case RESPAWN:
	{
		respawn_text = "";
		current = last_scene;
		if (current == LEVEL1) {
			level1();
		}
		if (current == LEVEL2) {
			level2();
		}
		if (current == LEVEL3) {
			level3();
		}
		break;
	}
	case LOST:
	{
		lost_text = "";
		exit_text = "";
		current = START;
		titleScene();
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
		
		elapsedTime = 0;
		changeScene();
	}
	
}
void Game::createPlayer()
{
	player = new RigidSolid(gPhysics, gScene, gMaterial,
		Vector3(-80, 45, 0), Vector3(0.0, -1.0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), Vector4(1.0, 0.0, 1.0, 1),
		2.0, 500, "BOX", "player");

	player->setMaterialProperties(0.0f, 0.6f, 0.5f);
	
	rigid_solids.push_back(player);
}

void Game::handleMouse(int button, int state, int x, int y)
{
	if (can_shoot && current == SNOW && button == 0) {
		
		if (cam != nullptr) {

			Vector3 pos = cam->getEye();
			Vector3 dir = cam->getMousePos(x,y);

			float speed = 200.0f;

			physx::PxVec3 vel = dir * speed;
			RigidSolid* s = new RigidSolid(
				gPhysics, gScene, gMaterial,
				pos, vel, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 1.0f, "SPHERE", "laser");
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
		if (current != SNOW && current != LEVEL1 && current != LEVEL2 && current != LEVEL3) {
			changeScene();
		}
		
		break;
	}
	case 'D':
	{
		if (player && !dying) {
			Vector3 pos = player->getPosition();
			pos.x += 1.0f;
			player->setPosition(pos);
		}
		break;
	}
	case 'A':
	{
		if (player && !dying) {
			Vector3 pos = player->getPosition();
			pos.x -= 1.0f;
			player->setPosition(pos);
		}
		break;
	}
	case 'W':
	{
		if (player && canJump && !dying) {
			Vector3 vel = player->getLinealVel();
			vel.y = 14.0f;
			if (GetAsyncKeyState('D')) {
				vel.x += 5.0f;
			}
			else if (GetAsyncKeyState('A')) {
				vel.x -= 5.0f;
			}
			player->setLinearVel(vel);
			canJump = false;
		}
		break;
	}
	case 'P':
	{
		
		if (cam != nullptr) {
			
			PxTransform p = cam->getTransform();
			PxVec3 dir = cam->getDir();
			std::cout << p.p.x << " " << p.p.y << " " << p.p.z << std::endl;
			std::cout << p.q.x << " " << p.q.y << " " << p.q.z << std::endl;
			std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
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

	
	if ((name1 && strcmp(name1, "laser") == 0 && name2 && strcmp(name2, "Diana") == 0) ||
		(name1 && strcmp(name1, "Diana") == 0 && name2 && strcmp(name2, "laser") == 0) ||
		(name1 && strcmp(name1, "laser") == 0 && name2 && strcmp(name2, "Diana_center") == 0) ||
		(name1 && strcmp(name1, "Diana_center") == 0 && name2 && strcmp(name2, "laser") == 0)) {

		if (name1 && strcmp(name1, "laser") == 0) {
			RigidSolid* s1 = static_cast<RigidSolid*>(actor1->userData);
			if (s1) s1->die();
		}
		if (name2 && strcmp(name2, "laser") == 0) {
			RigidSolid* s2 = static_cast<RigidSolid*>(actor2->userData);
			if (s2) s2->die();
		}

		points += DIANA_POINTS;
		points_text = "Puntos: " + to_string(points);
	}

	if ((name1 && strcmp(name1, "player") == 0 && name2 && strcmp(name2, "plataforma") == 0) ||
		(name1 && strcmp(name1, "plataforma") == 0 && name2 && strcmp(name2, "player") == 0)) {
		
		canJump = true;
	}

	if (!dying && (name1 && strcmp(name1, "player") == 0 && name2 && strcmp(name2, "lava") == 0) ||
		(name1 && strcmp(name1, "lava") == 0 && name2 && strcmp(name2, "player") == 0)) {
		
		respawnTime = 0;
		dying = true;
		lives--;
		next_level = false;

		ParticlesSystem* fire_system = new ParticlesSystem(Vector4(1.0, 0.5, 0.0, 1.0), player->getPosition(), Vector3(0, 0, 0), 20, 1.5f, 0.8f, 0.0f, 0.0f);

		fire_system->set_u_Distribution(false);
		fire_system->setNormalDistribPos(1.0, 0.5);
		fire_system->setNormalDistribVel(1.0, 0.5);
		fire_system->setNormalDistribLifeTime(1.0, 0.5);

		systems.push_back(fire_system);
		
		player->setInvisible();
		player->die();
	}

	if ( !next_level && (name1 && strcmp(name1, "player") == 0 && name2 && strcmp(name2, "flag") == 0) ||
		(name1 && strcmp(name1, "flag") == 0 && name2 && strcmp(name2, "player") == 0)) {

		std::cout << "ole";
		respawnTime = 0;
		next_level = true;
		wait_restart = true;
	}
}