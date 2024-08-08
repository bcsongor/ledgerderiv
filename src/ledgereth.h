#ifndef LEDGERETH_H
#define LEDGERETH_H

#include <stdint.h>
#include <hidapi.h>
#include "status.h"
#include "apdu.h"
#include "hdpath.h"

#define ADDRESS_LENGTH 40

/**
 * Gets the version of the Ethereum app installed on the Ledger device.
 *
 * @param[in]  dev          HID device handle.
 * @param[out] version      Version triplet (major, minor, patch).
 * @param[out] apdu_status  APDU status.
 *
 * @return  Status code.
 */
enum status
eth_ledger_get_app_version( hid_device *       dev,
                            uint8_t *          version,
                            enum apdu_status * apdu_status );

/**
 * Derives the Ethereum address for a given HD path.
 *
 * @param[in]  dev          HID device handle.
 * @param[in]  hdpath       HD path.
 * @param[out] address      Ethereum address without the leading '0x'.
 * @param[out] apdu_status  APDU status.
 *
 * @return  Status code.
 */
enum status
eth_ledger_get_address( hid_device *          dev,
                        const struct hdpath * hdpath,
                        char *                address,
                        enum apdu_status *    apdu_status );

#endif // LEDGERETH_H
