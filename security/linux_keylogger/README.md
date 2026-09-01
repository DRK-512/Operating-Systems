# Service Keylogger
- Create a program that will log all keyboard inputs and will place them in a file
- This will most likely be ran as a service so on boot, you will always be writting a file
- I mean you could in theory use this to write to /tmp/keylogger or maybe a samba server so you can record someones keystrokes, idk
- This will be in a Docker container since it requires a specific kernel and kernel modules and I expect a specific format
- Hopefully this will sway people away from using this for malicious use
- NOTE: This only works with 1 keyboard, so whatever keyboard is found first is utilized

# Deploy
To build this, you must first build and run the docker container
```bash
docker build . -t keylogger
docker run --device=/dev/input/ -it keylogger
```
Once you type in the above commands you will be inside the keylogger container<br>
From here you will just run the Keylogger script<br>
```bash
cd /root/
./Keylogger
```
Once you are done testing this, you can press CTRL+C to close the program and view the keylog.txt file and you will see your key stroke inputs<br>
