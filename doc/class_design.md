Class Architecture
==================

Cursed Mines basically consists of three different parts:

- Minefield
    - Implements the core mechanics, also really trigger happy with throwing exception.
- Controller
    - Provides a cursor to move around and also a `click()` method, hides all the exceptions of the core ("Minefield") from the user.
- Display
    - Puts the output of controller and minefield on the screen and translates the keypresses to the controller.

There is also a fourth component, which can usually be ignored:

- Main Function
    - The `main()`-function uses `argp` to parse the given command line arguments and display the `--help`-page. It generates the random seed, creates an instance of `Display` and catches errors.
