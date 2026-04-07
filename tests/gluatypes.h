// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <glua/glua.h>
#include <stddef.h>

typedef struct BaseType {
    int valueInteger;
    float valueNumber;
    char *valueString;
} BaseType;

typedef struct ComplexType {
    BaseType baseA;
    BaseType baseB;
} ComplexType;

typedef struct BaseTypeOpt {
    int valueInteger;
    float valueNumber;
    char *valueString;
} BaseTypeOpt;

typedef struct ComplexTypeOptNoOpt {
    BaseType baseA;
    BaseType baseB;
} ComplexTypeOptNoOpt;

typedef struct ComplexTypeOptOpt {
    BaseTypeOpt baseA;
    BaseTypeOpt baseB;
} ComplexTypeOptOpt;

typedef struct ArrayType {
    size_t valuesCount;
    Integer *values;
} ArrayType;

typedef struct DArrayType {
    Integer *values;
} DArrayType;

typedef struct ArrayTypeOpt {
    size_t valuesCount;
    Integer *values;
} ArrayTypeOpt;

typedef struct UnionType {
    int index;
    union {
        Integer valueInteger;
        Number valueNumber;
        char *valueString;
        BaseType valueBaseType;
    };
} UnionType;

typedef struct UnionTypeOpt {
    int index;
    union {
        Integer valueInteger;
        Number valueNumber;
    };
} UnionTypeOpt;
