#include "menu.h"
#include "mainwindow.h"
#include "font.h"
#include "texture.h"
#include "defs.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

Menu::Menu(const MainWindow& _win) : win(_win), font(), background(_win.getRenderer()), menu_s(SOUND_MENU) {
    background.loadFromFile(BACKGROUND_INTRO);
    menu_s.setVolume(128);
    std::vector<std::string> labels = {"START", "Highscore", "Exit"};
    std::vector<int> sizes = {60, 48, 40};
    std::vector<SDL_Color> colors = {
        {0xFF, 0, 0, 0xFF},
        {0xFF, 0, 0, 0xFF},
        {0xFF, 0xFF, 0, 0xFF}
    };
    for (size_t i = 0; i < labels.size(); ++i) {
        Font btnFont;
        btnFont.loadFromFile(MENU_FONT_PATH, sizes[i]);
        auto tex = std::make_unique<Texture>(win.getRenderer());
        tex->loadFromRenderedText(labels[i], btnFont, colors[i]);
        options.push_back(std::move(tex));
    }
}

void Menu::handle_events(GameStates& next_state) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            next_state = GameStates::Quit;
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP) {
                selected = (selected - 1 + options.size()) % options.size();
            }
            else if (e.key.keysym.sym == SDLK_DOWN) {
                selected = (selected + 1) % options.size();
            }
            else if (e.key.keysym.sym == SDLK_RETURN) {
                if (selected == 0) { Mix_PauseMusic(); menu_s.play(); SDL_Delay(200); Mix_ResumeMusic(); next_state = GameStates::Level; }
                else if (selected == 1) { Mix_PauseMusic(); menu_s.play(); SDL_Delay(200); Mix_ResumeMusic(); next_state = GameStates::HighScore; }
                else if (selected == 2) { Mix_PauseMusic(); menu_s.play(); SDL_Delay(200); Mix_ResumeMusic(); next_state = GameStates::Quit; }
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x;
            int my = e.button.y;
            int y = 200;
            for (size_t i = 0; i < options.size(); ++i) {
                int x = (win.getWidth() - options[i]->getWidth()) / 2;
                int w = options[i]->getWidth();
                int h = options[i]->getHeight();
                int padding_x = 30;
                int padding_y = 15;
                int hit_x = x - padding_x;
                int hit_y = y - padding_y;
                int hit_w = w + 2 * padding_x;
                int hit_h = h + 2 * padding_y;
                if (mx >= hit_x && mx <= hit_x + hit_w && my >= hit_y && my <= hit_y + hit_h) {
                    selected = i;
                    if (selected == 0) { Mix_PauseMusic(); menu_s.play(); SDL_Delay(200); Mix_ResumeMusic(); next_state = GameStates::Level; }
                    else if (selected == 1) { Mix_PauseMusic(); menu_s.play(); SDL_Delay(200); Mix_ResumeMusic(); next_state = GameStates::HighScore; }
                    else if (selected == 2) { Mix_PauseMusic(); menu_s.play(); SDL_Delay(200); Mix_ResumeMusic(); next_state = GameStates::Quit; }
                }
                y += h + 10;
            }
        }
    }
}

void Menu::render() {
    win.clear();
    background.render(0, 0);
    int y = 200;
    for (size_t i = 0; i < options.size(); ++i) {
        int x = (win.getWidth() - options[i]->getWidth()) / 2;
        options[i]->render(x, y);
        y += options[i]->getHeight() + 10;
    }
    win.update();
}
