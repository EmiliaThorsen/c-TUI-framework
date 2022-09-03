# c-TUI-framework
A small framework for making and managing a user interface in the terminal written in c for my other TUI projects

Made to be similar in look and feal to vim with a top bar, splits, status bar, floating window, and its signature keystroke system with "nested keystrokes" as i call them like for example "dd" or "gf" in vim.

## project status
This project is currently at where I want it for now, and will only update it if I find bugs or come up with more needed featrues, feel free to use it for your projects as well as I concider it "production ready" (big air qoutes)!

## usage
Should be easy egnouth to understand the structure from my comments in the .h file

Define the theme you want with the theme struct
define the keystrokes you want with the keystrokes struct containing keystroke strucs with pointers to functions in your project or nested keystrokes.

then in your main loop, poll keystrokes with the updateTUIKeystrokes function, update screen size with the updateScreenSize function, use the size getters to define a layout in the TUIStruct with tabs and content/splits and render it all with renderTUI that calls your defined window rendering functions.

(pssst the main.c file is testing code, but you can look in there for "examples" of how to do stuff, tho I will warn it's not the best example code...)
