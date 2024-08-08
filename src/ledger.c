#include <stdlib.h>
#include "ledger.h"
#include "hidutils.h"

const uint8_t  REPORT_ID  = 0x00;
const uint16_t CHANNEL_ID = 0x101;

const uint8_t TAG_PING = 0x02;
const uint8_t TAG_APDU = 0x05;

int32_t
hid_ledger_send_ping( hid_device * dev ) {

  BYTEBUF_NEW( buf, HID_API_PACKET_SIZE );

  write_u8( &buf, REPORT_ID ); // Report ID is ignored by Ledger.
  write_u16be( &buf, CHANNEL_ID );
  write_u8( &buf, TAG_PING );

  return hid_write( dev, buf.data, buf.len );
}

int32_t
hid_ledger_send_apdu( hid_device *                      dev,
                      const struct apdu_command * const command,
                      const struct bytebuf * const      payload ) {

  BYTEBUF_NEW( buf, HID_API_PACKET_SIZE );

  uint8_t payloadlen = payload == NULL ? 0 : (uint8_t)payload->len;

  write_u8( &buf, REPORT_ID ); // Report ID is ignored by Ledger.
  write_u16be( &buf, CHANNEL_ID );
  write_u8( &buf, TAG_APDU );
  write_u16be( &buf, 0x00 /* frame_id */ ); // TODO: Support larger payloads requiring multiple frames.

  const size_t apdulen = sizeof(struct apdu_command) + 1 + payloadlen;
  write_u16be( &buf, (uint16_t)apdulen );

  write_bytes( &buf, command, sizeof(struct apdu_command));
  write_u8( &buf, payloadlen );

  if( payload != NULL ) write_bytes( &buf, payload->data, payloadlen );

  return hid_write( dev, buf.data, buf.len );
}

int32_t
hid_ledger_recv( hid_device * dev,
                 uint8_t **   data,
                 size_t *     datalen ) {

  uint8_t buf[HID_API_PACKET_SIZE];

  int32_t res = hid_read_timeout_retry( dev, buf );
  if( res == HID_API_ERROR || datalen == NULL || data == NULL ) return res;

  // TODO: Implement header validation instead of skipping it.
  int32_t offset = 5;

  *datalen = ((uint16_t)buf[offset] << 8) | buf[offset + 1];
  *data    = (uint8_t *)malloc(  *datalen );

  offset += 2;
  memcpy( *data, &buf[offset], *datalen < res - offset ? *datalen : res - offset );

  int32_t left = (int32_t)*datalen - res + offset;
  while( left > 0 ) {
    res = hid_read_timeout_retry( dev, buf );
    if( res == HID_API_ERROR ) {
      *datalen = 0;
      free( *data );
      return res;
    }

    memcpy( &((*data)[*datalen - left]), &buf[5], left > sizeof(buf) ? sizeof(buf) : left );
    left -= res;
  }

  return res;
}

int32_t
hid_ledger_recv_apdu( hid_device *            dev,
                      struct apdu_response ** response ) {

  size_t    datalen;
  uint8_t * data;

  int32_t res = hid_ledger_recv( dev, &data, &datalen );
  if( res == HID_API_ERROR ) return res;

  *response = apdu_response_new( data, datalen );

  free( data );

  if( *response == NULL ) return HID_API_ERROR;

  return res;
}
