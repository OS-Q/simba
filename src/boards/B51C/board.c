#include "simba.h"

int board_pin_string_to_device_index(const char *str_p)
{
    long pin;

    if (strncmp(&str_p[0], "d", 1) == 0) {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }

        if (pin == 0) {
            pin = 16;
        } else if (pin == 1) {
            pin = 5;
        } else if (pin == 2) {
            pin = 4;
        } else if (pin == 3) {
            pin = 0;
        } else if (pin == 4) {
            pin = 2;
        } else if (pin == 5) {
            pin = 14;
        } else if (pin == 6) {
            pin = 12;
        } else if (pin == 7) {
            pin = 13;
        } else if (pin == 8) {
            pin = 15;
        } else if (pin == 9) {
            pin = 3;
        } else if (pin == 10) {
            pin = 1;
        } else {
            pin = -1;
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 16;
    } else {
        return (-1);
    }

    return (pin);
}
