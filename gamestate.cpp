#include "gamestate.h"
#include <SDL.h>

namespace GameState {
    bool isDay = true;
    Uint32 lastSwitchTime = 0;
    bool justSwitched = false;
    const Uint32 switchInterval = 5000;
    void updateDayNight() {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastSwitchTime >= switchInterval) {
            isDay = !isDay;
            lastSwitchTime = currentTime;
            justSwitched= true;
        } else { justSwitched = false;
        }
    }
}
