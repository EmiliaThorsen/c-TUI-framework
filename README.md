# c-TUI-framework
A small framework for making and managing a user interface in the terminal written in c for my other TUI projects

Made to be similar in look and feal to vim with a top bar, splits, status bar, floating window, and its signature keystroke system with "nested keystrokes" as i call them like for example "dd" or "gf" in vim.

## project status
This project is currently at where I want it for now, and will only update it if I find bugs or come up with more needed featrues, feel free to use it for your projects as well as I concider it "production ready" (big air qoutes)!

## usage
Should be easy egnouth to understand the structure from my comments in the .h file

Define the theme you want and the keystrokes you want with pointers to functions in your project.

then in your main loop, poll keystrokes with the updateTUIKeystrokes function, update screen size with that function, use the getters to define a layout in the TUIStruct and render it all with renderTUI.

(pssst the main.c file is testing code, but you can look in there for "examples" of how to do stuff, tho I will warn its crude...)
