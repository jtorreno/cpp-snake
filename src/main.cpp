#include <algorithm>
#include <iostream>
#include <locale>
#include <vector>

#include <conio.h>

enum class input { none, up, down, left, right };

input keyboard_input() noexcept {
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

class snake_map {
public:
    snake_map(size_t height, size_t width) : map(height, std::vector<std::string>(width, std::string(" "))){
        std::for_each(map.front().begin(), map.front().end(), [](auto& s) { s = "X"; });
        std::for_each(map.back().begin(), map.back().end(), [](auto& s) { s = "X"; });

        std::for_each(map.begin(), map.end(), [](auto& v){ v.front() = "X"; });
        std::for_each(map.begin(), map.end(), [](auto& v){ v.back() = "X"; });
    }

    void print() const noexcept {
        for (auto const& v : map) {
            for (auto const& i : v) std::cout << i;
            std::cout << std::endl;
        }
    }

    std::string const& at(unsigned int x, unsigned int y) const { return map.at(y).at(x); }
    std::string& at(unsigned int x, unsigned int y) { return map.at(y).at(x); }
private:
    std::vector<std::vector<std::string>> map;
};

int main() {
    snake_map ssnake_map(8, 8);
    ssnake_map.at(3, 4) = "O";

    ssnake_map.print();

    while (true) {
        if (keyboard_input() == input::down) std::cout << "yay" << std::endl;
    }
}
