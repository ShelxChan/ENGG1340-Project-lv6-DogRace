#ifndef DOG_H
#define DOG_H

#include <string>

class Dog {
private:
    char symbol;         // Dog's symbol, such as @, %, #
    int position;        // Position on the track
    bool isPlayer;       // Whether it's player-controlled
    std::string name;    // Dog's name

public:
    Dog(char symbol, int initialPosition, bool isPlayer, const std::string& name);
    
    // Get the dog's symbol
    char getSymbol() const;
    
    // Get the dog's position
    int getPosition() const;
    
    // Get the dog's name
    std::string getName() const;
    
    // Check if it's player-controlled
    bool isPlayerControlled() const;
    
    // Move the dog
    void move(int steps);
};

#endif // DOG_H 