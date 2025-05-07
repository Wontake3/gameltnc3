#ifndef MENU_H
#define MENU_H

#include "mainwindow.h"
#include "font.h"
#include "texture.h"
#include "defs.h"
#include "music.h"
#include <vector>
#include <string>
#include <memory>

class Menu {
    const MainWindow& win;
    Font font;
    std::vector<std::unique_ptr<Texture>> options;
    int selected = 0;
    Texture background;
    Sound menu_s;

public:
    Menu(const MainWindow&);
    void handle_events(GameStates& next_state);
    void render();
};

#endif
