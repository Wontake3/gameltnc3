#include "intro.h"
#include "mainwindow.h"
#include "defs.h"

#include <SDL.h>

Intro::Intro(const MainWindow& _win)
    : win(_win),
      background(_win),
      title(_win.getRenderer()),
      subtitle(_win.getRenderer()) {
    background.loadTextures(BACKGROUND_DAY, BACKGROUND_NIGHT);

    Font bigFont, smallFont;
    bigFont.loadFromFile(FONT_PATH, TEXT_SIZE_BIG);
    smallFont.loadFromFile(FONT_PATH, TEXT_SIZE_SMALL);

    title.loadFromRenderedText(WINDOW_TITLE, bigFont, SDL_Color{0xFF, 0, 0, 0xFF});
    subtitle.loadFromRenderedText("Click if you can ...", smallFont, SDL_Color{0, 0, 0, 0xFF});
}

void Intro::logic() {
    GameState::updateDayNight();
    background.scroll(3);
}

void Intro::render() {
    win.clear();
    background.render();
    title.render((win.getWidth() - title.getWidth()) / 2,
                 (win.getHeight() - title.getHeight()) / 2);
    subtitle.render((win.getWidth() - subtitle.getWidth()) / 2,
                    (win.getHeight() + title.getHeight() - subtitle.getHeight()) / 2);
    win.update();
}
