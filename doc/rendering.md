# Rendering
The `display` class is responsible for the rendering process. To implement that, it accesses the `minefield` class via its public methods.

The minefield gets rendered internally into a two-dimensional array of chars. From that, the positions that changed since the last drawing to screen are found an only the chars that have to be redrawn, get redrawn in order to keep rendering times low.

For reasons of spacing, only every other column on the terminal is used, so the displayed text doesn't get bunched up and hard to read. This still doesn't ensure a perfect 1:1 ratio of width and height (for a quadratic minefield), but is far better than pushing everything together and use every available column.

The following Symbols are rendered (during game):

| color pair  | foreground color    | background color  | character | meaning                                   |
| ----------- | ------------------- | ----------------- | --------- | ----------------------------------------- |
| `10`        | white               | black             | `*`       | unopened field                            |
| `0`         | white               | black             | ` ` Space | opened field, no sorrounding Mines        |
| `1`         | blue                | black             | `1`       | 1 sorrounding mines                       |
| `2`         | green               | black             | `2`       | 2 sorrounding mines                       |
| `3`         | red                 | black             | `3`       | 3 sorrounding mines                       |
| `4`         | cyan                | black             | `4`       | 4 sorrounding mines                       |
| `5`         | red                 | black             | `5`       | 5 sorrounding mines                       |
| `6`         | cyan                | black             | `6`       | 6 sorrounding mines                       |
| `7`         | magenta             | black             | `7`       | 7 sorrounding mines                       |
| `8`         | cyan                | black             | `8`       | 8 sorrounding mines                       |
| `0`         | white               | black             | `?`       | flag placed                               |

After the game ended, additional meaning is implied:

| color pair  | foreground color    | background color  | character | meaning                                   |
| ----------- | ------------------- | ----------------- | --------- | ----------------------------------------- |
| `0`         | white               | black             | `?`       | flag marks a mine correctly               |
| `11`        | white               | red               | `?`       | flag placed, but no mine at this position |
| `11`        | white               | red               | `X`       | mine here (when no flag is placed)        |

Additionally, the message describing the current game state also is color coded.

| color pair  | foreground color  | background color  | state   |
| ----------- | ----------------- | ----------------- | ------- |
| `12`        | yellow            | black             | running |
| `13`        | green             | black             | won     |
| `11`        | white             | red               | lost    |

> 'Color pair' is a concept used by `curses`, the library used to write to the console.
> When starting the game, these numbers are associated with their corresponding foreground and background color.
> On printing text with color, instead of specifying foreground and background color, a color pair number is given.
> Therefore the color pair number here is to be used as an internal reference.

For simplicity only the standard colors (blue, green, magenta...) are used instead of specifying RGB values. Even though RGB-mode is supported by `curses`, it is a giant hassle to get both RGB and standard colors working (as some terminals don't support RGB).
