#include "hidutils.h"
#include "sleep.h"

const int32_t  HID_READ_TIMEOUT_MS     = 3 * 1000;
const uint8_t  HID_READ_RETRY_COUNT    = 3;
const uint32_t HID_READ_RETRY_DELAY_MS = 1000;

int32_t
hid_read_timeout_retry( hid_device * dev,
                        uint8_t *    buf ) {

  int8_t  retries = 0;
  int32_t res     = 0;

  while( res == 0 && retries < HID_READ_RETRY_COUNT ) {
    res = hid_read_timeout( dev, buf, HID_API_PACKET_SIZE, HID_READ_TIMEOUT_MS );

    if( res == HID_API_ERROR ) return res;
    else if( res == 0 ) sleep_ms( HID_READ_RETRY_DELAY_MS );

    retries++;
  }

  return res;
}
