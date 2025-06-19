#include "../include/GUI.hpp"
#include "../include/FieldCalculator.hpp"
#include <iostream>
#include <cmath>

int windowWidth;
int windowHeight;

const int scale = 40;

// Convert screen pixels to simulation grid (centered at (0,0))
Vector2D screenToGrid(Vector2D screenPos)
{
    double gx = (screenPos.x - windowWidth / 2) / 40.0;
    double gy = (screenPos.y - windowHeight / 2) / 40.0;
    return Vector2D(gx, gy);
}

// Convert simulation grid (centered at 0,0) to screen pixels
Vector2D gridToScreen(Vector2D gridPos)
{
    double sx = windowWidth / 2 + gridPos.x * 40.0;
    double sy = windowHeight / 2 - gridPos.y * 40.0;
    return Vector2D(sx, sy);
}

GUI::GUI()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w * 0.8;
    windowHeight = displayMode.h * 0.8;

    window = SDL_CreateWindow("EM Field Visualizer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight,
                              SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    arrowTexture = IMG_LoadTexture(renderer, "/assets/arrow.png");
    font = TTF_OpenFont("/assets/arial.ttf", 16);
}

GUI::~GUI()
{
    SDL_DestroyTexture(arrowTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GUI::run()
{
    mainLoop();
}

void GUI::mainLoop()
{
    bool running = true;
    SDL_Event e;
    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            handleEvents(e);
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        if (inStartScreen)
        {
            drawStartScreen();
        }
        else
        {
            drawGrid();
            drawVectorField();
            drawField();
            drawUI();
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void GUI::handleEvents(SDL_Event &e)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
    {
        windowWidth = e.window.data1;
        windowHeight = e.window.data2;
    }
    else if (e.type == SDL_QUIT)
    {
        exit(0);
    }

    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_g:
            showGrid = !showGrid;
            break;
        case SDLK_ESCAPE:
            exit(0);
            break;
        case SDLK_p:
        {
            charges.emplace_back(Vector2D(mx, my), POSITIVE);
            break;
        }
        case SDLK_n:
        {
            charges.emplace_back(Vector2D(mx, my), NEGATIVE);
            break;
        }
        case SDLK_d:
        {
            // Delete the first charge found under mouse
            for (auto it = charges.begin(); it != charges.end(); ++it)
            {
                if (it->isInside(mx, my))
                {
                    charges.erase(it);
                    break;
                }
            }
            break;
        }
        }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN && inStartScreen)
    {
        if (isInsideStartButton(mx, my))
        {
            inStartScreen = false;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        for (auto &c : charges)
        {
            if (c.isInside(mx, my))
                c.isDragging = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP)
    {
        for (auto &c : charges)
            c.isDragging = false;
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        for (auto &c : charges)
        {
            if (c.isDragging)
                c.pos = Vector2D(mx, my);
        }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN && !inStartScreen)
    {
        if (mx >= goBackButton.x && mx <= goBackButton.x + goBackButton.w &&
            my >= goBackButton.y && my <= goBackButton.y + goBackButton.h)
        {
            inStartScreen = true;
            charges.clear(); // Optional: also reset when going back
            return;
        }

        if (mx >= resetButton.x && mx <= resetButton.x + resetButton.w &&
            my >= resetButton.y && my <= resetButton.y + resetButton.h)
        {
            charges.clear();
            return;
        }
    }
}

void GUI::drawGrid()
{
    if (!showGrid)
        return;

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 150);

    const int scale = 40;
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    const int xLines = windowWidth / scale;
    const int yLines = windowHeight / scale;

    // Vertical lines
    for (int i = -xLines / 2; i <= xLines / 2; ++i)
    {
        int x = centerX + i * scale;
        SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
    }

    // Horizontal lines
    for (int j = -yLines / 2; j <= yLines / 2; ++j)
    {
        int y = centerY - j * scale;
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }

    // Central axes
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawLine(renderer, centerX, 0, centerX, windowHeight); // Y-axis
    SDL_RenderDrawLine(renderer, 0, centerY, windowWidth, centerY);  // X-axis
}

void GUI::drawStartScreen()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Create textures
    SDL_Texture *title = renderText("Welcome to the EM Field Visualizer", 24);
    SDL_Texture *instruct1 = renderText("Press G to toggle grid on/off", 18);
    SDL_Texture *instruct2 = renderText("Press P to create a POSITIVE charge", 18);
    SDL_Texture *instruct3 = renderText("Press N to create a NEGATIVE charge", 18);
    SDL_Texture *instruct4 = renderText("Hover over a charge and press D to DELETE it", 18);
    //SDL_Texture *instruct5 = renderText("Press ESC to quit", 18);
    SDL_Texture *instruct5 = renderText("Click START to begin simulation", 18);
    SDL_Texture *startLabel = renderText("START", 20);

    int centerX = windowWidth / 2;
    int y = windowHeight / 2 - 160;

    int spacing = 40;

    // Center each line individually
    int w, h;

    SDL_QueryTexture(title, nullptr, nullptr, &w, &h);
    SDL_Rect r1 = {centerX - w / 2, y, w, h};
    y += spacing;

    SDL_QueryTexture(instruct1, nullptr, nullptr, &w, &h);
    SDL_Rect r2 = {centerX - w / 2, y, w, h};
    y += spacing;

    SDL_QueryTexture(instruct2, nullptr, nullptr, &w, &h);
    SDL_Rect r3 = {centerX - w / 2, y, w, h};
    y += spacing;

    SDL_QueryTexture(instruct3, nullptr, nullptr, &w, &h);
    SDL_Rect r4 = {centerX - w / 2, y, w, h};
    y += spacing;

    SDL_QueryTexture(instruct4, nullptr, nullptr, &w, &h);
    SDL_Rect r5 = {centerX - w / 2, y, w, h};
    y += spacing;

    /*
    SDL_QueryTexture(instruct5, nullptr, nullptr, &w, &h);
    SDL_Rect r6 = {centerX - w / 2, y, w, h};
    y += spacing;
    */

    SDL_QueryTexture(instruct5, nullptr, nullptr, &w, &h);
    SDL_Rect r6 = {centerX - w / 2, y, w, h};
    y += spacing + 20;

    // START button
    startButton = {centerX - 100, y, 200, 50}; // fixed size
    SDL_SetRenderDrawColor(renderer, 0, 120, 255, 255);
    SDL_RenderFillRect(renderer, &startButton);

    // Center START label inside the button
    SDL_QueryTexture(startLabel, nullptr, nullptr, &w, &h);
    SDL_Rect startTextRect = {
        startButton.x + (startButton.w - w) / 2,
        startButton.y + (startButton.h - h) / 2,
        w, h};

    // Render everything
    SDL_RenderCopy(renderer, title, nullptr, &r1);
    SDL_RenderCopy(renderer, instruct1, nullptr, &r2);
    SDL_RenderCopy(renderer, instruct2, nullptr, &r3);
    SDL_RenderCopy(renderer, instruct3, nullptr, &r4);
    SDL_RenderCopy(renderer, instruct4, nullptr, &r5);
    SDL_RenderCopy(renderer, instruct5, nullptr, &r6);
    //SDL_RenderCopy(renderer, instruct6, nullptr, &r7);
    SDL_RenderCopy(renderer, startLabel, nullptr, &startTextRect);

    SDL_DestroyTexture(title);
    SDL_DestroyTexture(instruct1);
    SDL_DestroyTexture(instruct2);
    SDL_DestroyTexture(instruct3);
    SDL_DestroyTexture(instruct4);
    SDL_DestroyTexture(instruct5);
    //SDL_DestroyTexture(instruct6);
    SDL_DestroyTexture(startLabel);
}

bool GUI::isInsideStartButton(int x, int y)
{
    return x >= startButton.x && x <= startButton.x + startButton.w &&
           y >= startButton.y && y <= startButton.y + startButton.h;
}

void GUI::drawField()
{
    for (auto &c : charges)
    {
        c.render(renderer, font);

        if (c.type == POSITIVE || c.type == NEGATIVE)
        {
            int numLines = 12;
            float radius = 10.0f;
            float angleStep = 2 * M_PI / numLines;
            SDL_Color color = (c.type == POSITIVE) ? SDL_Color{255, 0, 0, 255} : SDL_Color{0, 0, 255, 255};

            for (int i = 0; i < numLines; ++i)
            {
                float angle = i * angleStep;
                Vector2D dir(cos(angle), sin(angle));
                Vector2D seed = c.pos + dir * radius;
                traceFieldLine(seed, true, color);
                traceFieldLine(seed, false, color);
            }
        }
    }
}

void GUI::drawVectorField()
{
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    const int scale = 40;

    int unitsX = windowWidth / scale / 2;
    int unitsY = windowHeight / scale / 2;

    for (int j = -unitsY; j <= unitsY; ++j)
    {
        for (int i = -unitsX; i <= unitsX; ++i)
        {
            int x = centerX + i * scale;
            int y = centerY - j * scale;

            Vector2D field = FieldCalculator::calculateFieldAt(Vector2D(x, y), charges);
            if (field.magnitude() > 0.1f)
                renderArrow(Vector2D(x, y), field.angle());
        }
    }
}

void GUI::renderArrow(Vector2D position, double angle)
{
    if (!arrowTexture)
        return;
    SDL_Rect dst = {(int)position.x - 10, (int)position.y - 10, 20, 20};
    SDL_Point center = {10, 10};
    SDL_RenderCopyEx(renderer, arrowTexture, nullptr, &dst, angle, &center, SDL_FLIP_NONE);
}

SDL_Texture *GUI::renderText(const std::string &message, int size)
{
    SDL_Color white = {255, 255, 255, 255};
    TTF_Font *tempFont = TTF_OpenFont("/assets/arial.ttf", size);
    SDL_Surface *surface = TTF_RenderText_Blended(tempFont, message.c_str(), white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    TTF_CloseFont(tempFont);
    return texture;
}

void GUI::drawUI()
{
    Vector2D p(windowWidth / 2, windowHeight / 2);
    float volts = FieldCalculator::calculateVoltageAt(p, charges);
    Vector2D field = FieldCalculator::calculateFieldAt(p, charges);

    SDL_Texture *t1 = renderText("Voltage: " + std::to_string((int)volts) + " V");
    SDL_Texture *t2 = renderText("Field: (" + std::to_string((int)field.x) + ", " + std::to_string((int)field.y) + ") N/C");
    SDL_Rect r1 = {10, 10, 200, 20};
    SDL_Rect r2 = {10, 35, 300, 20};
    SDL_RenderCopy(renderer, t1, nullptr, &r1);
    SDL_RenderCopy(renderer, t2, nullptr, &r2);
    SDL_DestroyTexture(t1);
    SDL_DestroyTexture(t2);

    // Dynamic charge coordinate display
    int yOffset = 60;
    for (size_t i = 0; i < charges.size(); ++i)
    {
        const auto &c = charges[i];
        std::string label;

        if (c.type == POSITIVE)
            label += "Positive ";
        else if (c.type == NEGATIVE)
            label += "Negative ";
        else
            label += "Neutral ";

        label += "(" + std::to_string((int)c.pos.x - windowWidth / 2) + ", " + std::to_string((int)(windowHeight / 2 - c.pos.y)) + ")";
        SDL_Texture *coordLabel = renderText(label);
        SDL_Rect coordRect = {10, yOffset, 300, 20};
        SDL_RenderCopy(renderer, coordLabel, nullptr, &coordRect);
        SDL_DestroyTexture(coordLabel);
        yOffset += 20;
    }

    // Dynamic Y position near bottom
    int buttonY = windowHeight - 40;

    // === Go Back Button ===
    goBackButton = {20, buttonY, 120, 30};

    // === Reset Button ===
    resetButton = {windowWidth - 140, buttonY, 120, 30};

    // === Go Back Button ===
    // goBackButton = {20, 950, 120, 30};
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    SDL_RenderFillRect(renderer, &goBackButton);
    SDL_Texture *backLabel = renderText("Go Back", 16);
    int bw, bh;
    SDL_QueryTexture(backLabel, nullptr, nullptr, &bw, &bh);
    SDL_Rect backLabelRect = {
        goBackButton.x + (goBackButton.w - bw) / 2,
        goBackButton.y + (goBackButton.h - bh) / 2,
        bw, bh};
    SDL_RenderCopy(renderer, backLabel, nullptr, &backLabelRect);
    SDL_DestroyTexture(backLabel);

    // === Reset Button ===
    SDL_SetRenderDrawColor(renderer, 50, 150, 200, 255);
    SDL_RenderFillRect(renderer, &resetButton);
    SDL_Texture *resetLabel = renderText("Reset", 16);
    int rw, rh;
    SDL_QueryTexture(resetLabel, nullptr, nullptr, &rw, &rh);
    SDL_Rect resetLabelRect = {
        resetButton.x + (resetButton.w - rw) / 2,
        resetButton.y + (resetButton.h - rh) / 2,
        rw, rh};
    SDL_RenderCopy(renderer, resetLabel, nullptr, &resetLabelRect);
    SDL_DestroyTexture(resetLabel);
}

void GUI::traceFieldLine(Vector2D start, bool forward, SDL_Color color)
{
    Vector2D pos = start;
    int steps = 500;
    float stepSize = 4.0f;

    for (int i = 0; i < steps; ++i)
    {
        Vector2D E = FieldCalculator::calculateFieldAt(pos, charges);
        if (E.magnitude() < 1e-3f)
            break;

        Vector2D dir = E.normalized();
        if (!forward)
            dir = dir * -1;

        Vector2D next = pos + dir * stepSize;

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawLine(renderer, (int)pos.x, (int)pos.y, (int)next.x, (int)next.y);

        pos = next;
    }
}

Vector2D GUI::screenToGrid(Vector2D screenPos)
{
    double gx = (screenPos.x - windowWidth / 2) / 40.0;
    double gy = (screenPos.y - windowHeight / 2) / 40.0;
    return Vector2D(gx, gy);
}

Vector2D GUI::gridToScreen(Vector2D gridPos)
{
    double sx = windowWidth / 2 + gridPos.x * 40.0;
    double sy = windowHeight / 2 - gridPos.y * 40.0;
    return Vector2D(sx, sy);
}

void GUI::mainLoopStep()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        handleEvents(e);
    }

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    if (inStartScreen)
    {
        drawStartScreen();
    }
    else
    {
        drawGrid();
        drawVectorField();
        drawField();
        drawUI();
    }

    SDL_RenderPresent(renderer);
}
