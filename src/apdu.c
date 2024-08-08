#include <stdlib.h>
#include <string.h>
#include "apdu.h"

struct apdu_response *
apdu_response_new( uint8_t * const data,
                   const size_t    datalen ) {

  if( data == NULL || datalen < 2 ) return NULL;

  struct apdu_response * res = (struct apdu_response *)malloc( sizeof(struct apdu_response) );
  if( res == NULL ) return NULL;

  res->status     = (data[datalen - 2] << 8) | data[datalen - 1];
  res->payloadlen = datalen - 2;
  if( res->payloadlen == 0 ) {
    res->payload = NULL;
    return res;
  }

  res->payload = (uint8_t *)malloc( res->payloadlen );
  if( res->payload == NULL ) {
    free( res );
    return NULL;
  }

  memcpy( res->payload, data, res->payloadlen );

  return res;
}

void
apdu_response_free( struct apdu_response * const res ) {

  if( res == NULL ) return;

  if( res->payload != NULL ) free( res->payload );
  free( res );
}

const char *
apdu_error( const enum apdu_status status_word ) {

  switch( status_word ) {
    case APDU_STATUS_LOCKED: return "Device locked";
    case APDU_STATUS_INVALID_PATH: return "Invalid HD path";
    case APDU_STATUS_WRONG_APP: return "Ethereum application unavailable";
    case APDU_STATUS_SUCCESS: return "Success";
    default: return "Unknown error";
  }
}