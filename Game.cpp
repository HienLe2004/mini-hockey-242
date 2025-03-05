#include "Game.h"
#include "ECS.h"
#include "Components.h"
using namespace std;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager Game::mainMenuManager;
Manager Game::gameOverManager;
Manager Game::gameplayManager;
Manager Game::gameplayWithAIManager;
GameplayScene Game::gameplayScene;
GameplayWithAIScene Game::gameplayWithAIScene;

TTF_Font* font = nullptr;
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	if (TTF_Init() == -1) {
		std::cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
		isRunning = false;
		return;
	}
	font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 24);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		isRunning = false;
		return;
	}

	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystem initialized!" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created!" << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!" << std::endl;
		}
		isRunning = true;
		//Gameplay entities
		gameplayScene.init(&gameplayManager);
		//Main menu entities
		//Game over entities
		//GameplayWithAI entities
		gameplayWithAIScene.init(&gameplayWithAIManager);
	}
	else
	{
		isRunning = false;
	}
}


void Game::handleEvents()
{
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
	{
		isRunning = false;
	}
	if (gameState == GameState::MainMenu) {
		// Press [N] to play
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_n) {
				gameState = GameState::Gameplay;
			}
			else if (event.key.keysym.sym == SDLK_o) {
				gameState = GameState::GameplayWithAI;
			}
		}
	}
	else if (gameState == GameState::Gameplay) {
		// Press [M] to back to MainMenu
		// Press [N] to end game
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_m) {
				gameState = GameState::MainMenu;
			}
			else if (event.key.keysym.sym == SDLK_n) {
				gameState = GameState::GameOver; 
			}
		}
		gameplayScene.handleEvents(event);
	}
	else if (gameState == GameState::GameOver) {
		// Press [N] to replay
		// Press [M] to back to MainMenu
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_n) {
				gameState = GameState::Gameplay;
			}
			else if (event.key.keysym.sym == SDLK_m) {
				gameState = GameState::MainMenu;
			}
			else if (event.key.keysym.sym == SDLK_o) {
				gameState = GameState::MainMenu;
			}
		}
	}
	else if (gameState == GameState::GameplayWithAI) {
		// Press [M] to back to MainMenu
		// Press [N] to end game
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_m) {
				gameState = GameState::MainMenu;
			}
			else if (event.key.keysym.sym == SDLK_n) {
				gameState = GameState::GameOver;
			}
		}
		gameplayWithAIScene.handleEvents(event);
	}

}
void Game::update() {
	if (gameState == GameState::MainMenu) {
		mainMenuManager.refresh();
		mainMenuManager.update();

	}
	else if (gameState == GameState::Gameplay) {
		gameplayManager.refresh();
		gameplayManager.update();
		if (gameplayScene.ballInOpponentGoal()) {
			gameplayScene.resetPuck();
		}
		else if (gameplayScene.ballInPlayerGoal()) {
			gameplayScene.resetPuck();
		}
		gameplayScene.checkCollision();
	}
	else if (gameState == GameState::GameOver) {
		gameOverManager.refresh();
		gameOverManager.update();
	}
	else if (gameState == GameState::GameplayWithAI) {
		gameplayWithAIManager.refresh();
		gameplayWithAIManager.update();
		//Check collision
		gameplayWithAIScene.checkCollision();
	}
	
}
void Game::render()
{
	SDL_RenderClear(renderer);
	if (gameState == GameState::MainMenu) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255); // Blue background
		mainMenuManager.draw();
	}
	else if (gameState == GameState::Gameplay) {

		gameplayScene.drawWalls(renderer);
		gameplayScene.drawScore(renderer, font);
		gameplayManager.draw();
	}
	else if (gameState == GameState::GameOver) {
		SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // Red background
		gameOverManager.draw();
	}
	else if (gameState == GameState::GameplayWithAI) {
		gameplayWithAIScene.drawWalls(renderer);
		gameplayWithAIManager.draw();
	}
	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	TTF_CloseFont(font);
	TTF_Quit();
	std::cout << "Game cleaned!" << std::endl;
}
