#include "game.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Non-blocking keyboard input function
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// Clear the screen - Using more reliable multi-platform screen clearing method
void clearScreen() {
    // Combine multiple screen clearing methods to ensure maximum effectiveness
    
    // Method 1: ANSI escape sequence for screen clearing (works in most terminals)
    std::cout << "\033[2J"; // Clear entire screen
    std::cout << "\033[3J"; // Clear scrollback buffer (in supported terminals)
    std::cout << "\033[1;1H"; // Move cursor to top-left corner
    std::cout.flush(); // Flush output immediately
    
    // Method 2: Output multiple newlines to scroll the screen (universal method)
    for (int i = 0; i < 5; i++) {
        std::cout << std::endl;
    }
    
    // Method 3: Set cursor position again to ensure subsequent output starts from the top
    std::cout << "\033[1;1H" << std::flush;
    
    // Short delay to ensure above methods take effect
    usleep(10000); // 10 milliseconds
}

// Set cursor position
void setCursorPosition(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}

// Set text color
void setConsoleColor(int colorCode) {
    std::cout << "\033[" << colorCode << "m";
}

// Hide cursor
void hideCursor() {
    std::cout << "\033[?25l";
}

// Show cursor
void showCursor() {
    std::cout << "\033[?25h";
}

Game::Game() 
    : track(100),                // Initialize track length to 100
      playerDog('@', 0, true, "Player"),  // Initialize player's dog
      cpuDog1('%', -5, false, "CPU1"),  // Initialize CPU's dog 1, position at -5
      cpuDog2('#', -5, false, "CPU2"), // Initialize CPU's dog 2, position set to same as CPU1
      gameOver(false) {
    
    // Initialize random number generator
    std::random_device rd;
    rng = std::mt19937(rd());
    
    // Initialize last CPU dog move time
    lastCpuMoveTime = std::chrono::steady_clock::now();
}

Game::~Game() {
    // In the destructor, ensure terminal settings are restored
    showCursor();
}

void Game::initialize() {
    // Add debug information to help identify if the program is executed multiple times
    #ifndef NDEBUG
    std::cerr << "DEBUG: Game initialization started..." << std::endl;
    #endif
    
    // Add all dogs to the track
    track.addDog(&playerDog);
    track.addDog(&cpuDog1);
    track.addDog(&cpuDog2);
    
    // Thoroughly clear the screen, ensuring no previous content remains
    std::cout << "\033[2J\033[1;1H\033[3J" << std::flush; // Add \033[3J to clear scrollback buffer
    std::cout.flush();
    system("clear"); // Use system command to clear screen more thoroughly
    
    // Set up terminal
    clearScreen();
    hideCursor();
    
    // Only show the initial animation once to reduce repeated displays
    clearScreen();
    setConsoleColor(33); // Yellow
    
    std::cout << R"(
        .--.--.
       /  ()  \
      |   ^^   |
      \`----'/ 
       `------'  
)" << std::endl;
    
    setConsoleColor(37); // Bright white
    std::cout << R"(
    ╔═══════════════════════════════════╗
    ║          DOG RACE                 ║
    ╚═══════════════════════════════════╝
)" << std::endl;
    
    // Add game instructions
    setConsoleColor(36); // Cyan
    // Display prompt information with alternating colors
    setConsoleColor(32); // Green
    std::cout << R"(
    >>> Press SPACE to start! <<<
)" << std::endl;
    
    setConsoleColor(0); // Restore default color
    
    // Wait for spacebar, but no longer use looping animation
    setConsoleColor(37); // Bright white
    std::cout << "\n     Waiting for SPACE key..." << std::endl;
    setConsoleColor(0); // Restore default color
    
    // Simplified key waiting logic
    while (true) {
        if (kbhit()) {
            char key = getchar();
            if (key == ' ') break;
        }
        usleep(50000); // 50ms
    }
    
    // Clear the screen again to ensure a clean interface before the game starts
    clearScreen();
}

int Game::getRandomSteps(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

void Game::handleInput() {
    if (kbhit()) {
        char key = getchar();
        if (key == ' ') {
            movePlayer();
        }
    }
}

void Game::movePlayer() {
    // Calculate the position difference between player and CPU dogs
    int diffWithCpu1 = playerDog.getPosition() - cpuDog1.getPosition();
    int diffWithCpu2 = playerDog.getPosition() - cpuDog2.getPosition();
    
    // Adjust player movement steps based on position difference
    if (diffWithCpu1 > 15 && diffWithCpu2 > 15) {
        // If player is too far ahead, slightly reduce movement steps
        playerDog.move(getRandomSteps(1, 2));
    } else if ((diffWithCpu1 < -15 || diffWithCpu2 < -15) && 
              (playerDog.getPosition() < track.getLength() * 0.7)) {
        // If player is too far behind and not near the finish line, slightly increase movement steps
        // But only provide this "catch-up" mechanism in the first 70% of the race
        playerDog.move(getRandomSteps(2, 4));
    } else {
        // Normal case
        playerDog.move(getRandomSteps(1, 3));
    }
}

void Game::updateCpuDogs() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - lastCpuMoveTime).count();
    
    // CPU dogs move every 500 milliseconds (0.5 seconds)
    if (elapsedTime >= 500) {
        // Use the same random step count for both CPU dogs to ensure consistent movement speed
        int sharedSteps = getRandomSteps(1, 2);
        
        // Both CPU dogs advance by the same number of steps
        cpuDog1.move(sharedSteps);
        cpuDog2.move(sharedSteps);
        
        // Update last move time
        lastCpuMoveTime = currentTime;
    }
}

bool Game::isGameOver() const {
    return gameOver;
}

Dog* Game::getWinner() {
    return track.getWinner();
}

void Game::run() {
    // Thoroughly clear the screen again before starting the main game loop
    std::cout << "\033[2J\033[1;1H\033[3J" << std::flush; // Clear screen and scrollback buffer
    std::cout.flush();
    #ifndef _WIN32
    system("clear"); // Use system clear command in non-Windows environments
    #endif
    clearScreen(); // Use our own clear screen function
    
    // Prepare the game main loop
    auto startTime = std::chrono::steady_clock::now();
    
    while (!gameOver) {
        // Clear screen at the start of each loop to ensure a clean interface
        // clearScreen(); // Remove this line because Track::render() already includes screen clearing
        
        handleInput();
        updateCpuDogs();
        track.render();
        
        if (track.isRaceFinished()) {
            gameOver = true;
            Dog* winner = getWinner();
            auto endTime = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            double seconds = duration.count() / 1000.0;
            
            // Wait a short time to let the player see the final track state
            usleep(1000000); // 1 second
            
            // Display the ending screen
            clearScreen();
            showCursor(); // Restore cursor at the end
            
            if (winner && winner->isPlayerControlled()) {
                // Victory screen
                setConsoleColor(32); // Green
                std::cout << R"(
    ╔═══════════════════════════════════════════════════╗
    ║                                                   ║
    ║                                                   ║
                   ___________ 
                  '._==_==_=_.'
                  .-\:      /-.
                 | (|:.     |) |
                  '-|:.     |-'
                    \::.    /
                     '::. .'
                       ) (
                     _.' '._
                    `-------`
    ║                                                   ║
    ║                                                   ║
    ╚═══════════════════════════════════════════════════╝
)" << std::endl;

                setConsoleColor(33); // Yellow
                std::cout << R"(
         ✨ CONGRATULATIONS! ✨
)" << std::endl;
                
                setConsoleColor(37); // Bright white
                std::cout << "    Your dog finished in 1st place!\n" << std::endl;
                std::cout << "    Time: " << std::fixed << std::setprecision(2) << seconds << " seconds\n" << std::endl;
            } else {
                // Defeat screen
                setConsoleColor(31); // Red
                std::cout << R"(
    ╔═══════════════════════════════════════════════════╗
    ║                                                   ║
    ║                                                   ║
                    .--.
                   |o_o |
                   |:_/ |
                  //   \ \
                 (|     | )
                /'\_   _/`\
                \___)=(___/
    ║                                                   ║
    ║                                                   ║
    ╚═══════════════════════════════════════════════════╝
)" << std::endl;

                setConsoleColor(33); // Yellow
                std::cout << R"(
         😢 Better luck next time! 😢
)" << std::endl;
                
                setConsoleColor(37); // Bright white
                std::cout << "    You finished in 3rd place...\n" << std::endl;
            }
            
            setConsoleColor(0); // Restore default color
            std::cout << "    Press any key to exit..." << std::endl;
            getchar();
        }
        
        // Short delay between loops to control game speed
        // Approximately 30ms delay, corresponding to about 33FPS
        // usleep(30000);
        // Increase delay to 100ms (10FPS) to reduce refresh frequency and improve flickering issues
        usleep(100000);
    }
} 