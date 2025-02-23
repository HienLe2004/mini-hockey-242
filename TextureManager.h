#pragma once
#include "SDL.h"
class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* filename);
	static void Draw(SDL_Texture* objTexture, SDL_Rect srcRect, SDL_Rect desRect);
};