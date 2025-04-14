#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include <chrono>
#include <ctime>
#include "dog.h"
#include "track.h"

class Game {
private:
    Track track;                 // Track
    Dog playerDog;               // Player's dog
    Dog cpuDog1;                 // CPU's dog 1
    Dog cpuDog2;                 // CPU's dog 2
    bool gameOver;               // Whether the game is over
    
    std::mt19937 rng;            // Random number generator
    
    // Last time the CPU dogs moved
    std::chrono::time_point<std::chrono::steady_clock> lastCpuMoveTime;
    
    // Get random movement steps
    int getRandomSteps(int min, int max);
    
    // Handle player input
    void handleInput();
    
    // Update CPU dog positions
    void updateCpuDogs();
    
public:
    // Constructor
    Game();
    
    // Destructor
    ~Game();
    
    // Initialize the game
    void initialize();
    
    // Game main loop
    void run();
    
    // Player moves on space press
    void movePlayer();
    
    // Check if the game is over
    bool isGameOver() const;
    
    // Get the winner
    Dog* getWinner();
};

#endif // GAME_H 