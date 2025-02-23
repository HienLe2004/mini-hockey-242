#pragma once
#include "Components.h"
#include "SDL.h"
#include "Vector2D.h"
class KeyboardControlComponent : public Component
{
public:
	SDL_Keycode up, right, down, left;
	bool bUp, bRight, bDown, bLeft;
	KeyboardControlComponent(SDL_Keycode up, SDL_Keycode right, SDL_Keycode down, SDL_Keycode left) {
		this->up = up;
		this->right = right;
		this->down = down;
		this->left = left;
		bUp = bRight = bDown = bLeft = false;
	}
	void setKeys(SDL_Keycode up, SDL_Keycode right, SDL_Keycode down, SDL_Keycode left) {
		this->up = up;
		this->right = right;
		this->down = down;
		this->left = left;
		bUp = bRight = bDown = bLeft = false;
	}
	void reset() {
		bUp = bRight = bDown = bLeft = false;
	}
	Vector2D getDirection() {
		float x = 0;
		float y = 0;
		if ((!bUp && !bDown) || (bUp && bDown))
			y = 0;
		else if (bUp)
			y = -1;
		else
			y = 1;
		if ((!bRight && !bLeft) || (bRight && bLeft))
			x = 0;
		else if (bRight)
			x = 1;
		else
			x = -1;
		return (Vector2D(x, y).Normalize());
	}
	Vector2D handleEvents(SDL_Event event) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == up) {
				bUp = true;
			}
			else if (event.key.keysym.sym == right) {
				bRight = true;
			}
			else if (event.key.keysym.sym == down) {
				bDown = true;
			}
			else if (event.key.keysym.sym == left) {
				bLeft = true;
			}
		}
		else if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == up) {
				bUp = false;
			}
			else if (event.key.keysym.sym == right) {
				bRight = false;
			}
			else if (event.key.keysym.sym == down) {
				bDown = false;
			}
			else if (event.key.keysym.sym == left) {
				bLeft = false;
			}
		}
		return getDirection();
	}
};