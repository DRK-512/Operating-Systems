#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This function will read through the keyboard files and return the event path that is a keyboard
char* find_keyboards() {
    FILE *fp;
    char line[256];
    char *name = NULL;
    char *handlers = NULL;
    char *event_path = NULL;

    // Open the input devices file
    fp = fopen("/proc/bus/input/devices", "r");
    if (fp == NULL) {
        perror("Error opening /proc/bus/input/devices");
        return NULL;
    }

    // Read file line by line
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Check for Name line
        if (strncmp(line, "N: Name=", 8) == 0) {
            if (name) free(name); // Free previous name if exists
            name = strdup(line + 8);
            // Remove quotes if present
            if (name[0] == '"') {
                memmove(name, name + 1, strlen(name));
                name[strlen(name) - 1] = '\0';
            }
        }

        // Check for Handlers line
        if (strncmp(line, "H: Handlers=", 12) == 0) {
            if (handlers) free(handlers); // Free previous handlers if exists
            handlers = strdup(line + 12);
        }

        // If we have both name and handlers, check for keyboard
        if (name && handlers && strstr(name, "keyboard") != NULL) {
            // Look for eventX in handlers
            char *event = strstr(handlers, "event");
            if (event) {
                char event_num[16];
                sscanf(event, "event%15s", event_num);
                // Allocate memory for full path (e.g., /dev/input/eventX)
                event_path = malloc(32);
                snprintf(event_path, 32, "/dev/input/event%s", event_num);
                // Break since we found the keyboard
                break;
            }
        }

        // Reset for next device on empty line
        if (line[0] == '\0') {
            if (name) free(name);
            if (handlers) free(handlers);
            name = NULL;
            handlers = NULL;
        }
    }

    // Free any remaining allocated memory
    if (name) free(name);
    if (handlers) free(handlers);

    fclose(fp);
    return event_path;
}

