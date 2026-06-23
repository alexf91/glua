// SPDX-License-Identifier: Apache-2.0

#include "gluatypes.h"
#include <generated/glua.h>
#include <glua/glua.h>
#include <lauxlib.h>
#include <lua.h>
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

// Read a basic type from the Lua stack.
void test_base_type(void) {
    BaseType tp;
    const char *program = "return {"
                          "  valueInteger = 42,"
                          "  valueNumber = 43.44,"
                          "  valueString = 'FOO'"
                          "}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadBaseType(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(42, tp.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(43.44, tp.valueNumber);
    TEST_ASSERT_EQUAL_STRING("FOO", tp.valueString);

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    GluaFreeBaseType(L, tp);
}

// Read a basic type with optional fields from the Lua stack.
void test_base_type_opt(void) {
    BaseTypeOpt tp;
    const char *program = "return {}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadBaseTypeOpt(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(0, tp.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(0.0, tp.valueNumber);
    TEST_ASSERT_NULL(tp.valueString);

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    GluaFreeBaseTypeOpt(L, tp);
}

// Read a complex type from the Lua stack.
void test_complex_type(void) {
    ComplexType tp;
    const char *program = "return {"
                          "  baseA = {"
                          "    valueInteger = 42,"
                          "    valueNumber = 43.44,"
                          "    valueString = 'FOO'"
                          "  },"
                          "  baseB = {"
                          "    valueInteger = 45,"
                          "    valueNumber = 46.47,"
                          "    valueString = 'BAR'"
                          "  },"
                          "}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadComplexType(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(42, tp.baseA.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(43.44, tp.baseA.valueNumber);
    TEST_ASSERT_EQUAL_STRING("FOO", tp.baseA.valueString);

    TEST_ASSERT_EQUAL_INT(45, tp.baseB.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(46.47, tp.baseB.valueNumber);
    TEST_ASSERT_EQUAL_STRING("BAR", tp.baseB.valueString);

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    GluaFreeComplexType(L, tp);
}

// Read a complex type from the Lua stack.
// Optional values are missing completely, so the read function is successful.
void test_complex_type_opt_noopt(void) {
    ComplexTypeOptNoOpt tp;
    const char *program = "return {}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadComplexTypeOptNoOpt(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(0, tp.baseA.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(0.0, tp.baseA.valueNumber);
    TEST_ASSERT_NULL(tp.baseA.valueString);

    TEST_ASSERT_EQUAL_INT(0, tp.baseB.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(0.0, tp.baseB.valueNumber);
    TEST_ASSERT_NULL(tp.baseB.valueString);

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    GluaFreeComplexTypeOptNoOpt(L, tp);
}

// Read a complex type from the Lua stack.
// Optional values are incomplete, so the read function produces an error.
void test_complex_type_opt_noopt_incomplete(void) {
    ComplexTypeOptNoOpt tp;
    const char *program = "return {baseA = {valueInteger = 42}, baseB = {}}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_NIL_ERROR, GluaReadComplexTypeOptNoOpt(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(0, tp.baseA.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(0.0, tp.baseA.valueNumber);
    TEST_ASSERT_NULL(tp.baseA.valueString);

    TEST_ASSERT_EQUAL_INT(0, tp.baseB.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(0.0, tp.baseB.valueNumber);
    TEST_ASSERT_NULL(tp.baseB.valueString);

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    GluaFreeComplexTypeOptNoOpt(L, tp);
}

// Read a complex type from the Lua stack.
// Optional values are incomplete but also optional, so the read function succeeds.
void test_complex_type_opt_opt_incomplete(void) {
    ComplexTypeOptOpt tp;
    const char *program = "return {"
                          "  baseA = {valueInteger = 42},"
                          "  baseB = {valueString = \"FOO\"}"
                          "}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadComplexTypeOptOpt(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(42, tp.baseA.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(0.0, tp.baseA.valueNumber);
    TEST_ASSERT_NULL(tp.baseA.valueString);

    TEST_ASSERT_EQUAL_INT(0, tp.baseB.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(0.0, tp.baseB.valueNumber);
    TEST_ASSERT_EQUAL_STRING("FOO", tp.baseB.valueString);

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    GluaFreeComplexTypeOptOpt(L, tp);
}

// Try to get a non-optional field that is `nil`.
void test_base_nil_error(void) {
    BaseType tp;
    const char *program = "return {}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_NIL_ERROR, GluaReadBaseType(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_TRUE(lua_istable(L, -1));

    GluaFreeBaseType(L, tp);
}

// Try to read from a type that is not a table.
void test_base_type_error(void) {
    BaseType tp;
    const char *program = "return 123";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_TYPE_ERROR, GluaReadBaseType(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_TRUE(lua_isinteger(L, -1));

    GluaFreeBaseType(L, tp);
}

// Try to read a type that contains an array.
void test_array_type(void) {
    ArrayType tp;
    const char *program = "return {"
                          "  values = {42, 43, 44, 45}"
                          "}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayType(L, -1, &tp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(4, tp.valuesCount);
    TEST_ASSERT_EQUAL(42, tp.values[0]);
    TEST_ASSERT_EQUAL(43, tp.values[1]);
    TEST_ASSERT_EQUAL(44, tp.values[2]);
    TEST_ASSERT_EQUAL(45, tp.values[3]);

    GluaFreeArrayType(L, tp);
}

// Try to read a type that contains an array, but the array is nil.
void test_array_type_nil_error(void) {
    ArrayType tp;
    const char *program = "return {}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_NIL_ERROR, GluaReadArrayType(L, -1, &tp));

    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_TRUE(lua_istable(L, -1));

    GluaFreeArrayType(L, tp);
}

// Try to read a type that contains an optional array.
void test_array_type_opt(void) {
    ArrayTypeOpt tp;
    const char *program = "return {}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayTypeOpt(L, -1, &tp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(0, tp.valuesCount);
    TEST_ASSERT_NULL(tp.values);

    GluaFreeArrayTypeOpt(L, tp);
}

// Read an array directly from the stack.
void test_read_array_integer(void) {
    lua_Integer *values;
    size_t count;
    const char *program = "return {42, 43, 44, 45}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayInteger(L, -1, &values, &count));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(4, count);
    TEST_ASSERT_EQUAL(42, values[0]);
    TEST_ASSERT_EQUAL(43, values[1]);
    TEST_ASSERT_EQUAL(44, values[2]);
    TEST_ASSERT_EQUAL(45, values[3]);

    GluaFreeArrayInteger(L, values, count);
}

// Read a string array directly from the stack.
void test_read_array_string(void) {
    char **values;
    size_t count;
    const char *program = "return {'A', 'B', 'C', 'D'}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayString(L, -1, &values, &count));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(4, count);
    TEST_ASSERT_EQUAL_STRING("A", values[0]);
    TEST_ASSERT_EQUAL_STRING("B", values[1]);
    TEST_ASSERT_EQUAL_STRING("C", values[2]);
    TEST_ASSERT_EQUAL_STRING("D", values[3]);

    GluaFreeArrayString(L, values, count);
}

void test_union_type(void) {
    UnionType tp;
    const char *program = "return {index = 1, value = 42.43}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadUnionType(L, -1, &tp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(UNIONTYPE_VALUE_NUMBER, tp.index);
    TEST_ASSERT_EQUAL_FLOAT(42.43, tp.valueNumber);

    GluaFreeUnionType(L, tp);
}

void test_union_type_opt(void) {
    UnionTypeOpt tp;
    const char *program = "return {index = 1}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadUnionTypeOpt(L, -1, &tp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    TEST_ASSERT_EQUAL(-1, tp.index);
    GluaFreeUnionTypeOpt(L, tp);
}

void test_union_type_index_error(void) {
    UnionType tp;
    const char *program = "return {index = 8, value = 42.43}";
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(LUA_OK, luaL_dostring(L, program));
    TEST_ASSERT_EQUAL(GLUA_INDEX_ERROR, GluaReadUnionType(L, -1, &tp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    GluaFreeUnionType(L, tp);
}

void test_push_integer(void) {
    lua_Integer value;
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushInteger(L, 42));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadInteger(L, -1, &value));
    TEST_ASSERT_EQUAL_INT(42, value);

    GluaFreeInteger(L, value);
}

void test_push_number(void) {
    lua_Number value;
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushNumber(L, 42.4344));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadNumber(L, -1, &value));
    TEST_ASSERT_EQUAL_FLOAT(42.4344, value);

    GluaFreeNumber(L, value);
}

void test_push_string(void) {
    char *value;
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushString(L, "foobar"));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadString(L, -1, &value));
    TEST_ASSERT_EQUAL_STRING("foobar", value);

    GluaFreeString(L, value);
}

void test_push_string_nullptr(void) {
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushString(L, nullptr));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_TRUE(lua_isnil(L, -1));
}

void test_push_luaref_nil(void) {
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushLuaRef(L, LUA_REFNIL));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_TRUE(lua_isnil(L, -1));
}

void test_push_luaref_nil_new(void) {
    int top = lua_gettop(L);
    lua_pushnil(L);
    int ref = luaL_ref(L, -1);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushLuaRef(L, ref));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));
    TEST_ASSERT_TRUE(lua_isnil(L, -1));
}

void test_push_luaref_noref(void) {
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_REF_ERROR, GluaPushLuaRef(L, LUA_NOREF));
    TEST_ASSERT_EQUAL_INT(top, lua_gettop(L));
}

void test_push_luaref_invalid(void) {
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_REF_ERROR, GluaPushLuaRef(L, 424242));
    TEST_ASSERT_EQUAL_INT(top, lua_gettop(L));
}

void test_push_array_integer(void) {
    int top = lua_gettop(L);
    lua_Integer wvalues[] = {42, 43, 44, 45, 46, 47, 48};
    size_t wsize = sizeof(wvalues) / sizeof(wvalues[0]);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushArrayInteger(L, wvalues, wsize));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    lua_Integer *rvalues;
    size_t rsize;
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayInteger(L, -1, &rvalues, &rsize));
    TEST_ASSERT_EQUAL(wsize, rsize);
    TEST_ASSERT_EQUAL_INT64_ARRAY(wvalues, rvalues, rsize);

    GluaFreeArrayInteger(L, rvalues, rsize);
}

void test_push_array_integer_nullptr(void) {
    int top = lua_gettop(L);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushArrayInteger(L, nullptr, 0));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    lua_Integer *rvalues;
    size_t rsize;
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadArrayInteger(L, -1, &rvalues, &rsize));
    TEST_ASSERT_EQUAL(0, rsize);

    GluaFreeArrayInteger(L, rvalues, rsize);
}

void test_push_base_type(void) {
    int top = lua_gettop(L);
    BaseType wtp = {.valueInteger = 42, .valueNumber = 3.1415, .valueString = "foobar"};
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushBaseType(L, wtp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    BaseType rtp;
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadBaseType(L, -1, &rtp));
    TEST_ASSERT_EQUAL_INT(42, rtp.valueInteger);
    TEST_ASSERT_EQUAL_FLOAT(3.1415, rtp.valueNumber);
    TEST_ASSERT_EQUAL_STRING("foobar", rtp.valueString);

    GluaFreeBaseType(L, rtp);
}

void test_push_array_base_type(void) {
    int top = lua_gettop(L);
    BaseType wvalues[] = {
        {.valueInteger = 42, .valueNumber = 42.42, .valueString = "value42"},
        {.valueInteger = 43, .valueNumber = 43.43, .valueString = "value43"},
    };
    size_t wsize = sizeof(wvalues) / sizeof(wvalues[0]);
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushArrayBaseType(L, wvalues, wsize));
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
}

void test_push_union_type(void) {
    int top = lua_gettop(L);
    UnionType wtp = {.index = UNIONTYPE_VALUE_NUMBER, .valueNumber = 3.1415};
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushUnionType(L, wtp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    UnionType rtp;
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadUnionType(L, -1, &rtp));
    TEST_ASSERT_EQUAL(wtp.index, rtp.index);
    TEST_ASSERT_EQUAL_FLOAT(wtp.valueNumber, rtp.valueNumber);

    GluaFreeUnionType(L, rtp);
}

void test_push_union_type_index_error(void) {
    int top = lua_gettop(L);
    UnionType wtp;

    wtp.index = -1;
    TEST_ASSERT_EQUAL(GLUA_INDEX_ERROR, GluaPushUnionType(L, wtp));
    TEST_ASSERT_EQUAL_INT(top, lua_gettop(L));

    wtp.index = -42;
    TEST_ASSERT_EQUAL(GLUA_INDEX_ERROR, GluaPushUnionType(L, wtp));
    TEST_ASSERT_EQUAL_INT(top, lua_gettop(L));
}

void test_free_union_type_opt_absent(void) {
    UnionTypeOpt tp = {.index = -1};
    GluaFreeUnionTypeOpt(L, tp);
}

void test_push_union_type_opt(void) {
    int top = lua_gettop(L);
    UnionTypeOpt wtp = {.index = -1};
    TEST_ASSERT_EQUAL(GLUA_OK, GluaPushUnionTypeOpt(L, wtp));
    TEST_ASSERT_EQUAL_INT(top + 1, lua_gettop(L));

    UnionTypeOpt rtp;
    TEST_ASSERT_EQUAL(GLUA_OK, GluaReadUnionTypeOpt(L, -1, &rtp));
    TEST_ASSERT_EQUAL(wtp.index, rtp.index);

    GluaFreeUnionTypeOpt(L, rtp);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_base_type);
    RUN_TEST(test_complex_type);
    RUN_TEST(test_base_type_opt);
    RUN_TEST(test_complex_type_opt_noopt);
    RUN_TEST(test_complex_type_opt_noopt_incomplete);
    RUN_TEST(test_complex_type_opt_opt_incomplete);
    RUN_TEST(test_base_nil_error);
    RUN_TEST(test_base_type_error);
    RUN_TEST(test_array_type);
    RUN_TEST(test_array_type_nil_error);
    RUN_TEST(test_array_type_opt);
    RUN_TEST(test_read_array_integer);
    RUN_TEST(test_read_array_string);
    RUN_TEST(test_union_type);
    RUN_TEST(test_union_type_opt);
    RUN_TEST(test_union_type_index_error);

    RUN_TEST(test_push_integer);
    RUN_TEST(test_push_number);
    RUN_TEST(test_push_string);
    RUN_TEST(test_push_string_nullptr);
    RUN_TEST(test_push_luaref_nil);
    RUN_TEST(test_push_luaref_nil_new);
    RUN_TEST(test_push_luaref_noref);
    RUN_TEST(test_push_luaref_invalid);
    RUN_TEST(test_push_array_integer);
    RUN_TEST(test_push_array_integer_nullptr);
    RUN_TEST(test_push_base_type);
    RUN_TEST(test_push_array_base_type);
    RUN_TEST(test_push_union_type);
    RUN_TEST(test_push_union_type_index_error);
    RUN_TEST(test_push_union_type_opt);
    RUN_TEST(test_free_union_type_opt_absent);

    return UNITY_END();
}
