#include "simba.h"

/**
 * Mapping table Dx -> pin number.
 */
static const int digital_pin_map[] = {
    16,
     5,
     4,
     0,
     2,
    14,
    12,
    13,
    15,
};

int board_pin_string_to_device_index(const char *str_p)
{
    long pin;

    if (strncmp(&str_p[0], "gpio", 4) == 0) {
        if (std_strtol(&str_p[4], &pin) == NULL) {
            return (-1);
        }

        switch (pin) {

        case 0:
        case 2:
        case 4:
        case 5:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            break;

        default:
            return (-1);
        }
    } else if ((str_p[0] == 'd') || (str_p[0] == 'D')) {
        if (std_strtol(&str_p[1], &pin) == NULL) {
            return (-1);
        }

        if ((pin < 0) || (pin >= membersof(digital_pin_map))) {
            return (-1);
        }

        pin = digital_pin_map[pin];
    } else if (strcmp(str_p, "led") == 0) {
        pin = 2;
    } else {
        return (-1);
    }

    return (pin);
}
