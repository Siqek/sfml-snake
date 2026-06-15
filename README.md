# SFML Snake

A desktop Snake game built in C++ using the SFML library, featuring a multithreaded architecture and configurable gameplay.



## Screenshots

### Main Menu
![main-menu](/docs/assets/main-menu.png)

### Grid Settings
![grid-settings](/docs/assets/grid-settings.png)

### Game Instructions
![game-instructions](/docs/assets/game-instructions.png)

### Game
![game](/docs/assets/game.png)

### Pause Menu
![pause-menu](/docs/assets/pause-menu.png)

### Game Over
![game-over](/docs/assets/game-over.png)

### Settings
![game-settings](/docs/assets/game-settings.png)



## Features

- **Main menu** — start the game, adjust settings, or exit
- **Game settings** — configure grid size, snake speed, and number of apples
- **Grid types**
  - Rectangular — classic bounded grid
  - Rectangular Donut — rectangular grid with a hole in the center
- **In-game settings** — settings accessible directly during gameplay before starting a round
- **Pause menu** — pause, restart, or exit the game at any time



## Architecture

The application uses a multithreaded design:
- **Main thread** — game simulation and event handling
- **Render thread** — dedicated rendering loop



## Tech Stack

C++, SFML



## Roadmap

- [ ] How to build — setup guide
- [ ] Multiple snakes with unique abilities (e.g. teleporting snake)
- [ ] Animations
- [ ] Sound effects
- [ ] Graphics settings
- [ ] Migration from Premake5 to CMake (with FetchContent for automatic dependency management)
