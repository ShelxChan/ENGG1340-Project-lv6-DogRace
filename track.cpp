#include "track.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>

Track::Track(int length) : length(length) {
}

Track::~Track() {
    // No need to delete pointers in dogs, they're managed by the Game class
}

void Track::addDog(Dog* dog) {
    dogs.push_back(dog);
}

int Track::getLength() const {
    return length;
}

bool Track::isRaceFinished() const {
    for (const auto& dog : dogs) {
        if (dog->getPosition() >= length) {
            return true;
        }
    }
    return false;
}

Dog* Track::getWinner() const {
    for (const auto& dog : dogs) {
        if (dog->getPosition() >= length) {
            return dog;
        }
    }
    return nullptr;
}

const std::vector<Dog*>& Track::getDogs() const {
    return dogs;
}

void Track::render() const {
    // Only move cursor to the top-left corner, do not clear screen content
    // Since Game::run() already has clearScreen() operation, no need to repeat screen clearing here
    // Removed screen clearing command: std::cout << "\033[2J";
    std::cout << "\033[1;1H" << std::flush; // Move cursor to top-left corner
    
    // Display game title
    std::cout << "\033[33m"; // Yellow
    std::cout << "╔═══════════════════════ DOG RACE ════════════════════════╗" << std::endl;
    std::cout << "\033[37m"; // White
    
    // Get ranking information
    auto ranking = getRanking();
    
    // Display positions and rankings of all dogs
    std::cout << "\033[36m"; // Cyan
    std::cout << "║  ";
    
    // Display player dog information
    for (const auto& dog : dogs) {
        if (dog->isPlayerControlled()) {
            std::cout << "You(@): " << std::setw(3) << dog->getPosition() << "/" << length;
            
            // Display player ranking
            for (size_t i = 0; i < ranking.size(); ++i) {
                if (ranking[i]->isPlayerControlled()) {
                    std::cout << " [" << (i + 1) << "st]";
                    break;
                }
            }
            break;
        }
    }
    
    // Fill with spaces for layout alignment
    std::cout << std::string(26, ' ') << "║" << std::endl;
    
    // Display CPU dog information
    std::cout << "║  ";
    bool firstCpu = true;
    for (const auto& dog : dogs) {
        if (!dog->isPlayerControlled()) {
            if (!firstCpu) {
                std::cout << " | ";
            }
            std::cout << dog->getName() << "(" << dog->getSymbol() << "): " << std::setw(3) << dog->getPosition();
            firstCpu = false;
        }
    }
    
    // Fill with spaces for layout alignment
    std::cout << std::string(26, ' ') << "║" << std::endl;
    
    std::cout << "\033[37m"; // Reset to white
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    
    // Draw track area
    for (const auto& dog : dogs) {
        int pos = dog->getPosition();
        std::string trackBody(length, ' '); // Track body
        
        // Draw track background
        for (int i = 0; i < length; i += 4) {
            trackBody[i] = '.';
        }
        
        // Place dog symbol on the track
        if (pos >= 0 && pos < length) {
            trackBody[pos] = dog->getSymbol();
        }
        
        // Dog color
        std::cout << "║ ";
        if (dog->getSymbol() == '@') {
            std::cout << "\033[32m"; // Green for player dog
        } else if (dog->getSymbol() == '%') {
            std::cout << "\033[31m"; // Red for CPU1 dog
        } else {
            std::cout << "\033[34m"; // Blue for CPU2 dog
        }
        
        // Print track
        std::cout << trackBody;
        
        // Finish line
        std::cout << "\033[37m"; // Bright white
        std::cout << "║";
        std::cout << "\033[33m"; // Yellow
        std::cout << "▌▌";
        std::cout << "\033[37m"; // Reset to white
        std::cout << " ║" << std::endl;
    }
    
    // Draw bottom border
    std::cout << "\033[33m"; // Yellow
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;
    
    // Prompt information
    std::cout << "\033[37m"; // Bright white
    std::cout << "  Press SPACE to make your dog (@) move forward!" << std::endl;
    std::cout << "\033[0m"; // Reset all attributes
}

std::vector<Dog*> Track::getRanking() const {
    std::vector<Dog*> ranking = dogs;
    std::sort(ranking.begin(), ranking.end(), [](const Dog* a, const Dog* b) {
        return a->getPosition() > b->getPosition();
    });
    return ranking;
} 