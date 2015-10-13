#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const char HUMAN_PLAYER = 'h';
const char MINIMAX_PLAYER = 'm';
const char RANDOM_PLAYER = 'r';

class GameState {
    private:
        
        bool isP1sTurn;
        bool catsGame;
        bool p1Victory;
        bool p2Victory;
        char p1; // 'h' for human player 1, 'm' for minimax, 'r' for random
        char p2; // 'h' for human player 2, 'm' for minimax, 'r' for random
        char board[9]; // Game board: " " for no move, "X" for X, "O" for O
        int utility;
        int optimalNextMove;

        std::set<GameState*> successors;
        
        std::set<int> remainingMoves;

    public:

        // Initializes the game state
        GameState(char p1Arg, char p2Arg);

        // Generates a new successor GameState by cloning its parent and apply move
        GameState *successor(GameState *parent, int move);

        // Calculates all successors for this state, storing them in successors
        void updateUtility();


        // Returns true if the specified move is valid
        // Returns false if the move is out of bounds or already taken
        bool isValidMove(int move);

        // Returns true if the game is over, false otherwise
        bool gameOver();

        // TODO: getWinner() function for finding out who won?

        // Updates the game state by making the given move
        void makeMove(int move);

        // Calculates and executes the next move in the game
        // Queries a move from the user if required.
        void nextMove();

        // Updates the screen
        void updateScreen();

        // Queries the human player for a valid move
        int humanMove();

        // Determines a valid move at random
        int randomMove();

        // Uses minimax to select an optimal move
        int minimaxMove();
};

