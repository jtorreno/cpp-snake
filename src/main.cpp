#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <initializer_list>
#include <iostream>
#include <locale>
#include <random>
#include <thread>
#include <vector>

static_assert(_WIN32);
#include <conio.h>

std::mt19937 prng(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1));

enum class input { none, up, down, left, right };
class vec2 {
public:
    bool operator==(vec2 const& rv) const noexcept { return ((rv.x == x) && (rv.y == y)); }

    vec2 direction(input input) const noexcept {
        switch (input) {
            case input::up: return {x, y - 1};
            case input::down: return {x, y + 1};
            case input::left: return {x - 1, y};
            case input::right: return {x + 1, y};

            default: { return {x, y}; }
        }
    }

    int x, y;
};

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
                std::cout << "\n";
            }
        }

        auto const& at(vec2 pos) const { return _map.at(pos.y).at(pos.x); }
        auto& at(vec2 pos) { return _map.at(pos.y).at(pos.x); }

        auto& internal_map() { return _map; }

    private:
        std::vector<std::vector<std::string>> _map;
    };

    void new_food(map& muh_map, map const& muh_mmap) {
        for (auto& v : muh_map.internal_map()) {
            for (auto& i : v) {
                if (i == "*") i = " ";
            }
        }

        vec2 food_position{0, 0};
        while (true) {
            std::uniform_int_distribution<int> uid(1, 23);

            food_position.x = uid(prng);
            food_position.y = uid(prng);

            if (muh_mmap.at(food_position) == " ") break;
        }

        muh_map.at(food_position) = "*";
    }

    class snake {
    public:
        snake(vec2 const& spawn_position, input initial_direction) : body { spawn_position }, length(1), last_direction(initial_direction) {}

        bool move(input const& direction, map& snake_map) {
            input new_direction = direction;

            if (new_direction == input::none) { new_direction = last_direction; } else { last_direction = new_direction; }
            body.push_front(body.front().direction(new_direction));

            auto ssnake_map = snake_map;

            if (ssnake_map.at(body.front()) == "X") return false;
            if (std::count(body.begin(), body.end(), body.front()) > 1) return false;

            if (ssnake_map.at(body.front()) == "*") {
                new_food(snake_map, ssnake_map);
                ++length;
            }

            if (body.size() == length + 1) body.erase(body.begin() + length);
            for (auto i : body) ssnake_map.at(i) = "O";

            ssnake_map.print();
            std::cout << "\nscore: " << body.size() << " x: " << body.front().x << " y: " << body.front().y << "\n";

            return true;
        }

    private:
        std::deque<vec2> body;
        unsigned int length;

        input last_direction;
    };
}

auto main() -> int {
    snek::map snake_map(24, 24);
    snake_map.print();

    input initial_direction;

    while(true) {
        initial_direction = keyboard_input();
        if (initial_direction != input::none) {
            snek::snake snake({11, 11}, initial_direction);
            new_food(snake_map, snake_map);

            while (snake.move(keyboard_input(), snake_map)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(125));
                std::system("cls");
            }

            break;
        }
    }

    return 0;
}
