// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <glua/glua.h>
#include <lauxlib.h>
#include <lua.h>
#include <stdlib.h>
#include <string.h>

#ifdef GLUA_ENABLE_STB
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#endif /* GLUA_ENABLE_STB */

static_assert(LUA_VERSION_NUM == 505);

int GluaReadInteger(lua_State *L, int idx, lua_Integer *value) {
    *value = 0;
    if (!lua_isinteger(L, idx))
        return lua_isnil(L, idx) ? GLUA_NIL_ERROR : GLUA_TYPE_ERROR;

    int isnum;
    *value = lua_tointegerx(L, idx, &isnum);
    assert(isnum);

    return GLUA_OK;
}

void GluaFreeInteger(lua_State *L, lua_Integer value) {
    (void)L;
    (void)(value);
}

int GluaPushInteger(lua_State *L, lua_Integer value) {
    lua_pushinteger(L, value);
    return GLUA_OK;
}

GLUA_READ_ARRAY_FUNCTION(Integer, lua_Integer)
GLUA_PUSH_ARRAY_FUNCTION(Integer, lua_Integer)
GLUA_FREE_ARRAY_FUNCTION(Integer, lua_Integer)
#ifdef GLUA_ENABLE_STB
GLUA_READ_DARRAY_FUNCTION(Integer, lua_Integer)
GLUA_PUSH_DARRAY_FUNCTION(Integer, lua_Integer)
GLUA_FREE_DARRAY_FUNCTION(Integer, lua_Integer)
#endif /* GLUA_ENABLE_STB */

int GluaReadNumber(lua_State *L, int idx, lua_Number *value) {
    *value = 0;
    if (!lua_isnumber(L, idx))
        return lua_isnil(L, idx) ? GLUA_NIL_ERROR : GLUA_TYPE_ERROR;

    int isnum;
    *value = lua_tonumberx(L, idx, &isnum);
    assert(isnum);

    return GLUA_OK;
}

void GluaFreeNumber(lua_State *L, lua_Number value) {
    (void)L;
    (void)(value);
}

int GluaPushNumber(lua_State *L, lua_Number value) {
    lua_pushnumber(L, value);
    return GLUA_OK;
}

GLUA_READ_ARRAY_FUNCTION(Number, lua_Number)
GLUA_PUSH_ARRAY_FUNCTION(Number, lua_Number)
GLUA_FREE_ARRAY_FUNCTION(Number, lua_Number)
#ifdef GLUA_ENABLE_STB
GLUA_READ_DARRAY_FUNCTION(Number, lua_Number)
GLUA_PUSH_DARRAY_FUNCTION(Number, lua_Number)
GLUA_FREE_DARRAY_FUNCTION(Number, lua_Number)
#endif /* GLUA_ENABLE_STB */

int GluaReadString(lua_State *L, int idx, char **value) {
    int status = GLUA_OK;
    *value = nullptr;
    // Operate on a copy of the value at `idx`.
    lua_pushvalue(L, idx);

    const char *s = lua_tostring(L, -1);
    if (s == nullptr)
        status = lua_isnil(L, -1) ? GLUA_NIL_ERROR : GLUA_TYPE_ERROR;
    else
        *value = strdup(s);

    lua_pop(L, 1);
    return status;
}

void GluaFreeString(lua_State *L, char *value) {
    (void)L;
    free(value);
}

int GluaPushString(lua_State *L, const char *value) {
    lua_pushstring(L, value);
    return GLUA_OK;
}

GLUA_READ_ARRAY_FUNCTION(String, char *)
GLUA_PUSH_ARRAY_FUNCTION(String, const char *)
GLUA_FREE_ARRAY_FUNCTION(String, char *)
#ifdef GLUA_ENABLE_STB
GLUA_READ_DARRAY_FUNCTION(String, char *)
GLUA_PUSH_DARRAY_FUNCTION(String, const char *)
GLUA_FREE_DARRAY_FUNCTION(String, char *)
#endif /* GLUA_ENABLE_STB */

int GluaReadLuaRef(lua_State *L, int idx, int *ref) {
    lua_pushvalue(L, idx);
    *ref = luaL_ref(L, LUA_REGISTRYINDEX);
    return GLUA_OK;
}

void GluaFreeLuaRef(lua_State *L, int value) {
    luaL_unref(L, LUA_REGISTRYINDEX, value);
}

int GluaPushLuaRef(lua_State *L, int value) {
    // This should never happen.
    if (value == LUA_NOREF)
        return GLUA_REF_ERROR;

    // Invalid reference.
    if (lua_geti(L, LUA_REGISTRYINDEX, value) == LUA_TNIL && value != LUA_REFNIL) {
        lua_pop(L, 1);
        return GLUA_REF_ERROR;
    }
    return GLUA_OK;
}

GLUA_READ_ARRAY_FUNCTION(LuaRef, int)
GLUA_PUSH_ARRAY_FUNCTION(LuaRef, int)
GLUA_FREE_ARRAY_FUNCTION(LuaRef, int)
#ifdef GLUA_ENABLE_STB
GLUA_READ_DARRAY_FUNCTION(LuaRef, int)
GLUA_PUSH_DARRAY_FUNCTION(LuaRef, int)
GLUA_FREE_DARRAY_FUNCTION(LuaRef, int)
#endif /* GLUA_ENABLE_STB */

int GluaReadBoolean(lua_State *L, int idx, bool *value) {
    *value = lua_toboolean(L, idx);
    return GLUA_OK;
}

void GluaFreeBoolean(lua_State *L, bool value) {
    (void)L;
    (void)value;
}

int GluaPushBoolean(lua_State *L, bool value) {
    lua_pushboolean(L, value);
    return GLUA_OK;
}

GLUA_READ_ARRAY_FUNCTION(Boolean, bool)
GLUA_PUSH_ARRAY_FUNCTION(Boolean, bool)
GLUA_FREE_ARRAY_FUNCTION(Boolean, bool)
#ifdef GLUA_ENABLE_STB
GLUA_READ_DARRAY_FUNCTION(Boolean, bool)
GLUA_PUSH_DARRAY_FUNCTION(Boolean, bool)
GLUA_FREE_DARRAY_FUNCTION(Boolean, bool)
#endif /* GLUA_ENABLE_STB */
