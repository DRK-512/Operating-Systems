# Service Keylogger
- Create a program that will keylog all inputs and will place them in a file
- This will most likely be ran as a service so on boot, you will always be writting a file
- I mean you could in theory use this to write to /tmp/keylogger or maybe a samba server so you can record someones keystrokes, idk

## TODO
- So the main functionality works, but it needs major cleanup, such as not being the standard format I make everything (src/include dirs)
- Right now the findKey.c will tell you which of your devices is a keyboard, but I need to edit it so it displays all keyboards
- Then I have to add that to the main.c which will log all of your keystrokes
- I also gotta make a help command line option