#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <hidapi.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "luainterop.h"
#include "bytebuf.h"
#include "hidutils.h"
#include "apdu.h"
#include "ledger.h"
#include "ledgereth.h"
#include "hdpath.h"

//
// Error handling.
//
#define HANDLE_ERROR( cond, newstatus ) \
        do {                            \
          if( cond ) {                  \
            status = (newstatus);       \
            goto cleanup;               \
          }                             \
        } while( 0 )

#define HANDLE_HID_ERROR( cond )  HANDLE_ERROR( cond, STATUS_ERROR_HID )
#define HANDLE_APDU_ERROR( cond ) HANDLE_ERROR( cond, STATUS_ERROR_APDU )
#define HANDLE_LUA_ERROR( cond )  HANDLE_ERROR( cond, STATUS_ERROR_LUA )

//
// Globals.
//
enum status      status      = STATUS_OK;
enum apdu_status apdu_status = APDU_STATUS_SUCCESS;
bool             running     = true;

void
handle_exit_signal( int sig ) {

  (void)sig;
  running = false;
}

int32_t
usage( const char * error ) {

  static const char * app = "ledgerderiv"
#ifdef _WIN32
                            ".exe"
#endif
  ;

  printf( "Usage: %s <OPTIONS>\n", app );
  printf( "\nOptions: -s <script>  Lua script with the following functions defined:" );
  printf( "\n                        NextPath()                         Returns the next HD derivation path" );
  printf( "\n                        OnAddress(path, checksum_address)  Called after an address is derived" );
  printf( "\n                        OnExit()                           Called when the program exits" );
  printf( "\n         -h           Prints usage" );
  printf( "\n\nExample: %s -s scripts/find-vanity-address.lua\n", app );

  if( error != NULL ) {
    fprintf( stderr, "\nError: %s\n", error );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int
main( int    argc,
      char * argv[] ) {

  const char * script_path = NULL;

  // Parse command line arguments.
  for( int i = 1; i < argc; i++ ) {
    if( !strcmp( argv[i], "-s" ) ) {
      if( i + 1 < argc ) script_path = argv[++i];
      else return usage( "-s option requires a Lua script" );
    } else if( !strcmp( argv[i], "-h" )) {
      return usage( NULL );
    } else {
      return usage( "Unknown option" );
    }
  }

  // Lua script is required!
  if( script_path == NULL ) return usage( "No script provided" );

  // Register signal handlers to clean up resources on SIGINT and SIGTERM.
  signal( SIGINT, handle_exit_signal );
  signal( SIGTERM, handle_exit_signal );

  hid_device * dev = NULL;
  lua_State *  L   = NULL;

  HANDLE_HID_ERROR( hid_init() == HID_API_ERROR );

  // Initialize Lua, open standard libraries and register custom libraries.
  L = luaL_newstate();
  luaL_openlibs( L );
  luainterop_register_libs( L );

  // Load the Lua script.
  printf( "Loading script: %s\n", script_path );
  HANDLE_LUA_ERROR( luaL_dofile( L, script_path ) != LUA_OK );

  // Find Ledger device by vendor ID.
  struct hid_device_info * devs = hid_enumerate( LEDGER_VENDOR_ID, 0 );
  HANDLE_HID_ERROR( devs == NULL );

  // Use first Ledger device.
  uint16_t ledger_product_id = devs->product_id;
  wprintf( L"Ledger found: %ls - %ls (0x%X/0x%X)\n", devs->manufacturer_string, devs->product_string, devs->vendor_id, devs->product_id );
  hid_free_enumeration( devs );

  dev = hid_open( LEDGER_VENDOR_ID, ledger_product_id, NULL );
  HANDLE_HID_ERROR( dev == NULL );

  // Ping Ledger device to make sure connection is established.
  HANDLE_HID_ERROR( hid_ledger_send_ping( dev ) == HID_API_ERROR );
  HANDLE_HID_ERROR( hid_ledger_recv( dev, NULL, NULL ) == HID_API_ERROR );

  printf( "Ledger connected\n" );

  // Fetch Ethereum app version.
  uint8_t version[3];
  HANDLE_APDU_ERROR( eth_ledger_get_app_version( dev, version, &apdu_status ) != STATUS_OK );
  printf( "Ethereum app version: %u.%u.%u\n", version[0], version[1], version[2] );

  char         address[ADDRESS_LENGTH + 1]; // Without the leading '0x'.
  const char * path;

  // Derive Ethereum addresses until stopped.
  while( running ) {
    // Get next HD derivation path from Lua script.
    HANDLE_LUA_ERROR( luainterop_next_path( L, &path ) != LUA_OK );

    // Parse HD path string.
    struct hdpath * const hdpath = hdpath_parse( path );
    HANDLE_ERROR( hdpath == NULL, STATUS_ERROR_HDPATH );

    // Derive checksummed Ethereum address.
    const enum status retval = eth_ledger_get_address( dev, hdpath, address, &apdu_status );
    free( hdpath );

    HANDLE_APDU_ERROR( retval != STATUS_OK );

    // Notify Lua script about the derived address.
    HANDLE_LUA_ERROR( luainterop_on_address( L, path, address ) != LUA_OK );
  }

cleanup:
  if( status == STATUS_ERROR_HID ) fwprintf( stderr, L"HID ERROR: %ls\n", hid_error( NULL ) );
  else if( status == STATUS_ERROR_APDU ) fprintf( stderr, "APDU ERROR: %s (0x%X)\n", apdu_error( apdu_status ), apdu_status );
  else if( status == STATUS_ERROR_HDPATH ) fprintf( stderr, "ERROR: Failed to parse HD path\n" );
  else if( status == STATUS_ERROR_LUA ) fprintf( stderr, "LUA ERROR: %s\n", lua_tostring( L, -1 ));
  else if( status != STATUS_OK ) fprintf( stderr, "ERROR: Unknown error\n" );

  // Notify Lua script about program exit.
  luainterop_on_exit( L );

  if( dev != NULL ) hid_close( dev );
  if( hid_exit() == HID_API_ERROR ) status = STATUS_ERROR_HID;
  if( L != NULL ) lua_close( L );

  return status;
}
