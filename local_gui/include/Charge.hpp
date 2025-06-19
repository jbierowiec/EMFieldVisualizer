#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Vector2D.hpp"

enum ChargeType { POSITIVE, NEGATIVE, NEUTRAL };

class Charge {
public:
    Vector2D pos;
    ChargeType type;
    bool isDragging = false;
    float magnitude;

    Charge(Vector2D position, ChargeType type, float mag = 1.0f);
    void render(SDL_Renderer* renderer, TTF_Font* font);
    bool isInside(int mx, int my) const;
};
