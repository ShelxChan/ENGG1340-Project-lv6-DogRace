#ifndef TRACK_H
#define TRACK_H

#include <vector>
#include "dog.h"

class Track {
private:
    const int length;            // Total track length
    std::vector<Dog*> dogs;      // All participating dogs
    
public:
    // Constructor, sets the track length
    Track(int length);
    
    // Destructor doesn't need to delete Dog pointers in dogs, as they'll be managed in the Game class
    ~Track();
    
    // Add a dog to the track
    void addDog(Dog* dog);
    
    // Get the track length
    int getLength() const;
    
    // Check if any dog has finished the race
    bool isRaceFinished() const;
    
    // Get the winning dog (if any)
    Dog* getWinner() const;
    
    // Get all dogs
    const std::vector<Dog*>& getDogs() const;
    
    // Render the track state
    void render() const;
    
    // Get the current ranking of dogs
    std::vector<Dog*> getRanking() const;
};

#endif // TRACK_H 