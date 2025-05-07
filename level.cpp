#include "level.h"
#include "mainwindow.h"
#include "defs.h"
#include "gamestate.h"
#include "music.h"
#include <SDL_mixer.h>

#include <cmath>
#include <string>

#define SCROLLING_SPEED 1

bool check_collision(const Player& pl, const SDL_Rect& rec)
{
    int cX, cY;
    if(pl.posX < rec.x) {
        cX = rec.x;
    }
    else if( pl.posX > rec.x + rec.w ) {
        cX = rec.x + rec.w;
    }
    else {
        cX = static_cast<int>(pl.posX);
    }
    if(pl.posY < rec.y ) {
        cY = rec.y;
    }
    else if(pl.posY > rec.y + rec.h) {
        cY = rec.y + rec.h;
    }
    else {
        cY = static_cast<int>(pl.posY);
    }
    if(std::hypot(cX - pl.posX, cY - pl.posY) < pl.radius) {
        return true;
    }
    else return false;
}

bool check_collision(const Player& pl, const Pipe& pip)
{
    return check_collision(pl, pip.get_lower()) || check_collision(pl, pip.get_upper());
}

Level::Level(const MainWindow& _win)
    : win(_win),
      background(_win),
      pl{_win.getRenderer(), win.getWidth() / 4.0, win.getHeight() / 2.0},
      jump_s{SOUND_JUMP},
      lost_s{SOUND_LOST},
      transition_s{SOUND_TRANSITION},
      fall_s{SOUND_FALL},
      font{},
      title{_win.getRenderer()},
      pipeTexture{_win.getRenderer()},
      birdTexture{_win.getRenderer()},
      pumpkinTexture{_win.getRenderer()},
      music(nullptr),
      engine{this->seeder()},
      dist{100, 250}
{
    currentLevel = LevelType::Level1;
    background.loadTextures(LEVEL1_BG_IMAGE, LEVEL2_BG_IMAGE);
    font.loadFromFile(FONT_PATH, 30);
    pipeTexture.loadFromFile(LEVEL1_PIPE_IMAGE);
    birdTexture.loadFromFile(LEVEL1_BIRD_IMAGE);
    printf("Loading pumpkin texture...\n");
    pumpkinTexture.loadFromFile(PUMPKIN_IMAGE);
    printf("Pumpkin texture load attempted.\n");
    Mix_HaltMusic();
    printf("Trying to play: %s\n", LEVEL1_MUSIC);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    music = new Music(LEVEL1_MUSIC);
    music->play();
    jump_s.setVolume(32);
    for (decltype(pipes)::size_type i=0; i<pipes.size(); ++i) {
        pipes[i].init(&pipeTexture, 240);
        pipes[i].set_pos(win.getWidth() + i * (win.getWidth() + pipes[i].get_width()) / pipes.size());
        pipes[i].set_height(dist(engine));
    }
    pipeCounter = 0;
    skipPipes = 0;
    pumpkin.setActive(false);
    spawnPumpkin(win.getWidth() / 2 - 20, win.getHeight() / 2 - 20);
    levelStartTime = SDL_GetTicks();
    inTransition = false;
    levelDuration = 4000;
}

Level::~Level() {
    delete music;
}

void Level::handle_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            next_state = GameStates::Quit;
        }
        else if ((e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_FINGERDOWN) && !pl.isDead && !inTransition) {
            pl.make_jump();
            jump_s.play();
        }
    }
}

void Level::logic()
{
    if (inTransition) {
        handleTransition();
        return;
    }
    Uint32 now = SDL_GetTicks();
    if (now - levelStartTime >= levelDuration) {
        startTransition();
        return;
    }
    GameState::updateDayNight();
    pl.apply_forces();
    check_bounds();
    if (!pl.isDead) check_all_collisions();
    check_score();
    checkPumpkinCollision();
    background.scroll(SCROLLING_SPEED);
    for (auto& pip: pipes) {
        pip.scroll(SCROLLING_SPEED);
        if (pip.get_pos() < 0 - pip.get_width()) {
            pip.set_pos(win.getWidth());
            pip.set_height(dist(engine));
            pip.left_behind = false;
            pipeCounter++;
            if (skipPipes > 0) {
                skipPipes--;
                pip.left_behind = true;
            }
        }
    }
}

void Level::render()
{
    win.clear();
    background.render();
    for (auto& pip: pipes) pip.render();
    if (pumpkin.isActive()) pumpkin.render(win.getRenderer());
    title.loadFromRenderedText("Score: " + std::to_string(score), font, SDL_Color{0xFF,0,0,0xFF});
    title.render(((win.getWidth() - title.getWidth()) / 2), title.getHeight() / 2);
    pl.render(currentLevel == LevelType::Level1 ? &birdTexture : nullptr);
    if (inTransition && fadeAlpha > 0) {
        SDL_SetRenderDrawBlendMode(win.getRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(win.getRenderer(), 0, 0, 0, fadeAlpha);
        SDL_Rect overlay = {0, 0, win.getWidth(), win.getHeight()};
        SDL_RenderFillRect(win.getRenderer(), &overlay);
        SDL_SetRenderDrawBlendMode(win.getRenderer(), SDL_BLENDMODE_NONE);
    }
    win.update();
}

bool Level::hit_bottom()
{
    return pl.posY + pl.radius > win.getHeight();
}

bool Level::hit_roof()
{
    return pl.posY <= pl.radius;
}

void Level::check_bounds()
{
    if (!pl.isDead && hit_roof()) {
        pl.isDead = true;
        lost_s.play();
    }
    else if (hit_bottom()) {
        Mix_HaltMusic();
        next_state = GameStates::Lost;
        fall_s.play();
        if (!pl.isDead) {lost_s.play();}
    }
}

void Level::check_all_collisions()
{
    for (auto& pip: pipes) {
        if (check_collision(pl, pip)) {
            pl.isDead = true;
            lost_s.play();
            break;
        }
    }
}

void Level::check_score()
{
    if (!pl.isDead) {
        for (auto& pip: pipes) {
            if (!pip.left_behind) {
                if (pl.posX > pip.get_pos() + pip.get_width()) {
                    ++score;
                    pip.left_behind = true;
                    break;
                }
            }
        }
    }
}

void Level::switchLevel() {
    if (currentLevel == LevelType::Level1) {
        currentLevel = LevelType::Level2;
        background.loadTextures(LEVEL2_BG_IMAGE, LEVEL1_BG_IMAGE);
        pipeTexture.loadFromFile(LEVEL2_PIPE_IMAGE);
        birdTexture.loadFromFile(LEVEL2_BIRD_IMAGE);
        delete music;
        Mix_HaltMusic();
        printf("Trying to play: %s\n", LEVEL2_MUSIC);
        Mix_VolumeMusic(MIX_MAX_VOLUME);
        music = new Music(LEVEL2_MUSIC);
        levelDuration = 4000;
    } else {
        currentLevel = LevelType::Level1;
        background.loadTextures(LEVEL1_BG_IMAGE, LEVEL2_BG_IMAGE);
        pipeTexture.loadFromFile(LEVEL1_PIPE_IMAGE);
        birdTexture.loadFromFile(LEVEL1_BIRD_IMAGE);
        delete music;
        Mix_HaltMusic();
        printf("Trying to play: %s\n", LEVEL1_MUSIC);
        Mix_VolumeMusic(MIX_MAX_VOLUME);
        music = new Music(LEVEL1_MUSIC);
        levelDuration = 4000;
    }
    music->play();
    for (auto& pip: pipes) pip.setTexture(&pipeTexture);
    levelStartTime = SDL_GetTicks();
}

void Level::startTransition() {
    transition_s.play();
    inTransition = true;
    transitionStartTime = SDL_GetTicks();
}

void Level::handleTransition() {
    Uint32 now = SDL_GetTicks();
    Uint32 elapsed = now - transitionStartTime;
    if (elapsed >= TRANSITION_DURATION) {
        switchLevel();
        inTransition = false;
        fadeAlpha = 0;
        return;
    }
    float half = TRANSITION_DURATION / 2.0f;
    if (elapsed < half) {
        fadeAlpha = static_cast<int>(255.0f * (elapsed / half));
    } else {
        fadeAlpha = static_cast<int>(255.0f * (1.0f - (elapsed - half) / half));
    }
}

void Level::spawnPumpkin(int pipeX, int gapY) {
    printf("Spawning pumpkin at (%d, %d)\n", pipeX, gapY);
    pumpkin.init(&pumpkinTexture, pipeX, gapY);
    printf("Pumpkin active: %d\n", pumpkin.isActive());
}

void Level::checkPumpkinCollision() {
    if (pumpkin.isActive() && !pl.isDead) {
        SDL_Rect pRect = pumpkin.getRect();
        SDL_Rect birdRect = pl.getRect();
        if (SDL_HasIntersection(&pRect, &birdRect)) {
            score += 2;
            skipPipes = 2;
            pumpkin.setActive(false);
        }
    }
}
