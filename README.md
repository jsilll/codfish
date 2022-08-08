Chess-Engine
===

[![Chess-Engine](https://github.com/jsilll/chess-engine/actions/workflows/cmake.yml/badge.svg)](https://github.com/jsilll/chess-engine/actions)

Chess-Engine is a strong chess engine written in C++.

It comes with two operating modes: UCI and CLI:
- The UCI mode is meant for use with a UCI-compatible graphical user interface. 
- The CLI mode is meant for playing and testing the engine within the terminal.

Currently, Chess-Engine doesn't support opening books, endgame tables, multi-threading or any kind of neural network evaluation. Some of these features will certainly be implemented in future versions.

The main goal of this project is to provide a simple introduction to newcomers to the chess programming world. The code is meant to be efficient and fast but also clean, maintainable and, above all, understandable.

Features
===
### Interfaces
- UCI Mode for GUI interaction
- CLI Mode for Terminal Usage

### Move Generation
- Bitboard Representation
- Pre-calculated Attack Tables
- Magic Bitboards
- Pseudo-Legal Move Generation
- Make Unmake Approach

### Move Picker
- PeSTO inspired evaluation function
- NegaMax Search
- Quiescense Search with Null Move Pruning
- Move Sorting with PV, Killer, History and MVV LVA moves
- Triangular PV-Table
- Iterative Deepening
- Principal Variation Search
- Late Move Reduction
- Null Move Pruning
- Transposition Tables

Building and Installation
===

Currently Chess-Engine is meant to be built only for Unix systems (although it may be built for other target platforms with little to no effort).

To compile the source code and the unit tests into their respective binaries:
```
cmake
```

### Build Dependencies
- C++ Standard Library
- Catch2 (header included) for Unit Tests

Usage
===
### Starting in UCI mode
```
./engine_exec --uci
```

### Starting in CLI mode
```
./engine_exec --cli
```

If this argument is not specified at startup the engine will default to UCI mode.

Files
===
Chess-Engine consists of the following files:
- [`CMakeLists.txt`](https://github.com/jsilll/chess-engine/blob/master/CMakeLists.txt), a file that contains the rules for building the engine using [`cmake`](https://cmake.org/).
- [`src`](https://github.com/jsilll/chess-engine/blob/master/src), a subdirectory that contains all the source code for the engine.
- [`tests`](https://github.com/jsilll/chess-engine/blob/master/tests), a subdirectory that contains all the source for the unit tests.
- [`Doxyfile`](https://github.com/jsilll/chess-engine/blob/master/Doxyfile), a file for generating documentation for the source code using [`Doxygen`](https://doxygen.nl/).

Contributing
===
Everyone is more than welcome to contribute to the development of the project. In case you want to help improve this project here's a list of resources to get you started with chess programming:

- [Bitboard Chess Engine in C Video Tutorials Playlist](https://www.youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs)
- [Chess Program in 99 Steps](http://aghaznawi.comuf.com/computer%20chess/winglet/)
- [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)
- [Chess Programming Youtube Channel](https://www.youtube.com/channel/UCB9-prLkPwgvlKKqDgXhsMQ)
- [FEN Editor](http://en.lichess.org/editor)
- [How Magic Bitboards Work](http://www.rivalchess.com/magic-bitboards/)
- [Load PGN Files](http://en.lichess.org/paste)

Terms of Use
===
Chess-Engine is free, and distributed under the **MIT License**.

For full details, read the copy of the MIT License found in the file named
[*LICENSE*](https://github.com/jsilll/chess-engine/blob/master/LICENSE).
