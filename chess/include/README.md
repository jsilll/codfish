Example of usage of the codlib library
===

```cpp
#include <iostream>
#include <codchess/codchess.hpp>

using namespace codchess;

int main() {
    auto board = Board();
    board.Display(std::cout);
    const auto moves = movegen::Legal(board);
    std::cout << "Legal moves: " << std::endl;
    for (const auto move : moves) {
        std::cout << move << "\n";
    }
}
```