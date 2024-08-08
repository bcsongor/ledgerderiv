#ifndef APDU_H
#define APDU_H

#include <stdint.h>

enum apdu_status {
  APDU_STATUS_LOCKED       = 0x5515,
  APDU_STATUS_INVALID_PATH = 0x6a15,
  APDU_STATUS_WRONG_APP    = 0x6d02,
  APDU_STATUS_SUCCESS      = 0x9000,
};

struct apdu_command {
  uint8_t cla;
  uint8_t ins;
  uint8_t p1;
  uint8_t p2;
};

struct apdu_response {
  uint16_t  status;
  uint8_t * payload;
  size_t    payloadlen;
};

/**
 * Creates a new APDU response.
 *
 * @param[in] data    Data buffer containing an APDU payload.
 * @param[in] datalen Buffer length.
 *
 * @return  APDU response.
 */
struct apdu_response *
apdu_response_new( uint8_t * data,
                   size_t    datalen );

/**
 * Frees an APDU response resource.
 *
 * @param[in] res  APDU response.
 */
void
apdu_response_free( struct apdu_response * res );

/**
 * Returns a human-readable error message for an APDU status word.
 *
 * @param[in] status_word  APDU status word.
 *
 * @return  Error message.
 */
const char *
apdu_error( enum apdu_status status_word );

#endif // APDU_H
