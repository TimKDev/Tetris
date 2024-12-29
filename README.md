# Tetris Clone

A classic Tetris implementation built with GTK4, created as part of the [Coding Challenges](https://codingchallenges.fyi/challenges/challenge-tetris/) project.

## Features
- Classic Tetris gameplay mechanics
- Score tracking and high score system
- Next piece preview
- Pause/Resume functionality
- Player name customization

## Controls
- `Left` and `Right` arrows: Move the piece left and right
- `Down` arrow: Soft drop the piece
- `Up` arrow: Rotate the piece clockwise

## Prerequisites
### Ubuntu/Debian 
`sudo apt install -y build-essential libgtk-4-dev libjson-c-dev`

### Arch Linux
`sudo pacman -S base-devel gtk4 json-c`

## Build
`make`

## Run
`./tetris`
