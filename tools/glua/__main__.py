# SPDX-License-Identifier: Apache-2.0

import argparse
import os
import sys

import yaml
from mako.template import Template

from .types import TypeDef

DEFAULT_HEADERS = {
    "c": set(
        [
            "glua/glua.h",
            "string.h",
            "assert.h",
            "stdlib.h",
            "stdio.h",
            "stddef.h",
            "lauxlib.h",
        ]
    ),
    "zig": set(
        [
            "zlua",
            "std",
        ]
    ),
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--language", "-l", default="c", choices=["c", "zig"])
    parser.add_argument("schema", nargs="+", help="input YAML schema files")
    args = parser.parse_args()

    dirname = os.path.dirname(__file__)
    template = Template(filename=os.path.join(dirname, f"{args.language}.mako"))

    types = []
    for path in args.schema:
        with open(path) as fp:
            docs = yaml.safe_load_all(fp)
            for doc in docs:
                types.append(TypeDef.from_yaml(doc))

    print(template.render(default_headers=DEFAULT_HEADERS[args.language], types=types))

    return 0


if __name__ == "__main__":
    sys.exit(main())
