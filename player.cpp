#include "player.h"
#include "defs.h"
#include "SDL.h"

#define MAX_ANGLE 80.0
#define GRAVITY_ACCEL 2.5

Player::Player(SDL_Renderer* renderer, double x, double y)
    : texture{renderer},
    isDead{false},
    posX{x}, posY{y},
    radius{0.0},
    vertSpeed{0.0},
    vertAccel{0.05}
{
    texture.loadFromFile(BIRD_IMAGE);
    radius = texture.getWidth() / 3;
}

void Player::make_jump()
{
    if (vertSpeed > 0) vertSpeed = -GRAVITY_ACCEL;
    else vertSpeed -= GRAVITY_ACCEL;
}

void Player::apply_forces()
{
    vertSpeed += vertAccel;
    posY += vertSpeed;
}

void Player::render(Texture* customTexture)
{
    double angle = vertSpeed * 3.0;
    if (angle > MAX_ANGLE) angle = MAX_ANGLE;
    Texture* tex = customTexture ? customTexture : &texture;
    tex->render(
        static_cast<int>(posX - radius),
        static_cast<int>(posY - radius),
        nullptr,
        angle
    );
}

void Player::render()
{
    render(nullptr);
}

SDL_Rect Player::getRect() const {
    return SDL_Rect{
        static_cast<int>(posX - radius),
        static_cast<int>(posY - radius),
        static_cast<int>(radius * 2),
        static_cast<int>(radius * 2)
    };
}
