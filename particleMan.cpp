#include "particleMan.h"

std::vector<std::vector<bool>> ParticleMan::particleGrid;
std::vector<ParticleMan::particleObject> ParticleMan::particleTypes;
std::vector<std::shared_ptr <Engine::engineObject>> ParticleMan::activeParticles;
std::shared_ptr <ParticleMan::brushObject> brush;


void emptyParticle(std::shared_ptr<Engine::engineObject> obj) {
	auto particleObj = std::dynamic_pointer_cast<ParticleMan::particleObject>(obj);
}

void dirtParticle(std::shared_ptr<Engine::engineObject> obj) {
	auto particleObj = std::dynamic_pointer_cast<ParticleMan::particleObject>(obj);

	std::vector<SDL_FPoint> checkDirections; //Create Empty List of Directions
	for (int i = 0; i < checkDirections.size(); i++) { //For Each Item in Direction List
		if (particleObj->tryMove(checkDirections[i]))
			return;
	}
}

void sandParticle(std::shared_ptr<Engine::engineObject> obj) {
	auto particleObj = std::dynamic_pointer_cast<ParticleMan::particleObject>(obj);

	std::vector<SDL_FPoint> checkDirections; //Create Empty List of Directions
		checkDirections.push_back({ 0, 1 }); //Add Down to Directions List
	if (rand() % 2) {
		checkDirections.push_back({ 1, 1 }); //Add Down Left
		checkDirections.push_back({ -1, 1 }); //Add Down Right
	}
	for (int i = 0; i < checkDirections.size(); i++) { //For Each Item in Direction List
		if (particleObj->tryMove(checkDirections[i])) // If Direction Checked is Usable
			return; //Actually Move Particle
	}
}

void waterParticle(std::shared_ptr<Engine::engineObject> obj) {
	auto particleObj = std::dynamic_pointer_cast<ParticleMan::particleObject>(obj);

	std::vector<SDL_FPoint> checkDirections; //Create Empty List of Directions
	checkDirections.push_back({ 0, 1 }); //Add Down to Directions List
	if (rand() % 2) {
		checkDirections.push_back({ 1, 1 }); //Add Down Left
		checkDirections.push_back({ -1, 1 }); //Add Down Right
	}
	if (rand() % 2) {
		checkDirections.push_back({ -1, 0 }); //Add Left
		checkDirections.push_back({ 1, 0 }); //Add Right
	}
	else {
		checkDirections.push_back({ 1, 0 }); //Add Right
		checkDirections.push_back({ -1, 0 }); //Add Left
	}
	for (int i = 0; i < checkDirections.size(); i++) { //For Each Item in Direction List
		if (particleObj->tryMove(checkDirections[i]))
			return;
	}
}

void keepInBounds(std::shared_ptr<Engine::engineObject> obj) {
	if (obj->hull.x < 0)
		obj->hull.x = 0;
	if (obj->hull.y < 0)
		obj->hull.y = 0;
	if (obj->hull.x > Engine::baseRes.x-1)
		obj->hull.x = Engine::baseRes.x-1;
	if (obj->hull.y > Engine::baseRes.y-1)
		obj->hull.y = Engine::baseRes.y-1;
}

void brushUpdate(std::shared_ptr<Engine::engineObject> obj) {
	auto brushObj = std::dynamic_pointer_cast<ParticleMan::brushObject>(obj);
	brushObj->addParticle();
	brushObj->particleSelection();
	brushObj->generateBrushScale();
	brush->tex = ParticleMan::particleTypes[brush->selectedParticle].tex;
	obj->hull.x = Engine::mousePos.x;
	obj->hull.y = Engine::mousePos.y;
}

void ParticleMan::initParticle() {
	/*std::shared_ptr<ParticleMan::particleObject> velObj1 = std::make_shared<ParticleMan::particleObject>();
	particleTypes.push_back(Engine::addObject(velObj));
	particleTypes[1]->hull.x++;*/
	ParticleMan::particleGrid.resize(Engine::baseRes.x);
	for (int i = 0; i < ParticleMan::particleGrid.size(); i++) {
		ParticleMan::particleGrid[i].resize(Engine::baseRes.y);
		std::fill(ParticleMan::particleGrid[i].begin(), ParticleMan::particleGrid[i].end(), false);
	}

	//Adds Particle to Local List
	SDL_Color dirt = { 89, 58, 14, 255 }; //Create Particle Color
	ParticleMan::particleObject dirtPhysics = ParticleMan::particleObject(dirt); //Create Particle as Object
	dirtPhysics.updateFuncs.push_back(dirtParticle); //Define Particle Physics
	dirtPhysics.updateFuncs.push_back(keepInBounds); //Keeps Particle in Window Bounds
	ParticleMan::particleTypes.push_back(dirtPhysics);

	SDL_Color sand = { 235, 235, 180, 255 };
	ParticleMan::particleObject sandPhysics = ParticleMan::particleObject(sand);
	sandPhysics.updateFuncs.push_back(sandParticle);
	sandPhysics.updateFuncs.push_back(keepInBounds);
	ParticleMan::particleTypes.push_back(sandPhysics);

	SDL_Color water = { 0, 255, 240, 217 };
	ParticleMan::particleObject waterPhysics = ParticleMan::particleObject(water);
	waterPhysics.updateFuncs.push_back(waterParticle);
	waterPhysics.updateFuncs.push_back(keepInBounds);
	ParticleMan::particleTypes.push_back(waterPhysics);

	brush = std::make_shared<ParticleMan::brushObject>();
	Engine::addObject(brush);
	brush->updateFuncs.push_back(brushUpdate);
}

/*
bool ParticleMan::particles[192][108];
std::shared_ptr<Engine::engineObject> grid;


void drawGrid(std::shared_ptr<Engine::engineObject> obj) {
	Engine::setRenderTarget(grid->tex);
	Engine::drawRect({ 0, 0, float(Engine::baseRes.x), float(Engine::baseRes.y) }, { 5, 0, 10, 255 });
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			if (ParticleMan::particles[x][y]) {
				Engine::drawRect({ float(x), float(y), 1, 1 }, { 235, 235, 180, 255 });

			}
		}
	}
	Engine::setRenderTarget(NULL);
}

void addSandParticle(std::shared_ptr<Engine::engineObject> obj) {
	bool particlesCopy[192][108];
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			particlesCopy[x][y] = ParticleMan::particles[x][y];

		}
	}
	if (Engine::mouseStates[0] == 2) {
		ParticleMan::particles[int(Engine::mousePos.x)][int(Engine::mousePos.y)] = true;
	}
		for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
			for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
				if (particlesCopy[x][y] && (y + 1 < 108)) {
					if (!particlesCopy[x][y + 1]) {
						ParticleMan::particles[x][y] = false;
						ParticleMan::particles[x][y + 1] = true;
					}
					else if (!particlesCopy[x - 1][y + 1] && (x - 1 > 0)) {
						ParticleMan::particles[x][y] = false;
						ParticleMan::particles[x - 1][y + 1] = true;
					}
					else if (!particlesCopy[x + 1][y + 1] && (x + 1 < 192)) {
						ParticleMan::particles[x][y] = false;
						ParticleMan::particles[x + 1][y + 1] = true;
					}
				}
			}
		}
	
}

void addWaterParticle(std::shared_ptr<Engine::engineObject> obj) {
	bool particlesCopy[192][108];
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			particlesCopy[x][y] = ParticleMan::particles[x][y];

		}
	}
	if (Engine::mouseStates[2] == 2) {
		ParticleMan::particles[int(Engine::mousePos.x)][int(Engine::mousePos.y)] = true;
	}
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			if (particlesCopy[x][y] && (y + 1 < 108)) {
				//If nothing below, move down
				if (!particlesCopy[x][y + 1]) { 
					ParticleMan::particles[x][y] = false;
					ParticleMan::particles[x][y + 1] = true;
				}
				else if (!particlesCopy[x - 1][y + 1] && (x - 1 > 0)) {
					ParticleMan::particles[x][y] = false;
					ParticleMan::particles[x - 1][y + 1] = true;
				}
				else if (!particlesCopy[x + 1][y + 1] && (x + 1 < 192)) {
					ParticleMan::particles[x][y] = false;
					ParticleMan::particles[x + 1][y + 1] = true;
				}
			}
		}
	}

}

void ParticleMan::initParticle() {
	for (int x = 0; x < (sizeof(particles) / sizeof(*particles)); x++) {
		for (int y = 0; y < (sizeof(particles[0]) / sizeof(*particles[0])); y++) {
			particles[x][y] = false;
		}
	}
	for (int i = 0; i < rand() % 192; i++)
		particles[rand()%192][rand()%108] = true;


	grid = std::make_shared<Engine::engineObject>(Engine::engineObject({ 0, 0, float(Engine::baseRes.x), float(Engine::baseRes.y) }, NULL, 0, false));
	grid->tex = Engine::loadTargetTex({ int(grid->hull.w), int(grid->hull.h) });
	Engine::addObject(grid);
	grid->updateFuncs.push_back(addWaterParticle);
	grid->updateFuncs.push_back(addSandParticle);
	grid->drawFuncs.push_back(drawGrid);
}
*/