# ShotChallenge Plugin

![20240601114845_1](https://github.com/Ty-R/shot-challenge/assets/2464702/4ba69ff0-e01b-4224-b60a-0234198aa151)

ShotChallenge is a simple "multiplayer" practice plugin designed to enhance training sessions through friendly competition. It's great for those looking to improve their skills with structured and varied practice.

* [Features](#features)
* [Getting Started](#getting-started)
  * [Configuration & Usage](#configuration--usage)
* [Base Shot Lists](#base-shot-lists)
* [Multiplayer](#multiplayer)
  * [Issues](#issues)

## Features

* **Shuffled Shots**: Present a different list of shots (from the base list) each game
* **Customisable Shot Lists**: Create your own shot lists to be shuffled/shown in game
* **Score Tracking**: Keep track of scores through chat commands

## Getting Started

To install the plugin, download it from the plugins directory (the `.dll` file) and place it in the BakkesMod plugins directory.

Alternatively, building from source should just be a case of downloading the project, opening it in VS, and building it (e.g. `ctrl + b`).

### Configuration & Usage

Base shot list, and number of shots to show at a time, are configurable through the plugin settings (In-game: F2 -> Plugins -> ShotChallenge)

Once installed, and in-game, cycle through the list of shots using F9 and F11. If these conflict, or are a bit out of the way, then reassign them through BakkesMod bindings.

Scoreboards are visible to all players and are updated through the use of chat messages:

| Chat Message  | Result |
| ------------- | ------------- |
| ++  | Increments your score by 1  |
| --  | Decrements your score by 1  |
| sc reset  | Resets the scoreboard  |

> [!WARNING]  
> Will NOT work if chat is restricted (e.g. team chat)

## Base Shot Lists

A base shot list is all of the shots that the plugin will work with; it is what will be shuffled and truncated (per configuration). The plugin features a default shot list, but this can be overridden by pointing the plugin at a different list (e.g. maybe you want easier/harder shots, maybe you want a list that is flicks only, etc.). To create a custom list, simply create a JSON file and fill it like so:

```json
[
  "45 degree flick faster than 100 kph",
  "Breezi flick",
  "..."
]
```

> [!TIP]
> There isn't really a restriction for what a "shot" can be, you might want a regular shot (e.g. "Breezi flick") a shot with nested challenges (e.g. "Kuxir pinch with 0 bounces faster than 100kph"), or not even a shot at all (e.g. "carry the ball for 30 seconds").

## Multiplayer

While this is intended for multiple people to play together, there is no direct plugin-to-plugin communication happening.

For players to see the same shuffled shots, they must ensure the same base list is being used by all players. If using custom base shot lists, the JSON file being used must be shared with the other players, and they must update their plugin settings to point to this file.

![plugin-settings](https://github.com/Ty-R/shot-challenge/assets/2464702/34642bef-c479-48a6-872d-b80c94cf7852)

### Issues

**Not seeing the same shots as friends**

Likely cause is a different base shot list, double check they are the same for everyone. A second potential cause is a different seed, but the seed is based on the match GUID which _should_ be the same for all players in the lobby. The seed is visible in the settings panel (`21122` in the case of the previous screenshot).

**Shots look like they refresh twice on game start**

Known issue that exists as a result of the implementation - the first list of shots is from the previous game, and the second list is for the current game. I plan on improving this.

If issues persist, or aren't listed here, please create an issue.
