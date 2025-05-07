#include "pumpkin.h"

Pumpkin::Pumpkin() : x(0), y(0), active(false), texture(nullptr) {}

void Pumpkin::init(Texture* tex, int x_, int y_) {
    x = x_;
    y = y_;
    texture = tex;
    active = true;
}

void Pumpkin::render(SDL_Renderer* renderer) {
    if (active && texture) {
        SDL_Rect dst = {x, y, WIDTH, HEIGHT};
        texture->render(x, y, &dst);
    }
}

SDL_Rect Pumpkin::getRect() const {
    return SDL_Rect{x, y, WIDTH, HEIGHT};
}

bool Pumpkin::isActive() const {
    return active;
}

void Pumpkin::setActive(bool a) {
    active = a;
}

int Pumpkin::getX() const {
    return x;
}

int Pumpkin::getY() const {
    return y;
}
