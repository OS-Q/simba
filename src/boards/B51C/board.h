#ifndef __BOARD_H__
#define __BOARD_H__

#define pin_d0_dev  pin_device[16]
#define pin_d1_dev  pin_device[5]
#define pin_d2_dev  pin_device[4]
#define pin_d3_dev  pin_device[0]
#define pin_d4_dev  pin_device[2]
#define pin_d5_dev  pin_device[14]
#define pin_d6_dev  pin_device[12]
#define pin_d7_dev  pin_device[13]
#define pin_d8_dev  pin_device[15]
#define pin_d9_dev  pin_device[3]
#define pin_d10_dev pin_device[1]

#define pin_led_dev pin_d4_dev

#define pin_a0_dev pin_device[0]

#define adc_0_dev adc_device[0]

#define flash_0_dev flash_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
