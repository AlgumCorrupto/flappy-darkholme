#ifndef PLAYER
#define PLAYER

#include <SDL2/SDL.h>
#include <iostream>

struct Cartesian {
    float x;
    float y;
};

class Player {
public:
    std::string textuPath = {0};
    float jumpForce = {0};
    int height = {0};
    int width = {0};
    float mass = {0};
    double velocity = 0;
    double acceleration = 0;
    struct Cartesian anchor;

    Player(std::string textPath, float jForce, int h, int w, float m): textuPath{textPath}, jumpForce{jForce}, height{h}, width{w}, mass{m} {  };

};




#endif