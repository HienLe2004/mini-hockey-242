#pragma once
#include "Components.h"
#include "TextureManager.h"
#include "SDL.h"
class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, desRect;
public:
	SpriteComponent() = default;
	SpriteComponent(const char* path) {
		setTex(path);
	}
	SpriteComponent(const char* path, int srcW, int srcH, int desW, int desH) {
		setTex(path);
		setRectsSize(srcW, srcH, desW, desH);
	}
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		srcRect.x = srcRect.y = 0;
	}
	void setRectsSize(int srcW, int srcH, int desW, int desH) {
		srcRect.w = srcW;
		srcRect.h = srcH;
		desRect.w = desW;
		desRect.h = desH;
	}
	void setTex(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}
	void update() override {
		desRect.x = (int)transform->position.x - desRect.w/2;
		desRect.y = (int)transform->position.y - desRect.h/2;
	}
	void draw() override {
		TextureManager::Draw(texture, srcRect, desRect); 
	}
};