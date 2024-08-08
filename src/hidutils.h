#ifndef HIDUTILS_H
#define HIDUTILS_H

#include <stdint.h>
#include <hidapi.h>

#define HID_API_ERROR       (-1)
#define HID_API_PACKET_SIZE 64

/**
 * Waits and reads data from the HID device applying a timeout and retry logic.
 *
 * @param[in]  dev  HID device handle.
 * @param[out] buf  Data buffer.
 *
 * @return  Number of bytes read, or -1 on error.
 */
int32_t
hid_read_timeout_retry( hid_device * dev,
                        uint8_t *    buf );

#endif // HIDUTILS_H
