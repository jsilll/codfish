<p align="center">
  
</p>

<h1 align="center">Codfish</h1>

<p align="center">
  <a href="https://github.com/jsilll/codfish/actions"><img src="https://github.com/jsilll/codfish/actions/workflows/cmake.yml/badge.svg"></a>
</p>

Codfish is a strong chess engine written in C++.

Currently, Codfish doesn't support opening books, endgame tables, multi-threading or any kind of neural network evaluation. Some of these features will certainly be implemented in future releases.

The main goal of this project is to provide a simple introduction to newcomers to the chess programming world. The code is meant to be efficient and fast but also clean, maintainable and, above all, understandable.

Features
===
### Interfaces
- [UCI](http://wbec-ridderkerk.nl/html/UCIProtocol.html) Mode for GUI interaction
- CLI Mode for Terminal Usage

### Move Generation
- [Bitboard Representation](https://www.chessprogramming.org/Bitboards)
- [Pre-calculated Attack Tables](https://www.chessprogramming.org/Attack_and_Defend_Maps)
- [Magic Bitboards](https://www.chessprogramming.org/Looking_for_Magics)
- [Pseudo-Legal Move Generation](https://www.chessprogramming.org/Pseudo-Legal_Move)
- [Make Unmake Approach](https://www.chessprogramming.org/Make_Move)

### Move Picker
- [PeSTO inspired evaluation function](https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function)
- [NegaMax Search](https://www.chessprogramming.org/Negamax)
- [Quiescense Search](https://www.chessprogramming.org/Quiescence_Search)
- [Move Ordering with PV, Killer, History and MVV LVA moves](https://www.chessprogramming.org/index.php?title=Move_Ordering&mobileaction=toggle_view_mobile)
- [Triangular PV-Table](https://www.chessprogramming.org/index.php?title=Triangular_PV-Table&mobileaction=toggle_view_mobile)
- [Iterative Deepening](https://www.chessprogramming.org/Iterative_Deepening)
- [Principal Variation Search](https://www.chessprogramming.org/Principal_Variation_Search)
- [Late Move Reduction](https://www.chessprogramming.org/Late_Move_Reductions)
- [Null Move Pruning](https://www.chessprogramming.org/Null_Move_Pruning)
- [Null Window Search](https://www.chessprogramming.org/Null_Window)
- [Transposition Table](https://en.wikipedia.org/wiki/Transposition_table)
- [Repetition Detection](https://www.chessprogramming.org/Repetitions)

Building and Installation
===

Codfish is meant to be built only for Unix systems (although it may be built for other target platforms with little to no effort).

To compile the source code and the unit tests into their respective binaries:
```
mkdir build
cd build
cmake ../
cd ../
cmake --build build --config Release --target all
```

### Build Dependencies
- C++ Standard Library
- [Catch2](https://github.com/catchorg/Catch2) (header included) for Unit Tests

Usage
===
### Starting in UCI mode
```
codfish --uci
```

### Starting in CLI mode
```
codfish --cli
```

If this argument is not specified at startup the engine will default to UCI mode.
Once in UCI mode mode you can use any UCI compliant chess GUI to interact with the engine,
some examples are [CuteChess](https://cutechess.com/) and [Arena](https://cutechess.com/).

Files
===
Codfish consists of the following files:
- [`CMakeLists.txt`](https://github.com/jsilll/codfish/blob/master/CMakeLists.txt), a file that contains the rules for building the engine using [cmake](https://cmake.org/).
- [`src`](https://github.com/jsilll/codfish/blob/master/src), a subdirectory that contains all the source code for the engine.
- [`tests`](https://github.com/jsilll/codfish/blob/master/tests), a subdirectory that contains all the source for the unit tests.
- [`Doxyfile`](https://github.com/jsilll/codfish/blob/master/Doxyfile), a file for generating documentation for the source code using [Doxygen](https://doxygen.nl/).

Contributing
===
Everyone is more than welcome to contribute to the development of the project. In case you want to help improve this project here's a list of resources to get you started with chess programming:

- [Bitboard Chess Engine in C Video Tutorials Playlist](https://youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs)
- [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)
- [Chess Programming Youtube Channel](https://www.youtube.com/channel/UCB9-prLkPwgvlKKqDgXhsMQ)
- [FEN Editor](http://en.lichess.org/editor)
- [Load PGN Files](http://en.lichess.org/paste)

Terms of Use
===
Codfish is free, and distributed under the **MIT License**.

For full details, read the copy of the MIT License found in the file named
[LICENSE](https://github.com/jsilll/codfish/blob/master/LICENSE).
