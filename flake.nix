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

      perSystem = { pkgs, ... }:
        let
          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
          ];

          # Dependencies inherited by downstream consumers.
          # Those are always necessary for consumers.
          propagatedBuildInputs = with pkgs; [
            lua5_5
            (python3.withPackages (python-pkgs: [
              python-pkgs.mako
              python-pkgs.pyyaml
            ]))
          ];

          buildInputs = with pkgs; [
            stb
          ];
        in
        {
          # Package configuration.
          packages.default = pkgs.stdenv.mkDerivation {
            pname = "glua";
            version = "0.5.0";
            src = ./.;

            inherit nativeBuildInputs propagatedBuildInputs buildInputs;
          };

          # Developer configuration.
          devShells.default = pkgs.mkShell {
            packages =
              nativeBuildInputs
              ++ (with pkgs; [
                clang-tools
                pre-commit
                rustup
              ]);

            inherit propagatedBuildInputs buildInputs;
          };
        };
    };
}
