
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL.h>

namespace GameState {

    extern bool isDay;

    extern Uint32 lastSwitchTime;

    extern bool justSwitched;
    void updateDayNight();
}

#endif
