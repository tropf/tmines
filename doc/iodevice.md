# IODevice
The Output is handled via a so called 'Output-Device'. This is a derived class from the `IODevice` interface which can be used by the `Display` class.

In theory this could be anything, like a network controller, a module sending signals to a laser drawing the board...

In reality it is either a set of aliases for the curses library or a testing module that simulates the curses behaviour. This way, the inner workings of the Display class can be explored w/o ever touching the delicate operation of curses.

## Mock Resize
To provoke crashes, screen resizes can be mocked using the `mockResize()` method of the `IODeviceSimulation` class.

When the mocking is active, both `getHeight()` and `getWidth()` will return 1, despite all data being kept at the previous state.

> While reading from the IODevice `getHeight()` and `getWidth()` will be consulted.
> So to read data, the mocking state should be deactivated.

State   | Sample Call       | Description
------- | ----------------- | ---------------------------------
`0`     | `mockResize(0)`   | Instantly begin mocking
`1`     | `mockResize()`    | Switch to state `0` as soon as `getChar()` returns `KEY_RESIZE`
`-1`    | `mockResize(-1)`  | Disables all mocking

Note: All states > 0 are treated like 1, and all states < 0 like -1.

## Testing Output
If the output of the display class should be tested, it has to end with a crash. On a regular exit, the display class automatically clears the screen so nothing can be retrieved via testing. This is performed via a mock resize so the class crashes and the screen doesn't get cleared, like that:

```c++
io->addChar(KEY_RESIZE);
io->mockResize();
CHECK_THROWS(Display display(io, 8, 8, 10, 0, false));

// unmock resize
io->mockResize(-1);
```
