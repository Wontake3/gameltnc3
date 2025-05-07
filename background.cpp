#include "background.h"
#include "mainwindow.h"

Background::Background(const MainWindow& win)
    : dayTexture(win.getRenderer()), nightTexture(win.getRenderer()), scrollingOffset(0),
        isTransitioning(false), transitionAlpha ( 255) {
}

void Background::loadTextures(const std::string& dayPath, const std::string& nightPath) {
    dayTexture.loadFromFile(dayPath);
    nightTexture.loadFromFile(nightPath);
}

void Background::updateTransition() {
      if (GameState::justSwitched) {
           isTransitioning = true;
      }
      if ( isTransitioning) {
          if (GameState::isDay) {
               if(transitionAlpha < 255) {
                transitionAlpha += fadeSpeed;
               } else {
                   transitionAlpha = 255;
                   isTransitioning = false;
               }

          } else {
              if ( transitionAlpha > 0) {
                   transitionAlpha -= fadeSpeed;
              } else {
                   transitionAlpha = 0;
                   isTransitioning = false;
              }
          }
      }
}

void Background::render() {
    updateTransition();

    int textureWidth = GameState::isDay ? dayTexture.getWidth() : nightTexture.getWidth();
    if (isTransitioning) {
        dayTexture.setAlpha(transitionAlpha);
        nightTexture.setAlpha(255 - transitionAlpha);
        dayTexture.render(scrollingOffset, 0);
        dayTexture.render(scrollingOffset + textureWidth, 0);
        nightTexture.render(scrollingOffset, 0);
        nightTexture.render(scrollingOffset + textureWidth, 0);
    } else if (GameState::isDay) {
        dayTexture.setAlpha(255);
        dayTexture.render(scrollingOffset, 0);
        dayTexture.render(scrollingOffset + textureWidth, 0);
    } else {
        nightTexture.setAlpha(255);
        nightTexture.render(scrollingOffset, 0);
        nightTexture.render(scrollingOffset + textureWidth, 0);
    }
}

void Background::scroll(int distance) {
    scrollingOffset -= distance;
    int textureWidth = GameState::isDay ? dayTexture.getWidth() : nightTexture.getWidth();
    if (scrollingOffset <= -textureWidth) {
        scrollingOffset += textureWidth;
    }
}
