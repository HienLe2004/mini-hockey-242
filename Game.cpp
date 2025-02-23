#include "Game.h"
#include "ECS.h"
#include "Components.h"
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager Game::mainMenuManager;
Manager Game::gameOverManager;
Manager Game::gameplayManager;
GameplayScene Game::gameplayScene;
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
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
		}
	}

}
void Game::update()
{
	if (gameState == GameState::MainMenu) {
		mainMenuManager.refresh();
		mainMenuManager.update();
	}
	else if (gameState == GameState::Gameplay) {
		gameplayManager.refresh();
		gameplayManager.update();
		//Check collision
		gameplayScene.checkCollision();
	}
	else if (gameState == GameState::GameOver) {
		gameOverManager.refresh();
		gameOverManager.update();
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
		gameplayManager.draw();
	}
	else if (gameState == GameState::GameOver) {
		SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // Red background
		gameOverManager.draw();
	}
	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}
