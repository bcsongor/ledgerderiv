#include "utest.h"
#include "../src/hdpath.h"

UTEST( hdpath, test_parse_hdpath ) {
  struct hdpath * const hdpath = hdpath_parse( "m/44'/60'/0'/0/1" );

  ASSERT_NE( hdpath, NULL );
  ASSERT_EQ( hdpath->length, 5 );
  ASSERT_EQ( hdpath->components[0], 0x8000002c );
  ASSERT_EQ( hdpath->components[1], 0x8000003c );
  ASSERT_EQ( hdpath->components[2], 0x80000000 );
  ASSERT_EQ( hdpath->components[3], 0x00000000 );
  ASSERT_EQ( hdpath->components[4], 0x00000001 );

  free( hdpath );
}

UTEST( hdpath, test_parse_hdpath_invalid_empty_path ) {
  ASSERT_EQ(  hdpath_parse( "" ), NULL );
  ASSERT_EQ(  hdpath_parse( NULL ), NULL );
}

UTEST( hdpath, test_parse_hdpath_invalid_malformed_path ) {
  ASSERT_EQ(  hdpath_parse( "/44'/60'/0'/0/0" ), NULL );
  ASSERT_EQ(  hdpath_parse( "m/44'60'/0'/0/0" ), NULL );
}
