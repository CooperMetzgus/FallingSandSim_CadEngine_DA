#pragma once
#include "engine.h"

static class ParticleMan {
public:
	static std::vector< std::vector<bool>> particleGrid;

	struct particleObject : public Engine::engineObject	{
		SDL_FPoint vel;

		bool unoccupiedSpace(SDL_FPoint changePos) {
			//Apply changePos to hull
			SDL_FPoint checkPos = { hull.x + changePos.x, hull.y + changePos.y };

			if (checkPos.x < 0 ||
				checkPos.x >= Engine::baseRes.x ||
				checkPos.y < 0 ||
				checkPos.y >= Engine::baseRes.y) {
				return false;
			}
				return !particleGrid[checkPos.x][checkPos.y];
		}

		bool tryMove(SDL_FPoint changePos)	{
			if (unoccupiedSpace(changePos))	{
				SDL_FPoint newPos = { hull.x + changePos.x, hull.y + changePos.y };

				//Adjust Particle Position
				ParticleMan::particleGrid[hull.x][hull.y] = false; //Particle Free
				hull.x = newPos.x;
				hull.y = newPos.y;
				ParticleMan::particleGrid[hull.x][hull.y] = true; //Particle Bind

				return true;
			}
			return false;
		}

		particleObject(SDL_Color color = { 255, 255, 255, 255 }, const SDL_FRect & hull = { 0, 0, 1, 1 }, double rot = 0,
			bool centered = false, SDL_FlipMode flip = SDL_FLIP_NONE, float scale = 1.0,
			SDL_FPoint vel = { 0, 0 }, double spin = 0, int depth = 0)
			: engineObject(hull, NULL, rot,
				centered, flip, scale,
				depth),
			vel(vel) {
			tex = Engine::loadTargetTex({ 1,1 });
			SDL_Texture* texture = Engine::setRenderTarget(tex);
			Engine::drawRect(hull, color);
			Engine::setRenderTarget(texture);
		}
	};

	struct brushObject : Engine::engineObject {
		int brushSize = 3;
		int selectedParticle = 0;
		
		void addParticle() { //Function adds Active Engine Object
			if (Engine::mouseStates[0] == 2) {
				std::vector<SDL_FPoint> scaledBrush;
				scaledBrush = generateBrushScale();
				for (int i = 0; i < scaledBrush.size(); i++)
				if (!particleGrid[scaledBrush[i].x][scaledBrush[i].y]) { //If Nothing in Grid Tile at Mouse Position
					particleObject passParticleData = ParticleMan::particleTypes[selectedParticle];
					activeParticles.push_back(Engine::addObject(std::shared_ptr<ParticleMan::particleObject>(new ParticleMan::particleObject(passParticleData))));
					activeParticles.back()->hull.x = int(scaledBrush[i].x);
					activeParticles.back()->hull.y = int(scaledBrush[i].y);
					particleGrid[scaledBrush[i].x][scaledBrush[i].y] = true;
				}
			}
			if (Engine::mouseStates[1] == 2) {
				if (!particleGrid[Engine::mousePos.x][Engine::mousePos.y]) { //If Nothing in Particle Grid at Mouse Position

				}
			}
			if (Engine::mouseStates[2] == 2) {
				if (particleGrid[Engine::mousePos.x][Engine::mousePos.y]) { //If Something in Particle Grid at Mouse Position
					particleGrid[Engine::mousePos.x][Engine::mousePos.y] = false;
				}
			}
			//Brush Scaling Controls
			if (Engine::wheelStates[0] == 1) {
				brushSize++;
				printf("Brush Size: %i\n", brushSize);
			}

			if (Engine::wheelStates[1] == 1) {
				brushSize--;
				if (brushSize <= 0) {
					brushSize = 0;
					printf("Brush Size: %i\n", brushSize);
				}
			}
		}

		void particleSelection() {
			if (Engine::keyStates[SDL_SCANCODE_D] == 1) {
				selectedParticle++;
				if (selectedParticle >= particleTypes.size())
					selectedParticle = 0;

				printf("Selected Particle Type: %i\n", selectedParticle);
			}

			if (Engine::keyStates[SDL_SCANCODE_A] == 1) {
				selectedParticle--;
				if (selectedParticle < 0)
					selectedParticle = particleTypes.size() - 1;

				printf("Selected Particle Type: %i\n", selectedParticle);
			}
		}

		std::vector<SDL_FPoint> generateBrushScale() { //List of Vector Points for Brush
			int particleOffset = brushSize; //Used for Brush Line Start Position
			std::vector<SDL_FPoint> returnParticles; //Brush Line Values List for Drawing Line
			for (int i = 0; i < brushSize * 2 + 1; i++) {
				SDL_FPoint brushLineStart = { (Engine::mousePos.x - particleOffset), (Engine::mousePos.y - i) }; //Brush Line Start Set to Farthest Left Position, Based on Brush Size
				for (int j = 0; j < ((brushSize - i) * 2) + 1; j++) {
					returnParticles.push_back({ brushLineStart.x + j, brushLineStart.y }); //Call X to Brush Line Start, Loop to End of Horizontal Distance
					returnParticles.push_back({ Engine::mousePos.x + j - particleOffset, Engine::mousePos.y + i}); //Call X to Brush Line Start, Call Y to Next Row Down.
				}
				particleOffset--; //Move X Left
			}
			return returnParticles;
		}
		
		brushObject(SDL_Color color = { 255, 255, 255, 255 }, const SDL_FRect& hull = { 0, 0, 1, 1 }, double rot = 0,
			bool centered = false, SDL_FlipMode flip = SDL_FLIP_NONE, float scale = 1.0,
			SDL_FPoint vel = { 0, 0 }, double spin = 0, int depth = 0)
			: engineObject(hull, NULL, rot,
				centered, flip, scale,
				depth) {
			tex = Engine::loadTargetTex({ 1,1 });
			SDL_Texture* texture = Engine::setRenderTarget(tex);
			Engine::drawRect(hull, color);
			Engine::setRenderTarget(texture);
		}
	};
	static std::vector<particleObject> particleTypes;
	static void initParticle();
	static std::vector<std::shared_ptr <Engine::engineObject>> activeParticles;

};