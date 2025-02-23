#include "TextureManager.h"
#include "SDL_image.h"
#include "Game.h"
SDL_Texture* TextureManager::LoadTexture(const char* filename) {
	SDL_Surface* tempSurface = IMG_Load(filename);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}
void TextureManager::Draw(SDL_Texture* objTexture, SDL_Rect srcRect, SDL_Rect desRect)
{
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &desRect);
}