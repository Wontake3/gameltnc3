
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "texture.h"
#include "gamestate.h"
#include <string>
#include <SDL.h>

class MainWindow;

class Background {
public:
    Background(const MainWindow& win);
    void loadTextures(const std::string& dayPath, const std::string& nightPath);
    void render();
    void scroll(int distance);

private:
    Texture dayTexture;
    Texture nightTexture;
    int scrollingOffset;
    bool isTransitioning;
    Uint8 transitionAlpha;
    const int fadeSpeed = 2;
    void updateTransition() ;
};

#endif
