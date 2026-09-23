{
  description = "A glue code generator for C and Lua.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      perSystem = { pkgs, ... }: {
        # Package configuration.
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "glua";
          version = "unstable";

          src = ./.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
          ];

          buildInputs = with pkgs; [
            lua5_5
            stb
            (python3.withPackages (python-pkgs: [
              python-pkgs.mako
            ]))
          ];
        };

        # Developer configuration.
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            clang-tools
            cmake
            ninja
            pkg-config
            pre-commit
            rustup
          ];

          buildInputs = with pkgs; [
            lua5_5
            stb
            (python3.withPackages (python-pkgs: [
              python-pkgs.mako
            ]))
          ];
        };
      };
    };
}
