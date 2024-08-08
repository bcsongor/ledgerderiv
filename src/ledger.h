#ifndef LEDGER_H
#define LEDGER_H

#include <stdint.h>
#include <hidapi.h>
#include "bytebuf.h"
#include "apdu.h"

#define LEDGER_VENDOR_ID 0x2c97

/**
 * Sends a ping to the Ledger device.
 *
 * @param[in] dev  HID device handle.
 *
 * @return  Number of bytes sent, or -1 on error.
 */
int32_t
hid_ledger_send_ping( hid_device * dev );

/**
 * Sends an APDU command to the Ledger device.
 *
 * @param[in] dev      HID device handle.
 * @param[in] command  APDU command.
 * @param[in] payload  APDU payload.
 *
 * @return  Number of bytes sent, or -1 on error.
 */
int32_t
hid_ledger_send_apdu( hid_device *                dev,
                      const struct apdu_command * command,
                      const struct bytebuf *      payload );

/**
 * Receives data from the Ledger device.
 *
 * @param[in]  dev     HID device handle.
 * @param[out] data    Data buffer.
 * @param[out] datalen Data length.
 *
 * @return  Number of bytes received, or -1 on error.
 */
int32_t
hid_ledger_recv( hid_device * dev,
                 uint8_t **   data,
                 size_t *     datalen );

/**
 * Receives an APDU response from the Ledger device.
 *
 * @param[in]  dev HID device handle.
 * @param[out] res APDU response.
 *
 * @return  Number of bytes received, or -1 on error.
 */
int32_t
hid_ledger_recv_apdu( hid_device *            dev,
                      struct apdu_response ** res );

#endif // LEDGER_H
