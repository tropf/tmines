# Eror Reporting
TerminateMines creates extensive error logs.

Information saved in crash reports:

- error message
- series of keystrokes to reproduce the crash
- settings of the minefield: width, height, mine count and seed
- terminal information: number of lines and cols
- version information: version number and git commit build

The program then writes this crash report to the file `tmines.crash` in the current working directory.
Preexisting files with the same name are overriden.
If the report cannot be written to file, it will be printed to stderr.

Either way, a request to email the crash report to the bug report address is printed.

A stack trace is not in the error log, as it is fairly useless when the project is built without debugging symbols.

The keystrokes to reproduce the problem are recorded while playing the game. The displayed keystrokes are **not** the actual keystrokes by the user, but his actions mapped to the keys `h`, `j`, `k`, `l`, `f` and spacebar. Any keystroke that doesn't imply an action for the game is not recorded.

Any error thrown during the `Display::run()` method will be caught, and the relevant keystrokes will be appended to the message. With this message a new exception will be thrown.

This exception is then caught in the `main()` function, where all the other information is added. After the crash report has been fully prepared, the crash report file will be opened and the report wrote to file.

> **Only the first line** of an error message is printed for the user.

Everything after that is only visible in the error report.  
This results in two important features:

- The user only sees short (and therefore hopefully non-confusing) error messages.
- Debugging information can be appended after a newline `\n` without it confusing anyone.
