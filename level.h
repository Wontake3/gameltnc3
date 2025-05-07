#ifndef LEVEL_H
#define LEVEL_H

#include "defs.h"
#include "background.h"
#include "player.h"
#include "music.h"
#include "pipe.h"
#include "texture.h"
#include "font.h"
#include "gamestate.h"
#include <SDL.h>
#include <array>
#include <random>
#include "pumpkin.h"


class MainWindow;

class Level {
    const MainWindow& win;
    Background background;
    Player pl;
    Sound jump_s;
    Sound lost_s;
    Sound transition_s;
    Sound fall_s;
    std::array<Pipe, 3> pipes;
    Font font;
    Texture title;
    Texture pipeTexture;
    Texture birdTexture;
    Texture pumpkinTexture;
    Music* music;
    Pumpkin pumpkin;
    int pipeCounter = 0;
    int skipPipes = 0;
    LevelType currentLevel = LevelType::Level1;
    Uint32 levelStartTime = 0;
    bool inTransition = false;
    Uint32 transitionStartTime = 0;
    Uint32 levelDuration = 15000;
    static constexpr Uint32 TRANSITION_DURATION = 1000;
    int fadeAlpha = 0;
    std::random_device seeder;
    std::mt19937 engine;
    std::uniform_int_distribution<int> dist;
public:
    Level(const MainWindow&);
    void handle_events();
    void logic();
    void render();
    GameStates next_state = GameStates::Null;
    int score = 0;
    ~Level();
private:
    inline bool hit_bottom();
    inline bool hit_roof();
    inline void check_bounds();
    inline void check_all_collisions();
    inline void check_score();
    void switchLevel();
    void startTransition();
    void handleTransition();
    void spawnPumpkin(int pipeX, int gapY);
    void checkPumpkinCollision();
};

#endif
