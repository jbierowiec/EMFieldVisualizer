#include "../include/Charge.hpp"

Charge::Charge(Vector2D position, ChargeType type, float mag)
    : pos(position), type(type), magnitude(mag) {}

bool Charge::isInside(int mx, int my) const {
    return (mx >= pos.x - 10 && mx <= pos.x + 10 && my >= pos.y - 10 && my <= pos.y + 10);
}

void Charge::render(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Color fill;
    const char* sign = "";

    switch (type) {
        case POSITIVE: fill = {255, 0, 0, 255}; sign = "+"; break;
        case NEGATIVE: fill = {0, 0, 255, 255}; sign = "-"; break;
        case NEUTRAL: fill = {128, 128, 128, 255}; sign = ""; break;
    }

    // Draw circle manually
    for (int w = 0; w < 20; w++) {
        for (int h = 0; h < 20; h++) {
            int dx = 10 - w;
            int dy = 10 - h;
            if ((dx*dx + dy*dy) <= 100) {
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
                SDL_RenderDrawPoint(renderer, pos.x - 10 + w, pos.y - 10 + h);
            }
        }
    }

    // Draw symbol safely
    if (*sign != '\0' && font != nullptr) {
        SDL_Surface* surface = TTF_RenderText_Blended(font, sign, color);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                SDL_Rect dst = {(int)pos.x - surface->w / 2, (int)pos.y - surface->h / 2, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        } else {
            SDL_Log("Failed to render symbol: %s", TTF_GetError());
        }
    }
}
