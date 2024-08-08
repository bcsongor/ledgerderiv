#include <hidapi.h>
#include "ledgereth.h"
#include "ledger.h"
#include "hidutils.h"

enum status
eth_ledger_get_app_version( hid_device * const dev,
                            uint8_t * const    version,
                            enum apdu_status * apdu_status ) {

  const struct apdu_command cmd = {
    .cla = 0xe0,
    .ins = 0x06,
    .p1  = 0x00,
    .p2  = 0x00
  };

  if( hid_ledger_send_apdu( dev, &cmd, NULL ) == HID_API_ERROR ) return STATUS_ERROR_HID;

  struct apdu_response * res;
  if( hid_ledger_recv_apdu( dev, &res ) == HID_API_ERROR ) return STATUS_ERROR_HID;

  *apdu_status = res->status;
  if( *apdu_status != APDU_STATUS_SUCCESS ) return STATUS_ERROR_APDU;

  // Major Minor Patch
  memcpy( version, &res->payload[1], 3 );

  apdu_response_free( res );

  return STATUS_OK;
}

enum status
eth_ledger_get_address( hid_device * const          dev,
                        const struct hdpath * const hdpath,
                        char *                      address,
                        enum apdu_status *          apdu_status ) {

  BYTEBUF_NEW(payload, 1 + MAX_COMPONENT_COUNT * sizeof(uint32_t));

  write_u8( &payload, hdpath->length );
  for( size_t i = 0; i < hdpath->length; i++ ) {
    uint32_t index = hdpath->components[i];
    write_u32be( &payload, index );
  }

  const struct apdu_command cmd = {
    .cla = 0xe0,
    .ins = 0x02,
    .p1  = 0x00,              // 0x00: return address, 0x01: display and confirm before returning.
    .p2  = 0x00               // 0x00: do not return the chain code, 0x01: return the chain code.
  };
  if( hid_ledger_send_apdu( dev, &cmd, &payload ) == HID_API_ERROR ) return STATUS_ERROR_HID;

  struct apdu_response * res;
  if( hid_ledger_recv_apdu( dev, &res ) == HID_API_ERROR ) return STATUS_ERROR_HID;

  *apdu_status = res->status;
  if( *apdu_status != APDU_STATUS_SUCCESS ) return STATUS_ERROR_APDU;

  memcpy( address, &res->payload[1 + res->payload[0] + 1], ADDRESS_LENGTH );
  address[ADDRESS_LENGTH] = '\0';

  apdu_response_free( res );

  return STATUS_OK;
}
