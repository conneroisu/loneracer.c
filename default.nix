{pkgs ? import <nixpkgs> {}}:
pkgs.stdenv.mkDerivation {
  pname = "loneracer2";
  version = "0.1.0";

  src = ./.;

  buildInputs = with pkgs; [
    ncurses
  ];

  installPhase = ''
    mkdir -p $out/bin
    cp loneracer2 $out/bin/
    chmod +x $out/bin/loneracer2
  '';

  meta = with pkgs.lib; {
    description = "A quick lone racer game mockup which demonstrates basic ncurses functionality";
    mainProgram = "loneracer2";
    platforms = platforms.unix;
    license = licenses.mit; # Assuming MIT license, adjust if needed
  };
}
