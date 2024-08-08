#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <lauxlib.h>
#include "luainterop.h"

#define LUA_PREPARE_FUNCTION_CALL( L, name )                              \
        do {                                                              \
          lua_getglobal( L, name );                                       \
          if( !lua_isfunction( L, -1 ) ) {                                \
            fprintf( stderr, "LUA ERROR: " name " is not a function\n" ); \
            return LUA_ERRRUN;                                            \
          }                                                               \
        } while( 0 )

#define LUA_CALL_FUNCTION( L, nargs, nresults )                               \
        do {                                                                  \
          int32_t __status;                                                   \
          if( (__status = lua_pcall( L, nargs, nresults, 0 ) ) != LUA_OK ) { \
            return __status;                                                  \
          }                                                                   \
        } while( 0 )

static int32_t
has_repeating_characters( lua_State * const L ) {

  const char * const str       = luaL_checkstring( L, 1 );
  const uint8_t      min_count = (uint8_t)luaL_checkinteger( L, 2 );

  bool result = false;

  if( str == NULL || min_count == 0 ) {
    goto done;
  }

  const size_t len   = strlen( str );
  uint8_t      count = 1;

  for( size_t i = 1; i < len; i++ ) {
    if( str[i] == str[i - 1] ) {
      count++;
      if( count == min_count ) {
        result = true;
        break;
      }
    } else {
      count = 1;
    }
  }

done:
  lua_pushboolean( L, result );
  return 1;
}

static int32_t
has_leading_repeating_characters( lua_State * const L ) {

  const char *  const str       = luaL_checkstring( L, 1 );
  const uint8_t       min_count = (uint8_t)luaL_checkinteger( L, 2 );

  bool result = true;

  if( str == NULL || min_count == 0 ) {
    result = false;
    goto done;
  }

  for( size_t i = 1; i < min_count; i++ ) {
    if( str[i] == '\0' || str[i] != str[0] ) {
      result = false;
      break;
    }
  }

done:
  lua_pushboolean( L, result );
  return 1;
}

static const struct luaL_Reg vanitylib[] = {
  { "has_repeating_characters", has_repeating_characters },
  { "has_leading_repeating_characters", has_leading_repeating_characters },
  { NULL, NULL }
};

static int32_t
luaopen_vanity( lua_State * const L ) {

  luaL_newlib( L, vanitylib );
  return 1;
}

void
luainterop_register_libs( lua_State * const L ) {

  luaL_requiref( L, "vanity", luaopen_vanity, 1 );
  lua_pop( L, 1 );
}


int32_t
luainterop_next_path( lua_State * const L,
                      const char **     path ) {

  LUA_PREPARE_FUNCTION_CALL( L, "NextPath" );
  LUA_CALL_FUNCTION( L, 0, 1 );

  *path = lua_tostring( L, -1 );
  if( *path == NULL ) {
    fprintf( stderr, "LUA ERROR: NextPath did not return a string\n" );
    return LUA_ERRRUN;
  }

  lua_pop( L, 1 );

  return LUA_OK;
}

int32_t
luainterop_on_address( lua_State * const  L,
                       const char * const path,
                       const char * const address ) {

  LUA_PREPARE_FUNCTION_CALL( L, "OnAddress" );

  lua_pushstring( L, path );
  lua_pushstring( L, address );
  LUA_CALL_FUNCTION( L, 2, 0 );

  return LUA_OK;
}

int32_t
luainterop_on_exit( lua_State * const L ) {

  LUA_PREPARE_FUNCTION_CALL( L, "OnExit" );
  LUA_CALL_FUNCTION( L, 0, 0 );

  return LUA_OK;
}