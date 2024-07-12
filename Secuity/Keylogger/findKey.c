#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_EVENT_DEVICES 20  // Maximum number of event devices to check
#define COMMAND_BUF_SIZE 256  // Size of command buffer

int main() {
    int found_keyboard = 0;
    char command[COMMAND_BUF_SIZE];
    char response[COMMAND_BUF_SIZE];
    FILE *fp;

    // Iterate through event devices
    for (int i = 0; i < MAX_EVENT_DEVICES; ++i) {
	found_keyboard=0; 
        // Construct udevadm command to query device properties
        snprintf(command, COMMAND_BUF_SIZE, "udevadm info --query=property --name=/dev/input/event%d", i);

        // Execute udevadm command
        fp = popen(command, "r");
        if (fp == NULL) {
            perror("Failed to run udevadm command");
            exit(EXIT_FAILURE);
        }

        // Read response from udevadm
        while (fgets(response, sizeof(response), fp) != NULL) {
            // Check if response contains "ID_INPUT_KEYBOARD"
            if (strstr(response, "ID_INPUT_KEYBOARD") != NULL) {
                found_keyboard = 1;
                break;
            }
        }

        // Close udevadm output stream
        pclose(fp);

        // Stop searching if keyboard is found
        if (found_keyboard) {
            printf("Keyboard found at /dev/input/event%d\n", i);
            //break;
        }
	if (i==20) {
		break; 
	}
    }

    if (!found_keyboard) {
        printf("No keyboard found among the first %d event devices.\n", MAX_EVENT_DEVICES);
    }

    return 0;
}
