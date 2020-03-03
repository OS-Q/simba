#include "simba.h"

int board_pin_string_to_device_index(const char *str_p)
{
    long pin;

    if (strncmp(&str_p[0], "gpio", 4) == 0) {
        if (std_strtol(&str_p[4], &pin) == NULL) {
            return (-1);
        }

        if ((pin >= 0) && (pin <= 27)) {
            /* Correct pin number. */
        } else if ((pin >= 32) && (pin <= 36)) {
            pin -= 4;
        } else if (pin == 39) {
            pin -= 4;
        } else {
            return (-1);
        }
    } else if (strcmp(str_p, "led") == 0) {
        pin = 16;
    } else {
        return (-1);
    }

    return (pin);
}
