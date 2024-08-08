#ifndef BYTEBUF_H
#define BYTEBUF_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Detect endianness of the system and define BYTE_ORDER_LITTLE_ENDIAN if it is little endian.
// Assume Windows is always little endian, unless we're running NT kernel on an Xbox 360 :).
#ifdef _WIN32
    #define BYTE_ORDER_LITTLE_ENDIAN
    #define __builtin_bswap16 _byteswap_ushort
    #define __builtin_bswap32 _byteswap_ulong
#else
    #if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            #define BYTE_ORDER_LITTLE_ENDIAN
        #endif
    #endif
#endif

/**
 * Allocates a new byte buffer on the stack.
 */
#define BYTEBUF_NEW( varname, size )                                                      \
        uint8_t varname ## _data_[size];                                                  \
        struct bytebuf varname = { .data = varname ## _data_, .datalen = size, .len = 0 }

struct bytebuf {
  uint8_t *    data;
  const size_t datalen;
  size_t       len;
};

enum bytebuf_status {
  BYTEBUF_OK,
  BYTEBUF_ERROR_OVERFLOW,
};

/**
 * Writes an 8-bit unsigned integer to the buffer.
 *
 * @return  BYTEBUF_OK on success, BYTEBUF_ERROR_OVERFLOW on buffer overflow.
 */
static inline enum bytebuf_status
write_u8( struct bytebuf * const buf,
          const uint8_t          val ) {

  if( buf->len + 1 > buf->datalen ) return BYTEBUF_ERROR_OVERFLOW;

  buf->data[buf->len++] = val;

  return BYTEBUF_OK;
}

/**
 * Writes a 16-bit unsigned integer to the buffer in big-endian byte order.
 *
 * @return  BYTEBUF_OK on success, BYTEBUF_ERROR_OVERFLOW on buffer overflow.
 */
static inline enum bytebuf_status
write_u16be( struct bytebuf * const buf,
             uint16_t               val ) {

  if( buf->len + 2 > buf->datalen ) return BYTEBUF_ERROR_OVERFLOW;

#ifdef BYTE_ORDER_LITTLE_ENDIAN
  val = __builtin_bswap16( val );
#endif
  buf->data[buf->len++] = val & 0xff;
  buf->data[buf->len++] = (val >> 8) & 0xff;

  return BYTEBUF_OK;
}

/**
 * Writes a 32-bit unsigned integer to the buffer in big-endian byte order.
 *
 * @return  BYTEBUF_OK on success, BYTEBUF_ERROR_OVERFLOW on buffer overflow.
 */
static inline enum bytebuf_status
write_u32be( struct bytebuf * const buf,
             uint32_t               val ) {

  if( buf->len + 4 > buf->datalen ) return BYTEBUF_ERROR_OVERFLOW;

#ifdef BYTE_ORDER_LITTLE_ENDIAN
  val = __builtin_bswap32( val );
#endif
  buf->data[buf->len++] = val & 0xff;
  buf->data[buf->len++] = (val >> 8) & 0xff;
  buf->data[buf->len++] = (val >> 16) & 0xff;
  buf->data[buf->len++] = (val >> 24) & 0xff;

  return BYTEBUF_OK;
}

/**
 * Writes bytes to the buffer.
 *
 * @return  BYTEBUF_OK on success, BYTEBUF_ERROR_OVERFLOW on buffer overflow.
 */
static inline enum bytebuf_status
write_bytes( struct bytebuf * const buf,
             const void * const     data,
             const size_t           len ) {

  if( buf->len + len > buf->datalen ) return BYTEBUF_ERROR_OVERFLOW;

  memcpy( &buf->data[buf->len], data, len );
  buf->len += len;

  return BYTEBUF_OK;
}

/**
 * Dumps the contents of a byte buffer to the standard output.
 */
static inline void
hexdump( const uint8_t * const data,
         const size_t          len ) {

  char ascii[17];
  ascii[16] = '\0';

  for( size_t i = 0; i < len; i++ ) {
    printf( "%02x ", data[i] );
    if( data[i] >= ' ' && data[i] <= '~' ) {
      ascii[i % 16] = (char)data[i];
    } else {
      ascii[i % 16] = '.';
    }
    if( (i + 1) % 8 == 0 || i + 1 == len ) {
      printf( " " );
      if( (i + 1) % 16 == 0 ) {
        printf( "|  %s\n", ascii );
      } else if( i + 1 == len ) {
        ascii[(i + 1) % 16] = '\0';
        if( (i + 1) % 16 <= 8 ) {
          printf( " " );
        }
        for( size_t j = (i + 1) % 16; j < 16; j++ ) {
          printf( "   " );
        }
        printf( "|  %s\n", ascii );
      }
    }
  }
}

#endif // BYTEBUF_H