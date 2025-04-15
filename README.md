# Dog Race Game

This is a text-based racing game. The player controls their dog (represented by "@") and competes in a race against two computer-controlled dogs (represented by "%" and "#").

## Game Rules

- The player moves their dog forward by repeatedly pressing the spacebar
- Each press of the spacebar advances the player's dog by a random 1-3 steps
- CPU-controlled dogs automatically advance 1-2 steps every 0.5 seconds
- The race track is 100 character units long
- The first dog to reach the finish line wins

## Controls

- Spacebar: Move the player's dog forward
- Any key: Start game/End game

## Compilation and Running

### Using Makefile (Recommended)

The game now supports cross-platform compilation and execution:

```bash
# Compile for any platform
make

# Run (cross-platform, automatically detects your OS)
make run

# Run on Windows specifically
make run-win

# Run on Linux/Unix specifically
make run-unix
```

### Troubleshooting

If you encounter the "chcp: not found" error on Linux/Unix, this is normal and can be ignored. The game has been updated to handle this gracefully.

If you see duplicate game screens when running on Linux/Unix, try using the specific platform command:
```bash
make run-unix
```

### Manual Compilation

#### Windows
```bash
g++ -o dograce.exe main.cpp game.cpp dog.cpp track.cpp -std=c++11
.\dograce.exe
```

#### Linux/Mac
```bash
g++ -o dograce main.cpp game.cpp dog.cpp track.cpp -std=c++11
./dograce
```

## File Structure

- `main.cpp` - Main program entry
- `game.h` and `game.cpp` - Main game logic
- `dog.h` and `dog.cpp` - Dog character class definition and implementation
- `track.h` and `track.cpp` - Track display and management
- `Makefile` - Project compilation script
- `README.md` - Project documentation file

## Author
- [Shelx] - 5th version 
