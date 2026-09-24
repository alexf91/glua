{
  description = "A glue code generator for C and Lua.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      forAllSystems =
        function:
        nixpkgs.lib.genAttrs [ "x86_64-linux" "aarch64-linux" ] (
          system: function nixpkgs.legacyPackages.${system}
        );

      # Common dependencies for the package and the dev shell.
      deps = pkgs: {
        nativeBuildInputs = with pkgs; [
          cmake
          ninja
          pkg-config
        ];

        propagatedBuildInputs = with pkgs; [
          lua5_5
          stb
          (python3.withPackages (python-pkgs: [
            python-pkgs.mako
            python-pkgs.pyyaml
          ]))
        ];
      };
    in
    {
      # Generate packages for all systems.
      packages = forAllSystems (
        pkgs:
        let
          inherit (deps pkgs) nativeBuildInputs propagatedBuildInputs;
        in
        {
          default = pkgs.stdenv.mkDerivation {
            pname = "glua";
            version = "0.5.0";
            src = ./.;
            inherit nativeBuildInputs propagatedBuildInputs;
          };
        }
      );

      # Generate dev shells for all systems.
      devShells = forAllSystems (
        pkgs:
        let
          inherit (deps pkgs) nativeBuildInputs propagatedBuildInputs;
        in
        {
          default = pkgs.mkShell {
            packages =
              nativeBuildInputs
              ++ (with pkgs; [
                clang-tools
                pre-commit
                rustup
              ]);
            inherit propagatedBuildInputs;
          };
        }
      );
    };
}
