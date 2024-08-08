#ifndef HDPATH_H
#define HDPATH_H

#include <stdint.h>

#define MAX_COMPONENT_COUNT 10

struct hdpath {
  uint8_t  length;
  uint32_t components[MAX_COMPONENT_COUNT];
};

/**
 * Parses a BIP32 path.
 *
 * @param[in] path  BIP32 path.
 *
 * @return  BIP32 path structure, or NULL on error.
 */
struct hdpath *
hdpath_parse( const char * path );

#endif // HDPATH_H
