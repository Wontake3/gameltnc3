#ifndef DEFS_H
#define DEFS_H

#define WINDOW_TITLE "Creepy Bird"
#define WINDOW_ICON_PATH "./flappy_assets/creepybick3.png"
#define INTRO_IMAGE_PATH "./flappy_assets/backgorund640.jpg"
#define BACKGROUND_INTRO "./flappy_assets/foggy640.jpg"
#define BACKGROUND_LEVEL "./flappy_assets/daylight640.jpg"
#define BACKGROUND_GAMEOVER "flappy_assets/mark640.jpg"
#define BACKGROUND_HIGHSCORE "./flappy_assets/haiscore640.jpg"
#define BACKGROUND_DAY "./flappy_assets/daylight640.jpg"
#define BACKGROUND_NIGHT "./flappy_assets/nightlight640.jpg"
#define LEVEL1_BIRD_IMAGE "./flappy_assets/darkbird40.png"
#define LEVEL1_BG_IMAGE "./flappy_assets/darkforest170.jpg"
#define LEVEL1_PIPE_IMAGE "./flappy_assets/horrorpipe34.png"
#define LEVEL1_MUSIC "./flappy_assets/music2.mp3"
#define LEVEL2_BIRD_IMAGE "./flappy_assets/bird170.png"
#define LEVEL2_BG_IMAGE "./flappy_assets/darkforest270.jpg"
#define LEVEL2_PIPE_IMAGE "./flappy_assets/horrorpipe4803.png"
#define LEVEL2_MUSIC "./flappy_assets/music1.mp3"
#define PUMPKIN_IMAGE "./flappy_assets/pumpkin90.png"
#define FONT_PATH "./flappy_assets/bloodcrow.ttf"
#define MENU_FONT_PATH "./flappy_assets/facefears.ttf"
#define MUSIC_THEME_PATH "./flappy_assets/music3.mp3"
#define SOUND_JUMP "./flappy_assets/mario2.wav"
#define SOUND_LOST "./flappy_assets/lose1.wav"
#define SOUND_TRANSITION "./flappy_assets/changinglevelsound1.wav"
#define SOUND_FALL "./flappy_assets/outrosound4.wav"
#define SOUND_MENU "./flappy_assets/clicksound.wav"
#define PIPE_IMAGE "./flappy_assets/horrorpipe2.png"
#define BIRD_IMAGE "./flappy_assets/bird170.png"
#define HIGHSCORE_FILE "highscores.txt"
#define TEXT_SIZE_BIG 30
#define TEXT_SIZE_SMALL 16

enum class LevelType { Level1, Level2 };

enum class GameStates {
    Menu,
    Level,
    Lost,
    HighScore,
    Quit,
    Null
};

#endif
