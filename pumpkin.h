
#ifndef PUMPKIN_H
#define PUMPKIN_H

#include "texture.h"
#include <SDL.h>

class Pumpkin {
public:
    Pumpkin();
    void init(Texture* tex, int x, int y);
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;
    bool isActive() const;
    void setActive(bool active);
    int getX() const;
    int getY() const;
private:
    int x, y;
    bool active;
    Texture* texture;
    static constexpr int WIDTH = 40;
    static constexpr int HEIGHT = 40;
};

#endif
