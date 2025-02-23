#include "SDL.h"
#include "ECS.h"
#include "GameplayScene.h"
#include <iostream>
enum class GameState {
	MainMenu,
	Gameplay,
	GameOver
};
class Game {
public:
	Game() {};
	~Game() {};
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; };
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static Manager gameplayManager;
	static Manager mainMenuManager;
	static Manager gameOverManager;
	static GameplayScene gameplayScene;
private:
	GameState gameState = GameState::MainMenu;
	bool isRunning;
	SDL_Window* window;
};