#include "dog.h"

Dog::Dog(char symbol, int initialPosition, bool isPlayer, const std::string& name) 
    : symbol(symbol), position(initialPosition), isPlayer(isPlayer), name(name) {
}

char Dog::getSymbol() const {
    return symbol;
}

int Dog::getPosition() const {
    return position;
}

std::string Dog::getName() const {
    return name;
}

bool Dog::isPlayerControlled() const {
    return isPlayer;
}

void Dog::move(int steps) {
    position += steps;
} 