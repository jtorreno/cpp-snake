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

#include <conio.h>

std::random_device rd;
std::mt19937 prng(rd());

enum class input { none, up, down, left, right };
class vec2 {
public:
    vec2(std::initializer_list<int> list) : v(list) {
        x = v.at(0);
        y = v.at(1);
    }

    vec2 up() const noexcept { return {x, y - 1}; }
    vec2 down() const noexcept { return {x, y + 1}; }

    vec2 left() const noexcept { return {x - 1, y}; }
    vec2 right() const noexcept { return {x + 1, y}; }

    vec2 direction(input input) const noexcept {
        switch (input) {
            case input::up: { return {x, y - 1}; break;}
            case input::down: { return {x, y + 1}; break;}
            case input::left: { return {x - 1, y}; break;}
            case input::right: { return {x + 1, y}; break;}

            default: { return {x, y}; }
        }
    }

    int x, y;
private:
    std::vector<int> v;
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

    class snake {
    public:
        snake(vec2 const& spawn_position, input initial_direction) : _body { spawn_position }, length(1), _last_direction(initial_direction) {}

        void grow() { ++length; }

        void move(input const& direction) {
            input ddirection = direction;
            if (ddirection == input::none) { ddirection = _last_direction; } else { _last_direction = ddirection; }

            switch (ddirection) {
                case input::up: { _body.push_front(_body.front().up()); break; }
                case input::down: { _body.push_front(_body.front().down()); break; }
                case input::left: { _body.push_front(_body.front().left()); break; }
                case input::right: { _body.push_front(_body.front().right()); break; }

                case input::none: {}
            }

            if (_body.size() == length + 1) _body.erase(_body.begin() + length);
            std::cout << "score: " << _body.size() << " x: " << _body.front().x << " y: " << _body.front().y << "\n";
        }

        auto const& body() { return _body; }
        auto const& last_direction() { return _last_direction; }
    private:
        std::deque<vec2> _body;
        unsigned int length;

        input _last_direction;
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

            while (true) {
                auto ssnake_map = snake_map;

                if (ssnake_map.at(snake.body().front()) == "X") break;

                for (auto i : snake.body()) {
                    ssnake_map.at(i) = "O";
                }

                if (ssnake_map.at(snake.body().front().direction(snake.last_direction())) == "*") {
                    new_food(snake_map, ssnake_map);
                    snake.grow();
                }

                if (ssnake_map.at(snake.body().front().direction(snake.last_direction())) == "O") break;

                std::system("cls");
                ssnake_map.print();

                snake.move(keyboard_input());
                std::this_thread::sleep_for(std::chrono::milliseconds(125));
            }

            break;
        }
    }

    return 0;
}
