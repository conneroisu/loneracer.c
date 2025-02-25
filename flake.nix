{
  description = "Lone Racer 2 - ncurses-based racing game";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = {
          default = self.packages.${system}.loneracer2;
          loneracer2 = pkgs.stdenv.mkDerivation {
            pname = "loneracer2";
            version = "0.1.0";

            src = ./.;

            nativeBuildInputs = with pkgs; [
              pkg-config
            ];

            buildInputs = with pkgs; [
              ncurses
            ];

            buildPhase = ''
              $CC -Wall -Wextra -o loneracer2 loneracer2.c -lncurses
            '';

            installPhase = ''
              mkdir -p $out/bin
              cp loneracer2 $out/bin/
              chmod +x $out/bin/loneracer2
            '';

            meta = with pkgs.lib; {
              description = "A quick lone racer game mockup which demonstrates basic ncurses functionality";
              homepage = "https://github.com/yourusername/loneracer2"; # Replace with your actual repository if applicable
              mainProgram = "loneracer2";
              platforms = platforms.unix;
              license = licenses.mit; # Assuming MIT license, adjust if needed
            };
          };
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            ncurses
            gcc
            gnumake
          ];
        };
      }
    );
}
