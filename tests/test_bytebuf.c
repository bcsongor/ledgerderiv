#include <stdint.h>
#include "utest.h"
#include "../src/bytebuf.h"

UTEST( bytebuf, test_write_bytes ) {
  BYTEBUF_NEW( buf, 10 );

  const uint8_t to_write[] = { 0x01, 0x02, 0x03, 0x04 };

  int32_t res = write_bytes( &buf, to_write, sizeof(to_write));

  ASSERT_EQ( res, BYTEBUF_OK );
  ASSERT_EQ( buf.len, sizeof(to_write) );
  for( size_t i = 0; i < buf.len; i++ ) {
    ASSERT_EQ( buf.data[i], to_write[i] );
  }
}

UTEST( bytebuf, test_write_bytes_overflow ) {
  BYTEBUF_NEW( buf, 2 );

  const uint8_t to_write[] = { 0x01, 0x02, 0x03, 0x04 };

  int32_t res = write_bytes( &buf, to_write, sizeof(to_write));
  ASSERT_EQ( res, BYTEBUF_ERROR_OVERFLOW );
}

UTEST( bytebuf, test_write_u8 ) {
  BYTEBUF_NEW( buf, 10 );

  int32_t res = write_u8( &buf, 0x01 );
  ASSERT_EQ( res, BYTEBUF_OK );
  ASSERT_EQ( buf.len, 1 );
  ASSERT_EQ( buf.data[0], 0x01 );
}

UTEST( bytebuf, test_write_u8_overflow ) {
  BYTEBUF_NEW( buf, 0 );

  int32_t res = write_u8( &buf, 0x01 );
  ASSERT_EQ( res, BYTEBUF_ERROR_OVERFLOW );
}

UTEST( bytebuf, test_write_u16be ) {
  BYTEBUF_NEW( buf, 10 );

  int32_t res = write_u16be( &buf, 0x0102 );
  ASSERT_EQ( res, BYTEBUF_OK );
  ASSERT_EQ( buf.len, 2 );
  ASSERT_EQ( buf.data[0], 0x01 );
  ASSERT_EQ( buf.data[1], 0x02 );
}

UTEST( bytebuf, test_write_u16be_overflow ) {
  BYTEBUF_NEW( buf, 1 );

  int32_t res = write_u16be( &buf, 0x0102 );
  ASSERT_EQ( res, BYTEBUF_ERROR_OVERFLOW );
}

UTEST( bytebuf, test_write_u32be ) {
  BYTEBUF_NEW( buf, 10 );

  int32_t res = write_u32be( &buf, 0x01020304 );
  ASSERT_EQ( res, BYTEBUF_OK );
  ASSERT_EQ( buf.len, 4 );
  ASSERT_EQ( buf.data[0], 0x01 );
  ASSERT_EQ( buf.data[1], 0x02 );
  ASSERT_EQ( buf.data[2], 0x03 );
  ASSERT_EQ( buf.data[3], 0x04 );
}

UTEST( bytebuf, test_write_u32be_overflow ) {
  BYTEBUF_NEW( buf, 3 );

  int32_t res = write_u32be( &buf, 0x01020304 );
  ASSERT_EQ( res, BYTEBUF_ERROR_OVERFLOW );
}
