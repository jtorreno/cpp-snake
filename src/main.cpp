#include <algorithm>
#include <iostream>
#include <locale>
#include <vector>
#include <conio.h>

enum class input { none, up, down, left, right };

auto keyboard_input() noexcept {
    if (_kbhit()) {
        auto character = std::tolower(_getch());

        switch (character) {
        case 'w': return input::up;
        case 'a': return input::left;
        case 's': return input::down;
        case 'd': return input::right;

        default: return input::none;
        }
    }

    return input::none;
}

namespace snek {

    struct map {
        map(size_t const height, size_t const width) : _map(height, std::vector<std::string>(width, std::string(" "))) {
            std::for_each(_map.front().begin(), _map.front().end(), [](auto& s) { s = "X"; });
            std::for_each(_map.back().begin(), _map.back().end(), [](auto& s) { s = "X"; });

            std::for_each(_map.begin(), _map.end(), [](auto& v) { v.front() = "X"; });
            std::for_each(_map.begin(), _map.end(), [](auto& v) { v.back() = "X"; });
        }

        auto print() const noexcept {
            for (auto const& v : _map) {
                for (auto const& i : v) std::cout << i;
                std::cout << std::endl;
            }
        }

        std::string const& at(unsigned int x, unsigned int y) const { return _map.at(y).at(x); }
        std::string& at(unsigned int x, unsigned int y) { return _map.at(y).at(x); }

    private:
        std::vector<std::vector<std::string>> _map;
    };

}

auto main() -> int {
    
    snek::map snake_map(8, 8);
    snake_map.at(3, 4) = "O";
    snake_map.print();

    auto some_exit_condition = false;
    while (!some_exit_condition) {
        if (keyboard_input() == input::down) std::cout << "yay" << std::endl;
    }

    return 0;

}
