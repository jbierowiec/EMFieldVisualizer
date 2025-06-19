#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Vector2D.hpp"
#include "Charge.hpp"

class GUI {
public:
    GUI();
    ~GUI();
    void run();
    void mainLoopStep();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* arrowTexture;
    TTF_Font* font;
    std::vector<Charge> charges;
    SDL_Rect startButton;
    SDL_Rect goBackButton;
    SDL_Rect resetButton;


    bool showGrid = false;
    bool inStartScreen = true;

    void showMenu();
    void mainLoop();
    void handleEvents(SDL_Event& e);
    void drawField();
    void drawVectorField();
    void drawUI();
    void drawGrid();
    void drawStartScreen();
    bool isInsideStartButton(int x, int y);
    void renderArrow(Vector2D position, double angle);
    SDL_Texture* renderText(const std::string& message, int size = 16);
    void traceFieldLine(Vector2D start, bool forward, SDL_Color color);
    Vector2D screenToGrid(Vector2D screenPos);
    Vector2D gridToScreen(Vector2D gridPos);
};
