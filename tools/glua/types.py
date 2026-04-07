# SPDX-License-Identifier: Apache-2.0

from dataclasses import dataclass
from typing import Any, ClassVar, Sequence


class GluaError(Exception):
    """Error during code generation."""


class BaseDef:
    """Base type for all definitions."""

    ALLOWED_FIELDS: ClassVar[set[str]]
    REQUIRED_FIELDS: ClassVar[set[str]]

    @classmethod
    def check_fields(cls, doc: dict[str, Any]) -> None:
        """Check if the fields in the document match the specification."""
        if not set.issubset(set(doc), cls.ALLOWED_FIELDS):
            invalid = set(doc) - cls.ALLOWED_FIELDS
            raise GluaError(f"invalid fields(s) in type description: {invalid}")

        if not set.issubset(cls.REQUIRED_FIELDS, set(doc)):
            missing = set(cls.REQUIRED_FIELDS) - set(doc)
            raise GluaError(f"missing required field(s): {missing}")


@dataclass
class FieldDef(BaseDef):
    """Definition of a scalar field."""

    name: str
    """Name of the field."""
    type: str
    """Type of the field."""
    optional: bool = False
    """The field can be missing."""

    ALLOWED_FIELDS: ClassVar[set[str]] = {"name", "type", "optional"}
    REQUIRED_FIELDS: ClassVar[set[str]] = {"name", "type"}

    @classmethod
    def from_yaml(cls, doc: dict[str, Any]) -> FieldDef:
        """Create a field description from the YAML field."""
        cls.check_fields(doc)
        return cls(**doc)


@dataclass
class ArrayDef(BaseDef):
    """Definition of an array field."""

    name: str
    """Name of the field."""
    type: str
    """Type of the field."""
    length: str | None = None
    """Name of the length field."""
    optional: bool = False
    """The field can be missing."""
    dynamic: bool = False
    """The array is dynamic. This disallows the `length` field."""

    ALLOWED_FIELDS: ClassVar[set[str]] = {
        "name",
        "type",
        "optional",
        "length",
        "dynamic",
    }
    REQUIRED_FIELDS: ClassVar[set[str]] = {"name", "type"}

    @classmethod
    def from_yaml(cls, doc: dict[str, Any]) -> ArrayDef:
        """Create a field description from the YAML document."""
        cls.check_fields(doc)
        obj = cls(**doc)
        if obj.length is None and not obj.dynamic:
            raise GluaError("static array defined without 'length' field")
        elif obj.length is not None and obj.dynamic:
            raise GluaError("dynamic array defined with 'length' field")
        return obj


@dataclass
class UnionDef(BaseDef):
    """Definition of a union field.

    This also works for regular structs with a certain structure.
    Basic setup in C:
    ```
    typedef struct <name> {
        int <index>;
        union {
            <types[0]> <fields[0]>;
            ...
            <types[n]> <fields[n]>;
        };
    } <name>;
    ```
    In Lua, the type is simply a table of the requested type with an additional field
    with an `<index>` variable.
    ```
    { <index> = 0, <name> = <some value>}
    ```
    So for the example above, the read-code would determine the type by reading
    `<index>` from the Lua table and then call the appropriate read-function for
    `types[<index>]` and assign it to `fields[<index>]`.
    """

    name: str
    """Name of the field in Lua."""
    types: list[str]
    """List of strings of possible types."""
    fields: list[str]
    """List of variable names where each of the above types is assigned."""
    index: str
    """Name of the index variable in the Lua table and the C struct."""
    optional: bool = False
    """The field can be missing.

    The value of `<index>` is set to `-1` in this case.
    """
    enum: bool = True
    """Generate an enum for the index.

    The following naming scheme is used: `<type>_<name>_<fieldtype>` in uppercase.
    """

    ALLOWED_FIELDS: ClassVar[set[str]] = {
        "name",
        "types",
        "fields",
        "index",
        "optional",
        "enum",
    }
    REQUIRED_FIELDS: ClassVar[set[str]] = {"name", "types", "fields", "index"}

    @classmethod
    def from_yaml(cls, doc: dict[str, Any]) -> ArrayDef:
        """Create a field description from the YAML document."""
        cls.check_fields(doc)
        if len(doc["types"]) != len(doc["fields"]):
            raise GluaError("'types' and 'fields' have different length")
        return cls(**doc)


@dataclass
class TypeDef(BaseDef):
    """Definition of a type."""

    name: str
    headers: list[str]
    fields: list[FieldDef]
    arrays: list[ArrayDef]
    unions: list[UnionDef]

    ALLOWED_FIELDS: ClassVar[set[str]] = {
        "name",
        "headers",
        "fields",
        "arrays",
        "unions",
    }
    REQUIRED_FIELDS: ClassVar[set[str]] = {"name"}

    @classmethod
    def from_yaml(cls, doc: dict[str, Any]) -> TypeDef:
        """Create a type description from the YAML document."""
        cls.check_fields(doc)
        return cls(
            name=doc["name"],
            headers=doc.get("headers", []),
            fields=[FieldDef.from_yaml(f) for f in doc.get("fields", [])],
            arrays=[ArrayDef.from_yaml(f) for f in doc.get("arrays", [])],
            unions=[UnionDef.from_yaml(f) for f in doc.get("unions", [])],
        )

    @property
    def allfields(self) -> Sequence[FieldDef | ArrayDef | UnionDef]:
        """Get all fields of the type."""
        return tuple(self.fields + self.arrays + self.unions)
