#include <vector>
#include <fstream>
std::vector<int> playerScores;

#include <bits/stdc++.h>
#include "menu.h"
#include "mainwindow.h"
#include "defs.h"
#include "intro.h"
#include "level.h"
#include "music.h"
#include <SDL.h>
#include <memory>
#include <algorithm>

GameStates showGameOver(MainWindow& win, int score);

void loadPlayerScores() {
    playerScores.clear();
    std::ifstream fin(HIGHSCORE_FILE);
    int score;
    while (fin >> score) {
        playerScores.push_back(score);
    }
    fin.close();
}

void savePlayerScores() {
    std::ofstream fout(HIGHSCORE_FILE);
    for (int score : playerScores) {
        fout << score << std::endl;
    }
    fout.close();
}

GameStates doMenu(MainWindow& win) {
    Menu menu(win);
    GameStates next_state = GameStates::Null;
    while (next_state == GameStates::Null) {
        menu.render();
        menu.handle_events(next_state);
        SDL_Delay(16);
    }
    return next_state;
}

GameStates doHighScore(MainWindow& win) {
    Texture background(win.getRenderer());
    background.loadFromFile(BACKGROUND_HIGHSCORE);

    Font f;
    f.loadFromFile(FONT_PATH, TEXT_SIZE_BIG);

    std::vector<int> highscores = {100};
    for (int score : playerScores) highscores.push_back(score);
    while (highscores.size() < 5) highscores.push_back(0);

    std::vector<std::unique_ptr<Texture>> lines;
    for (int i = 0; i < 5; ++i) {
        std::string text = "Top " + std::to_string(i+1) + ": ";
        std::string mark = std::to_string(highscores[i]);
        auto t = std::make_unique<Texture>(win.getRenderer());
        t->loadFromRenderedText(text, f, SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
        auto tmark = std::make_unique<Texture>(win.getRenderer());
        tmark->loadFromRenderedText(mark, f, SDL_Color{0xFF, 0, 0, 0xFF});
        lines.push_back(std::move(t));
        lines.push_back(std::move(tmark));
    }

    SDL_Event e;
    while (true) {
        win.clear();
        background.render(0, 0);

        int y = 120;
        for (size_t i = 0; i < lines.size(); i += 2) {
            int x = (win.getWidth() - (lines[i]->getWidth() + lines[i+1]->getWidth())) / 2;
            lines[i]->render(x, y);
            lines[i+1]->render(x + lines[i]->getWidth(), y);
            y += lines[i]->getHeight() + 20;
        }
        win.update();

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return GameStates::Quit;
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_FINGERDOWN)
                return GameStates::Menu;
        }
        SDL_Delay(16);
    }
}

GameStates doLevel(MainWindow& win) {
    Level level(win);

    while (level.next_state == GameStates::Null) {
        level.handle_events();
        level.logic();
        level.render();
    }

    if (level.next_state == GameStates::Lost) {
        playerScores.push_back(level.score);
        std::sort(playerScores.begin(), playerScores.end(), std::greater<int>());
        if (playerScores.size() > 4) playerScores.resize(4);
        savePlayerScores();

        return showGameOver(win, level.score);
    }
    return level.next_state;
}

GameStates showGameOver(MainWindow& win, int score) {
    Texture back(win.getRenderer()), t(win.getRenderer());
    back.loadFromFile(BACKGROUND_GAMEOVER);
    Font f;
    f.loadFromFile(FONT_PATH, TEXT_SIZE_BIG);
    t.loadFromRenderedText("YOU LOST!!! Score: " + std::to_string(score), f, SDL_Color{0xFF,0,0,0xFF});

    Uint32 start = SDL_GetTicks();
    bool proceed = false;
    SDL_Event e;
    while (!proceed && SDL_GetTicks() - start < 5000) {
        win.clear();
        back.render(0,0);
        t.render((win.getWidth() - t.getWidth()) / 2, ((win.getHeight() - t.getHeight()) / 2));
        win.update();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return GameStates::Quit;
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_FINGERDOWN)
                proceed = true;
        }
        SDL_Delay(16);
    }

    Font btnFont;
    btnFont.loadFromFile(FONT_PATH, 32);
    Texture btnContinue(win.getRenderer()), btnMenu(win.getRenderer());
    btnContinue.loadFromRenderedText("Continue", btnFont, SDL_Color{0xFF,0,0,0xFF});
    btnMenu.loadFromRenderedText("Menu", btnFont, SDL_Color{0xFF,0,0,0xFF});

    int btnWidth = std::max(btnContinue.getWidth(), btnMenu.getWidth());
    int btnHeight = std::max(btnContinue.getHeight(), btnMenu.getHeight());
    int gap = 30;

    int totalWidth = btnWidth * 2 + gap;
    int x1 = (win.getWidth() - totalWidth) / 2;
    int x2 = x1 + btnWidth + gap;
    int y = (win.getHeight() - btnHeight) / 2;

    int selected = 0;

    while (true) {
        win.clear();
        back.render(0,0);

        if (selected == 0) btnContinue.setColor(0xFF, 0xA5, 0x00);
        else btnContinue.setColor(0xFF, 0, 0);
        if (selected == 1) btnMenu.setColor(0xFF, 0xA5, 0x00);
        else btnMenu.setColor(0xFF, 0, 0);

        btnContinue.render(x1 + (btnWidth - btnContinue.getWidth()) / 2, y);
        btnMenu.render(x2 + (btnWidth - btnMenu.getWidth()) / 2, y);

        win.update();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return GameStates::Quit;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) selected = 0;
                if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) selected = 1;
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
                    return selected == 0 ? GameStates::Level : GameStates::Menu;
                }
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x, my = e.button.y;
                if (mx >= x1 && mx <= x1 + btnWidth && my >= y && my <= y + btnHeight)
                    return GameStates::Level;
                if (mx >= x2 && mx <= x2 + btnWidth && my >= y && my <= y + btnHeight)
                    return GameStates::Menu;
            }
        }
        SDL_Delay(16);
    }
}

void showIntroImage(MainWindow& win) {
    Texture intro(win.getRenderer());
    intro.loadFromFile(INTRO_IMAGE_PATH);

    SDL_Event e;
    bool proceed = false;
    while (!proceed) {
        win.clear();
        intro.render((win.getWidth() - intro.getWidth()) / 2, (win.getHeight() - intro.getHeight()) / 2);
        win.update();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_FINGERDOWN)
                proceed = true;
        }
        SDL_Delay(16);
    }
}

int main(int argc, char *argv[])
{
    MainWindow win (640, 480, WINDOW_TITLE);
    win.setIcon(WINDOW_ICON_PATH);
    Music mus {MUSIC_THEME_PATH};
    mus.play();

    showIntroImage(win);

    loadPlayerScores();

    GameStates current_state = GameStates::Menu;

    while (current_state != GameStates::Quit) {
        switch (current_state)
        {
            case GameStates::Menu:
                mus.play();
                current_state = doMenu(win);
                break;
            case GameStates::Level:
                mus.stop();
                current_state = doLevel(win);
                break;
            case GameStates::HighScore:
                current_state = doHighScore(win);
                break;
            default:
                return -1;
        }
    }
    return 0;
}
