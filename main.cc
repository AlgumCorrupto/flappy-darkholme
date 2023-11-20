#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "src/Player.h"
#include <math.h>
#include <windows.h>
#include <random>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720

#define GRAVITY_FORCE 1.0d
#define TIMESTEP 1

#define FPS 60

#define GAP_SIZE 256

void createObstacle(SDL_Rect *rect1, SDL_Rect *rect2, int offset);
void updateObstacle(SDL_Rect *rect1, SDL_Rect *rect2);
bool checkCollision(SDL_Rect *player, SDL_Rect *obstacle);

int main(int argc, char** argv){
    //inicia o SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    //cria objeto window
    SDL_Window *window = SDL_CreateWindow("Flappy Darkholme", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    //criação do renderer sdl
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }


    //criação de objeto player
    Player* player = new Player("assets/player.jpg", 2.0f, 64, 64, 1);

    SDL_Surface* tmpSurface = IMG_Load(player->textuPath.c_str());
    SDL_Texture* playerTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    tmpSurface = IMG_Load("assets/MEATE.jpg");
    SDL_Texture* obsTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);


    SDL_Rect playerRect;
    playerRect.h = player->height;
    playerRect.w = player->width;
    playerRect.x = SCREEN_WIDTH/2;
    playerRect.y = 10;

    SDL_Rect obstaculos[4][2];

    for(int i = 0; i < 4; i++){
        createObstacle(&obstaculos[i][0], &obstaculos[i][1], i);
    }
  

    int time = 1;

    uint32_t starting_tick;

    std::cout << player->textuPath.c_str() << " " << player->width << "\n" ;
    //game loop
    bool running = true;
    while(running){
        starting_tick = SDL_GetTicks();
        //criação de eventos SDL
        SDL_Event event;
        //loop de eventos
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if(SDLK_SPACE == event.key.keysym.sym)  
                        player->velocity = -12.0f;
                    break;
                default:
                    break;
            }
        }

        if(1000/FPS > SDL_GetTicks() - starting_tick) {
            SDL_Delay(1000/FPS - (SDL_GetTicks() -  starting_tick));
        }

        //std::cout << player->acceleration << '\n'; 
        //update
        //Aceleração = (velocidade/tempo) + força da gravidade
        player->acceleration = player->velocity/time+GRAVITY_FORCE;
        time += TIMESTEP;
        if(playerRect.y > SCREEN_HEIGHT)
            //running = false;
            playerRect.y = 0;
        else
        //posição = posição inicial + velocidade
            playerRect.y += TIMESTEP*player->velocity;
        player->velocity += TIMESTEP * player->acceleration;
        
        for(int i = 0; i < 4; i++){
            obstaculos[i][0].x = obstaculos[i][1].x -= TIMESTEP*2;
            if(checkCollision(&playerRect, &obstaculos[i][0]))
                running = false;
            
            if(checkCollision(&playerRect, &obstaculos[i][1]))
                running = false;
            if(obstaculos[i][0].x < -100) {
                obstaculos[i][0].x = obstaculos[i][1].x = SCREEN_WIDTH;
                updateObstacle(&obstaculos[i][0], &obstaculos[i][1]);
            }
                
        }

        //desenhar
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, playerTex, NULL, &playerRect);
        
        for(int i = 0; i < 4; i++){
            SDL_RenderCopy(renderer, obsTex, NULL, &obstaculos[i][0]);
            SDL_RenderCopy(renderer, obsTex, NULL, &obstaculos[i][1]);
        }

        //apresentar o que foi renderizado
        SDL_RenderPresent(renderer);
    }

    return 0;
}

void createObstacle(SDL_Rect *rect1, SDL_Rect *rect2, int offset) {
    rect1->y = 0;
    int random = rand() % (SCREEN_HEIGHT - 384 ) + 384;
    rect1->h = random - GAP_SIZE;
    rect2->y = rect1->h + GAP_SIZE;
    rect2->h = SCREEN_HEIGHT - rect2->y;

    rect1->x = SCREEN_WIDTH + 325*offset;
    rect2->x = SCREEN_WIDTH + 325*offset;
    rect1->w = 64;
    rect2->w = 64;
}

void updateObstacle(SDL_Rect *rect1, SDL_Rect *rect2) {
    int random = rand() % (SCREEN_HEIGHT - 384) + 384;
    rect1->h =  random - GAP_SIZE;
    rect2->y = rect1->h + GAP_SIZE;
    rect2->h = SCREEN_HEIGHT - rect2->y;
}

//NIGAGAAAAAAAAAAAAAGAGAGAGAGA
bool checkCollision(SDL_Rect *player, SDL_Rect *obstacle) {
    if(player->x + player->w >= obstacle->x && obstacle->x + obstacle->w >= player->x ){
        if(player->y + player->h >= obstacle->y && obstacle->y + obstacle->h >= player->y ) {
            std::cout << "colisao" << '\n';
            return true;
        }
            
    }
    return false;
}