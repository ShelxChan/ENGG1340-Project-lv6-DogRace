#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
// Add platform detection headers
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include "game.h"

// Simple program mutex mechanism
bool acquireLock() {
    #ifdef _WIN32
    // Windows platform uses named mutex
    HANDLE hMutex = CreateMutex(NULL, TRUE, "DogRaceGameMutex");
    if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS) {
        if (hMutex) {
            CloseHandle(hMutex);
        }
        return false;
    }
    return true;
    #else
    // Linux/Unix platform uses file lock
    int fd = open("/tmp/dograce.lock", O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        return false;
    }
    if (flock(fd, LOCK_EX | LOCK_NB) < 0) {
        close(fd);
        return false;
    }
    // Don't close fd to maintain the lock, OS will automatically close it when program exits
    return true;
    #endif
}

int main() {
    // Ensure only one game instance is running
    if (!acquireLock()) {
        std::cerr << "Error: Game is already running!" << std::endl;
        return 1;
    }
    
    // Set up UTF-8 display (Windows environment only)
    #ifdef _WIN32
    system("chcp 65001");
    #endif
    
    // Initialize random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Create game instance
    Game dogRace;
    
    // Initialize game
    dogRace.initialize();
    
    // Run game
    dogRace.run();
    
    return 0;
} 