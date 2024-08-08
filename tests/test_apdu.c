#include "utest.h"
#include "../src/apdu.h"

UTEST( apdu, test_response_new ) {
  const uint8_t          data[] = { 0x02, 0x01, 0x0a, 0x04, 0x90, 0x00 };
  struct apdu_response * res    = apdu_response_new( (uint8_t *)data, sizeof(data) );

  ASSERT_NE( res, NULL );
  ASSERT_EQ( res->status, APDU_STATUS_SUCCESS );
  ASSERT_EQ( res->payloadlen, sizeof(data) - 2 );
  ASSERT_EQ( res->payload[1], 1 );
  ASSERT_EQ( res->payload[2], 10 );
  ASSERT_EQ( res->payload[3], 4 );
}

UTEST( apdu, test_response_new_null ) {
  struct apdu_response * res = apdu_response_new( NULL, 0 );
  ASSERT_EQ( res, NULL );
}

UTEST( apdu, test_response_new_short ) {
  const uint8_t          data[] = { 0x90 };
  struct apdu_response * res    = apdu_response_new( (uint8_t *)data, sizeof(data) );

  ASSERT_EQ( res, NULL );
}

UTEST( apdu, test_response_new_no_payload ) {
  const uint8_t          data[] = { 0x90, 0x00 };
  struct apdu_response * res    = apdu_response_new( (uint8_t *)data, sizeof(data) );

  ASSERT_NE( res, NULL );
  ASSERT_EQ( res->status, APDU_STATUS_SUCCESS );
  ASSERT_EQ( res->payloadlen, 0 );
  ASSERT_EQ( res->payload, NULL );
}
