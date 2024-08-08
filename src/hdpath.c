#include <stdlib.h>
#include <string.h>
#include "hdpath.h"

#ifdef _WIN32
#define strdup _strdup
#endif

struct hdpath *
hdpath_parse( const char * const path ) {

  if( path == NULL ) return NULL;

  char * path_copy = strdup( path );
  if( path_copy == NULL ) return NULL;

  char * token = strtok( path_copy, "/" );
  if( token == NULL || strcmp( token, "m" ) != 0 ) {
    free( path_copy );
    return NULL;
  }

  struct hdpath * const hdpath = (struct hdpath *)malloc( sizeof(struct hdpath) );
  if( hdpath == NULL ) {
    free( path_copy );
    return NULL;
  }

  hdpath->length = 0;
  while((token = strtok( NULL, "/" )) != NULL ) {
    if( hdpath->length >= MAX_COMPONENT_COUNT ) {
      free( path_copy );
      free( hdpath );
      return NULL;
    }

    char *   endptr;
    uint32_t index = strtoul( token, &endptr, 10 );
    if( *endptr == '\'' && *(endptr + 1) == '\0' ) {
      index |= 0x80000000;             // Hardened index.
    } else if( *endptr != '\0' ) {
      free( path_copy );
      free( hdpath );
      return NULL;
    }

    hdpath->components[hdpath->length++] = index;
  }

  free( path_copy );

  return hdpath;
}