# ShotChallenge Plugin

ShotChallenge is a simple practice plugin designed to enhance your training sessions by shuffling a list of shot challenges and displaying them on screen. It's great for those looking to improve their skills with structured and varied practice.

* [Features](#features)
* [Getting Started](#getting-started)
* [Configuration & Usage](#configuration--usage)
* [Base Shot Lists](#base-shot-lists)
* [Considerations](#considerations)

## Features

* **Shot Challenge Shuffling**: Randomly shuffles a list of shot challenges and displays them on screen
* **Navigation**: Cycle through the challenges using F9 and F11
* **Customisable Shot Lists**: Create a local file with your custom shot list and point the plugin to it via settings
* **Score Tracking**: Keep track of scores through chat commands. Typing ++ will increment your score, and -- will decrement it, updating the scoreboard accordingly
* **Multiplayer Synchronisation**: Shuffled shots, and scores, are visible to all players in the lobby providing they have the plugin and configuration

## Getting Started

To install the plugin, download it from the plugins directory (the `.dll` file) and place it in the BakkesMod plugins directory.

Alternatively, building from source should just be a case of downloading the project, opening it in VS, and building it (e.g. `ctrl + b`). If for whatever reason this is not the case, I'd be happy to assist.

#### Configuration & Usage

Base shot list, and number of shots to show at a time, are configurable through the plugin settings (In-game: F2 -> Plugins -> ShotChallenge)

Once installed, and in-game, cycle through the list of shots using F9 and F11. Send chat messages to update scoreboards:

* "++" will increment your own score by 1
* "--" will decrement your own score by 1
* "sc reset" will reset everyone's score

Scoreboards will be synced if multiple players are in the same game.

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

## Considerations

There is no separate networking happening for "multiplayer", it is being achieved through a couple of tricks, and may be slightly fiddly as a result.

Seeing the same shuffled list of shots -- this is pseudorandom, and made deterministic through the use of a seed. The seed in this case is a slightly rounded down timestamp taken when the game starts. Since players would start the game at roughly the same time, the plugins will separately get the same seed, so their separate lists are shuffled the same way. A timestamp is used because this allows for a different shuffle across games.

Seeing the same scores -- this is hooking into chat events. When a player sends a specific chat message, this is being picked up by the different plugin instances, so they can separate build the same scoreboard. This will NOT work for chat that is not visible (e.g. team chat)
