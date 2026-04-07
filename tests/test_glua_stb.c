// SPDX-License-Identifier: Apache-2.0

#include "gluatypes.h"
#include <generated/glua_stb.h>
#include <glua/glua.h>
#include <lauxlib.h>
#include <lua.h>
#include <stb_ds.h>
#include <unity.h>

// State of the Lua interpreter used throughout tests.
lua_State *L = nullptr;

void setUp(void) {
    L = luaL_newstate();
}

void tearDown(void) {
    lua_close(L);
    L = nullptr;
}

// Number of entries in the LUA_REGISTRYINDEX table.
int refcount(void) {
    int count = 0;
    lua_pushnil(L);
    while (lua_next(L, LUA_REGISTRYINDEX)) {
        count++;
        lua_pop(L, 1);
    }
    return count;
}

// Try to read a type that contains a dynamic array.
void test_darray_type(void) {
    DArrayType tp;
    const char *program = "return {"
                          "  values = {42, 43, 44, 45}"
                          "}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadDArrayType(L, -1, &tp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(4, arrlen(tp.values));
    TEST_ASSERT_EQUAL(42, tp.values[0]);
    TEST_ASSERT_EQUAL(43, tp.values[1]);
    TEST_ASSERT_EQUAL(44, tp.values[2]);
    TEST_ASSERT_EQUAL(45, tp.values[3]);

    GluaFreeDArrayType(L, tp);
}

// Read a dynamic array directly from the stack.
void test_read_darray_integer(void) {
    lua_Integer *values = nullptr;
    const char *program = "return {42, 43, 44, 45}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadDArrayInteger(L, -1, &values));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(4, arrlen(values));
    TEST_ASSERT_EQUAL(42, values[0]);
    TEST_ASSERT_EQUAL(43, values[1]);
    TEST_ASSERT_EQUAL(44, values[2]);
    TEST_ASSERT_EQUAL(45, values[3]);

    GluaFreeDArrayInteger(L, values);
}

// Read a dynamic string array directly from the stack.
void test_read_darray_string(void) {
    char **values = nullptr;
    const char *program = "return {'A', 'B', 'C', 'D'}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadDArrayString(L, -1, &values));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(4, arrlen(values));
    TEST_ASSERT_EQUAL_STRING("A", values[0]);
    TEST_ASSERT_EQUAL_STRING("B", values[1]);
    TEST_ASSERT_EQUAL_STRING("C", values[2]);
    TEST_ASSERT_EQUAL_STRING("D", values[3]);

    GluaFreeDArrayString(L, values);
}

void test_push_darray_integer(void) {
    int top = lua_gettop(L);
    lua_Integer wvalues[] = {42, 43, 44, 45, 46, 47, 48};
    size_t wsize = sizeof(wvalues) / sizeof(wvalues[0]);
    lua_Integer *warr = nullptr;
    arrsetlen(warr, wsize);
    memcpy(warr, wvalues, sizeof(wvalues));

    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushDArrayInteger(L, warr));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    lua_Integer *rvalues;
    size_t rsize;
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayInteger(L, -1, &rvalues, &rsize));
    TEST_ASSERT_EQUAL(wsize, rsize);
    TEST_ASSERT_EQUAL_INT64_ARRAY(wvalues, rvalues, rsize);

    GluaFreeArrayInteger(L, rvalues, rsize);
    arrfree(warr);
}

void test_push_darray_base_type(void) {
    int top = lua_gettop(L);
    BaseType wvalues[] = {
        {.valueInteger = 42, .valueNumber = 42.42, .valueString = "value42"},
        {.valueInteger = 43, .valueNumber = 43.43, .valueString = "value43"},
    };
    size_t wsize = sizeof(wvalues) / sizeof(wvalues[0]);
    BaseType *warr = nullptr;
    arrsetlen(warr, wsize);
    memcpy(warr, wvalues, sizeof(wvalues));

    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushDArrayBaseType(L, warr));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    BaseType *rvalues;
    size_t rsize;
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayBaseType(L, -1, &rvalues, &rsize));
    TEST_ASSERT_EQUAL(wsize, rsize);

    for (size_t i = 0; i < rsize; i++) {
        TEST_ASSERT_EQUAL_INT64(wvalues[i].valueInteger, rvalues[i].valueInteger);
        TEST_ASSERT_EQUAL_FLOAT(wvalues[i].valueNumber, rvalues[i].valueNumber);
        TEST_ASSERT_EQUAL_STRING(wvalues[i].valueString, rvalues[i].valueString);
    }

    GluaFreeArrayBaseType(L, rvalues, rsize);
    arrfree(warr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_darray_type);
    RUN_TEST(test_read_darray_integer);
    RUN_TEST(test_read_darray_string);
    RUN_TEST(test_push_darray_integer);
    RUN_TEST(test_push_darray_base_type);

    return UNITY_END();
}
