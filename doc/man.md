tmines(1) -- Terminate Mines, a terminal minesweeper clone
==========================================================

## SYNOPSIS
`tmines` [_OPTION_...]

## DESCRIPTION
tmines allow you to play minesweeper on the terminal.

Use these Options to configure the game:


### Field Size
note: use either fullscreen or specify size explicitly (not both)

- **-f**, **--fullscreen**:
    use full screen size
- **-h**, **-y**, **--height**=_HEIGHT_:
    height of the minefield, default: 10
- **-w**, **-x**, **--width**=_WIDTH_:
    width of the minefield, default: 10

### Mines
note: use either mine density or specify mine count explicitly (not both)

- **-c**, **--mine-count**=_NUM_:
    number of mines to be placed
- **-d**, **--mine-density**=_PERCENTAGE_:
    density of mines to be placed, default: 16%.  
    Specify as number between 0 and 100 (for
    percentages), omit % charater

### Other
- **-a**, **--autodiscover-only**:
    if enabled: fields can only be opened using
    autodiscover feature
- **-s**, **--seed**=_SEED_:
    seed for field generation, suitable seed will be
    chosen automatically  
- **-?**, **--help**:
    Give a help list
- **--usage**:
    Give a short usage message
- **-V**, **--version**:
    Print program version

### Controls
- **Movement**:
    WASD, Arrow Keys, vimlike (HJKL)
- **Flag Mine**:
    F
- **Open Field**:
    Space
- **Quit**:
    Q

### General Rules
A set number of mines is placed at random on the minefield. You can open single spots to see how many mines are sorrounding that spot. This number can go up to 8 in the worst case, in the best case it is 0.

Your job if to open all spots that are not mines. If you open a mine, you loose the game.

Flags can be placed to mark a spot where a mine is located. Flagged spots can't be opened accidentally by clicking.

### Autodiscover
Suppose a setup like this:

```
* * * *
* * ? 2
* *[1]1
* * 2 
```

When you then click the mine `[1]`, the obvious fields not containing any mines will be opened:

```
* * * *
* 2 ? 2
* 1 1 1
* 1 2 
```

When using **--autodiscover-only**, the only way to open mines is in this way.
While **--autodiscover-only** is active, trying to directly open a mine `*` will not work and be ignored by the program.

## REPORTING BUGS
Report bugs to tropf@posteo.de.

When a Crash happens, the file `tmines.crash` will be written into your current directory.
Please include that file in your crash report.

It contains useful information to reproduce and fix the problem.

Note: The crash report contains a list of pressed keys. These are anonymized, meaning WASD gets converted to hjkl, and only keypresses relevant to the program are recorded.

## AUTHORS

## COPYRIGHT
