#include <iostream>
#include <SDL2/SDL.h>
#include "circle.h"

// Nu ne-a mers SDL-ul :(

//define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//define SDL Window related variables
SDL_Window *window = NULL;
SDL_Renderer *windowRenderer = NULL;
SDL_Event currentEvent;

SDL_Rect rectangle;

bool quit = false;

int mouseX, mouseY;

Circle cerc;

bool goingRight = true;
bool animationRunning = false;

glm::vec2 movementDirection;
float circleSpeed = 2000.0f;
float dt = 0.1f;

float elapsedTime = 0.0f;

bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    } else {

        //Try to create the window
        window = SDL_CreateWindow(
                "SDL Hello Window Example",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if(window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        } else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if(windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            } else {

                //Set background color
                SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

                //Apply background color
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}

void processEvents() {

    //Check for events in queue
    SDL_PollEvent(&currentEvent);

    //User requests quit
    if(currentEvent.type == SDL_QUIT) {

        quit = true;
    }

    //Mouse event -> pressed button
    if(currentEvent.type == SDL_MOUSEBUTTONDOWN) {

        if(currentEvent.button.button == SDL_BUTTON_LEFT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse left click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }

        if(currentEvent.button.button == SDL_BUTTON_RIGHT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse right click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    //Mouse event -> mouse movement
    if(currentEvent.type == SDL_MOUSEMOTION) {

        SDL_GetMouseState(&mouseX, &mouseY);
        //std::cout << "Mouse move => " << "x: " << mouseX << ", y: " << mouseY << std::endl;

        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move while left click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }

        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move while right click=> " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    //Keyboard event
    if(currentEvent.type == SDL_KEYDOWN) {

        switch(currentEvent.key.keysym.sym) {

            case SDLK_UP:
                break;

            case SDLK_DOWN:
                break;

            case SDLK_LEFT:
                break;

            case SDLK_RIGHT:
                break;

            case SDLK_r:
                animationRunning = !animationRunning;
                break;

            case SDLK_s:
                break;

            case SDLK_ESCAPE:

                quit = true;
                break;

            default:
                break;
        }
    }
}

void animationStep() {

//    if (goingRight) {
//        cerc.center.x += 0.1f;
//        if (cerc.center.x > 600.0f) {
//            goingRight = false;
//        }
//
//    } else {
//        cerc.center.x -= 0.1f;
//        if (cerc.center.x < 200.0f) {
//            goingRight = true;
//        }
//    }
    glm::vec2 mousePos = glm::vec2(mouseX, mouseY);
    movementDirection = glm::normalize(mousePos - cerc.center);

    cerc.center += movementDirection * circleSpeed * dt * elapsedTime;

    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(windowRenderer, cerc.center.x, cerc.center.y, mousePos.x, mousePos.y);

}

void drawFrame() {

    Uint32 start = SDL_GetTicks();

    //Clear the background
    SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
    SDL_RenderClear(windowRenderer);

    //Draw scene
    //TODO

    //draw line
    SDL_SetRenderDrawColor(windowRenderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(windowRenderer, 0, 0, WINDOW_WIDTH, 0);
    SDL_RenderDrawLine(windowRenderer, WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_RenderDrawLine(windowRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, 0, WINDOW_HEIGHT);

    //draw rectangle
    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 255, 255);
    SDL_RenderFillRect(windowRenderer, &rectangle);

    //draw circle
    SDL_FillCircle(windowRenderer, cerc);

    if (animationRunning) {
        animationStep();
    }

    //Update window
    SDL_RenderPresent(windowRenderer);

    Uint32 end = SDL_GetTicks();
    elapsedTime = (end - start) / 1000.0f;
}

void cleanup() {

    //Destroy renderer
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }

    //Destroy window
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }

    //Quit SDL
    SDL_Quit();
}

void initObjects() {

    //init rectangle
    rectangle.x = 10;
    rectangle.y = WINDOW_HEIGHT / 2;
    rectangle.w = 50;
    rectangle.h = 200;

    //init circle
    cerc.radius = 10.0f;
    cerc.center.x = WINDOW_WIDTH / 2;
    cerc.center.y = WINDOW_HEIGHT / 2;
    cerc.color = glm::vec4(100.0f, 0.0f, 100.0f, 0.0f);
}

int main(int argc, char * argv[]) {
    //Initialize window
    if(!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    initObjects();

    //Game loop
    while (!quit) {

        processEvents();
        drawFrame();
    }

    cleanup();
    return 0;
}