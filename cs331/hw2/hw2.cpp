#include <iostream>
#include "GameState.h"

int main(int argc, char *args[]) 
{

    if(argc != 3) {
        std::cout << "Requires two arguments: p1_mode p2_mode\n";
        std::cout << "Where mode can be: human, random, minimax\n";
        return 1;
    }

    // Initialize game state from command line arguments
    GameState state(args[1][0], args[2][0]);

    while (state.gameOver() == false) {
        state.updateScreen();
        state.nextMove();
    }

    state.updateScreen();


    return 0;
}
