// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <lua.h>
#include <stb_ds.h>

typedef lua_Integer Integer;
typedef lua_Number Number;
typedef int LuaRef;
typedef bool Boolean;

/** Status codes used in generated code. */
enum GluaStatusCode {
    GLUA_OK = 0,
    // General error if no other error applies.
    GLUA_ERROR,
    // A non-optional field is `nil`.
    GLUA_NIL_ERROR,
    // The index field of a union type is missing or out of range.
    GLUA_INDEX_ERROR,
    // Reference doesn't exist.
    GLUA_REF_ERROR,
    // General type error, e.g. during conversion or when there is a type mismatch.
    GLUA_TYPE_ERROR,
};

// clang-format off
/**
 * Create the definition of a `GluaReadArray` function for the type `luatype` with the
 * corresponding type `ctype`.
 */
#define GLUA_READ_ARRAY_FUNCTION(luatype, ctype)                                       \
    int GluaReadArray##luatype(lua_State *L, int idx, ctype **values, size_t *count) { \
        size_t length = luaL_len(L, idx);                                              \
        *count = 0;                                                                    \
        *values = calloc(length, sizeof(**values));                                    \
        assert(*values);                                                               \
        for (size_t i = 0; i < length; i++) {                                          \
            lua_geti(L, -1, i + 1);                                                    \
            int status = GluaRead##luatype(L, -1, *values + i);                        \
            lua_pop(L, 1);                                                             \
            if (status != GLUA_OK) {                                                   \
                GluaFreeArray##luatype(L, *values, *count);                            \
                *values = nullptr;                                                     \
                *count = 0;                                                            \
                return status;                                                         \
            }                                                                          \
            (*count)++;                                                                \
        }                                                                              \
        return GLUA_OK;                                                                \
    }

/**
 * Create the definition of a `GluaReadDArray` function for the type `luatype` with the
 * corresponding type `ctype`.
 * For now, this is implemented by simply reading a regular array and copying the
 * values.
 */
#define GLUA_READ_DARRAY_FUNCTION(luatype, ctype)                                      \
    int GluaReadDArray##luatype(lua_State *L, int idx, ctype **values) {               \
        assert(*values == nullptr);                                                    \
        size_t scount = 0;                                                             \
        ctype *svalues = nullptr;                                                      \
        int status = GluaReadArray##luatype(L, idx, &svalues, &scount);                \
        if (status != GLUA_OK)                                                         \
            return status;                                                             \
        arrsetlen(*values, scount);                                                    \
        memcpy(*values, svalues, scount * sizeof(ctype));                              \
        free(svalues);                                                                 \
        return GLUA_OK;                                                                \
    }

/**
 * Create the definition of a `GluaFreeArray` function for the type `luatype` with the
 * corresponding type `ctype`.
 */
#define GLUA_FREE_ARRAY_FUNCTION(luatype, ctype)                                       \
    void GluaFreeArray##luatype(lua_State *L, ctype *values, size_t count) {           \
        for (size_t i = 0; i < count; i++)                                             \
            GluaFree##luatype(L, values[i]);                                           \
        free(values);                                                                  \
    }

/**
 * Create the definition of a `GluaFreeDArray` function for the type `luatype` with the
 * corresponding type `ctype`.
 */
#define GLUA_FREE_DARRAY_FUNCTION(luatype, ctype)                                      \
    void GluaFreeDArray##luatype(lua_State *L, ctype *values) {                        \
        for (size_t i = 0; i < arrlenu(values); i++)                                   \
            GluaFree##luatype(L, values[i]);                                           \
        arrfree(values);                                                               \
    }

/**
 * Create a definition of a `GluaPushArray` function for the type `luatype` with the
 * corresponding type `ctype`.
 */
#define GLUA_PUSH_ARRAY_FUNCTION(luatype, ctype)                                       \
    int GluaPushArray##luatype(lua_State *L, ctype *values, size_t size) {             \
        lua_newtable(L);                                                               \
        for (size_t i = 0; i < size; i++) {                                            \
            int status = GluaPush##luatype(L, values[i]);                              \
            if (status != GLUA_OK) {                                                   \
                lua_pop(L, 1);                                                         \
                return status;                                                         \
            }                                                                          \
            lua_seti(L, -2, i + 1);                                                    \
        }                                                                              \
        return GLUA_OK;                                                                \
    }

/**
 * Create a definition of a `GluaDPushArray` function for the type `luatype` with the
 * corresponding type `ctype`.
 */
#define GLUA_PUSH_DARRAY_FUNCTION(luatype, ctype)                                      \
    int GluaPushDArray##luatype(lua_State *L, ctype *values) {                         \
        return GluaPushArray##luatype(L, values, arrlenu(values));                     \
    }

// clang-format on

/** Initialize the Glua subsystem. */
int GluaInit(void);

/** Cleanup the Glua subsystem. */
void GluaCleanup(void);

/** Read an integer value from the stack index `idx`. */
int GluaReadInteger(lua_State *L, int idx, lua_Integer *value);

/** Read an integer array from the stack index `idx`. */
int GluaReadArrayInteger(lua_State *L, int idx, lua_Integer **values, size_t *count);

/** Read a dynamic (STB) integer array from the stack index `idx`. */
int GluaReadDArrayInteger(lua_State *L, int idx, lua_Integer **values);

/** Free an integer. */
void GluaFreeInteger(lua_State *L, lua_Integer value);

/** Free an integer array. */
void GluaFreeArrayInteger(lua_State *L, lua_Integer *values, size_t count);

/** Free a dynamic integer array. */
void GluaFreeDArrayInteger(lua_State *L, lua_Integer *values);

/** Push an integer value to the stack. */
int GluaPushInteger(lua_State *L, lua_Integer value);

/** Push an integer array to the stack. */
int GluaPushArrayInteger(lua_State *L, lua_Integer *values, size_t size);

/** Push a dynamic integer array to the stack. */
int GluaPushDArrayInteger(lua_State *L, lua_Integer *values);

/** Read a floating point value from the stack index `idx`. */
int GluaReadNumber(lua_State *L, int idx, lua_Number *value);

/** Read a floating point array from the stack index `idx`. */
int GluaReadArrayNumber(lua_State *L, int idx, lua_Number **values, size_t *count);

/** Read a dynamic (STB) floating point array from the stack index `idx`. */
int GluaReadDArrayNumber(lua_State *L, int idx, lua_Number **values);

/** Free a number. */
void GluaFreeNumber(lua_State *L, lua_Number value);

/** Free a number array. */
void GluaFreeArrayNumber(lua_State *L, lua_Number *values, size_t count);

/** Free a dynamic number array. */
void GluaFreeDArrayNumber(lua_State *L, lua_Number *values);

/** Push a floating point value to the stack. */
int GluaPushNumber(lua_State *L, lua_Number value);

/** Push a number array to the stack. */
int GluaPushArrayNumber(lua_State *L, lua_Number *values, size_t size);

/** Push a dynamic number array to the stack. */
int GluaPushDArrayNumber(lua_State *L, lua_Number *values);

/**
 * Read a string value from the stack index `idx`.
 *
 * The resulting string has to be freed later. If an error occurs, then the result
 * `value` is set to `nullptr`.
 */
int GluaReadString(lua_State *L, int idx, char **value);

/** Read a string array from the stack index `idx`. */
int GluaReadArrayString(lua_State *L, int idx, char ***values, size_t *count);

/** Read a dynamic (STB) string array from the stack index `idx`. */
int GluaReadDArrayString(lua_State *L, int idx, char ***values);

/** Free a string. */
void GluaFreeString(lua_State *L, char *value);

/** Free a string array. */
void GluaFreeArrayString(lua_State *L, char **values, size_t count);

/** Free a dynamic string array. */
void GluaFreeDArrayString(lua_State *L, char **values);

/** Push a string value to the stack. */
int GluaPushString(lua_State *L, const char *value);

/** Push a string array to the stack. */
int GluaPushArrayString(lua_State *L, const char **values, size_t size);

/** Push a dynamic string array to the stack. */
int GluaPushDArrayString(lua_State *L, const char **values);

/**
 * Read an arbitrary Lua value from the stack index `idx` and create a reference to it
 * by calling `luaL_ref()`.
 *
 * Implementation detail:
 * Since `luaL_ref()` operates on the top of the stack and removes the object, it is
 * first copied to the top.
 */
int GluaReadLuaRef(lua_State *L, int idx, int *ref);

/** Read a LuaRef array from the stack index `idx`. */
int GluaReadArrayLuaRef(lua_State *L, int idx, int **values, size_t *count);

/** Read a dynamic (STB) LuaRef array from the stack index `idx`. */
int GluaReadDArrayLuaRef(lua_State *L, int idx, int **values);

/** Free a reference by calling `luaL_unref()`. */
void GluaFreeLuaRef(lua_State *L, int value);

/** Free a LuaRef array. */
void GluaFreeArrayLuaRef(lua_State *L, int *values, size_t count);

/** Free a dynamic LuaRef array. */
void GluaFreeDArrayLuaRef(lua_State *L, int *values);

/**
 * Push LuaRef value to the stack.
 *
 * This reads the ref from the `LUA_REGISTRYINDEX` table and pushes it.
 */
int GluaPushLuaRef(lua_State *L, int value);

/** Push a LuaRef array to the stack. */
int GluaPushArrayLuaRef(lua_State *L, int *values, size_t size);

/** Push a dynamic LuaRef array to the stack. */
int GluaPushDArrayLuaRef(lua_State *L, int *values);

/** Read a boolean value from the stack index `idx`. */
int GluaReadBoolean(lua_State *L, int idx, bool *value);

/** Read a boolean array from the stack index `idx`. */
int GluaReadArrayBoolean(lua_State *L, int idx, bool **values, size_t *count);

/** Read a dynamic (STB) boolean array from the stack index `idx`. */
int GluaReadDArrayBoolean(lua_State *L, int idx, bool **values);

/** Free a boolean. */
void GluaFreeBoolean(lua_State *L, bool value);

/** Free a boolean array. */
void GluaFreeArrayBoolean(lua_State *L, bool *values, size_t count);

/** Free a dynamic boolean array. */
void GluaFreeDArrayBoolean(lua_State *L, bool *values);

/** Push a boolean value to the stack. */
int GluaPushBoolean(lua_State *L, bool value);

/** Push a boolean array to the stack. */
int GluaPushArrayBoolean(lua_State *L, bool *values, size_t size);

/** Push a dynamic boolean array to the stack. */
int GluaPushDArrayBoolean(lua_State *L, bool *values);
