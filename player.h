#ifndef PLAYER_H
#define PLAYER_H

#include "texture.h"

class Player {
    Texture texture;

public:
    bool isDead;
    double posX, posY;
    double radius;
    double vertSpeed, vertAccel;
    Player(SDL_Renderer* renderer, double x, double y);
    void make_jump();
    void render();
    void render(Texture* customTexture);
    void apply_forces();
    SDL_Rect getRect() const;
};

#endif
