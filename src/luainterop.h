#ifndef LUAINTEROP_H
#define LUAINTEROP_H

#include <lua.h>

/**
 * Registers the Lua libraries (e.g. vanity).
 *
 * @param[in] L  Lua state.
 */
void
luainterop_register_libs( lua_State * L );

/**
 * Returns the next HD path used for address derivation using
 * the NextPath Lua function.
 *
 * @param[in]  L     Lua state.
 * @param[out] path  HD derivation path.
 *
 * @return  Lua status code.
 */
int32_t
luainterop_next_path( lua_State *   L,
                      const char ** path );

/**
 * Triggers the OnAddress handler in Lua.
 *
 * @param[in] L        Lua state.
 * @param[in] path     HD derivation path.
 * @param[in] address  Ethereum address.
 *
 * @return  Lua status code.
 */
int32_t
luainterop_on_address( lua_State *  L,
                       const char * path,
                       const char * address );

/**
 * Triggers the OnExit handler in Lua.
 *
 * @param[in] L  Lua state.
 *
 * @return  Lua status code.
 */
int32_t
luainterop_on_exit( lua_State * L );

#endif // LUAINTEROP_H
