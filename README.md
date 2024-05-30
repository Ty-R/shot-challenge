# Shot Challenge

A basic Bakkesmod plugin intended for practice, and friendly competition. When in freeplay, or a private game, the plugin will shuffle and show a list of shots to try and score. The plugin features some default shots, but you can also create a list file locally and point the plugin at it via the settings.

By default, it shuffles the shots then shows the first 10. This is also configurable through the settings.

The shuffle is determined by a "seed" which itself is based on a rounded timestamp. The idea is that if multiple people had the plugin, and the same base list of shots, when they start a game, they will be given the same shuffled, truncated list to practice/compete with.

A base list of shots is just a JSON array, like such:

```json
[
  "Double reset",
  "Ground pinch",
  "..."
]
```

In order for friends to see the same shots, they must have the same base list, so it'd need to be shared, then configured in the respective plugin settings (F2 -> Plugins -> ShotChallenge)