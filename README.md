# MegaDriveSnake
My version of game "Snake" written in C that compiles into a SEGA Mega Drive (Genesis) ROM that can be played on an emulator.

## How to compile:
* Following SGDK installation instructions (NB: makelib.gen is important).
* Build the project in Visual Studio or by running:
```%GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen```.
* This should output `rom.bin` in `./Megadrive/out/`.

## How to play:
* Load `rom.bin` in the emulator of your choice.

## Uses:
* [SGDK v1.34 library](https://github.com/Stephane-D/SGDK).
* Tested in Kega Fusion and Gens.
