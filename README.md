# ShotChallenge Plugin

ShotChallenge is a simple "multiplayer" practice plugin designed to enhance training sessions through friendly competition. It's great for those looking to improve their skills with structured and varied practice.

* [Features](#features)
* [Getting Started](#getting-started)
* [Configuration & Usage](#configuration--usage)
* [Base Shot Lists](#base-shot-lists)
* [Multiplayer](#multiplayer)

## Features

* **Shuffled Shots**: Present a different list of shots (from the base list) each game
* **Navigation**: Cycle through shuffled shots using F9 and F11
* **Customisable Shot Lists**: Create a local file with your custom shot list and point the plugin to it via settings
* **Score Tracking**: Keep track of scores through chat commands. Typing ++ will increment your score, and -- will decrement it, updating the scoreboard accordingly

## Getting Started

To install the plugin, download it from the plugins directory (the `.dll` file) and place it in the BakkesMod plugins directory.

Alternatively, building from source should just be a case of downloading the project, opening it in VS, and building it (e.g. `ctrl + b`). If for whatever reason this is not the case, I'd be happy to assist.

#### Configuration & Usage

Base shot list, and number of shots to show at a time, are configurable through the plugin settings (In-game: F2 -> Plugins -> ShotChallenge)

Once installed, and in-game, cycle through the list of shots using F9 and F11. 

Scoreboards are updated through the use of chat messages:

| Chat Message  | Result |
| ------------- | ------------- |
| ++  | Increments your score by 1  |
| --  | Decrements your score by 1  |
| sc reset  | Resets the scoreboard  |

Requires chat to be visible.

## Base Shot Lists

A base shot list is all of the shots that the plugin will work with; it is what will be shuffled and truncated (per configuration). The plugin features a default shot list, but this can be overridden by pointing the plugin at a different list (e.g. maybe you want easier/harder shots, maybe you want a list that is flicks only, etc.). To create a custom list, simply create a JSON file and fill it like so:

```json
[
  "45 degree flick faster than 100 kph",
  "Breezi flick",
  "..."
]
```

There isn't really a restriction for what a "shot" can be, you might want a regular shot (e.g. "Breezi flick") a shot with nested challenges (e.g. "Kuxir pinch with 0 bounces faster than 100kph"), or not even a shot at all (e.g. "carry the ball for 30 seconds").

## Multiplayer

While this is intended for multiple people to play together, there is no direct plugin-to-plugin communication happening.

For players to see the same shuffled shots, they must ensure the same base list is being used by all players. If using custom base shot lists, the JSON file being used must be shared with the other players, and they must update their plugin settings to point to this file.
