#ifndef SLEEP_H
#define SLEEP_H

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static inline void
sleep_ms( uint32_t ms ) {

#ifdef _WIN32
  Sleep( ms );
#else
  usleep( ms * 1000 );
#endif
}

#endif // SLEEP_H
