#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <locale>
#include <random>
#include <thread>
#include <vector>

#include <conio.h>

enum class input { none, up, down, left, right };
struct vec2 { int x, y; };

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

        auto const& at(unsigned int x, unsigned int y) const { return _map.at(y).at(x); }
        auto& at(unsigned int x, unsigned int y) { return _map.at(y).at(x); }

        auto& internal_map() { return _map; }

    private:
        std::vector<std::vector<std::string>> _map;
    };

    class snake {
    public:
        snake(vec2 const& spawn_position, input initial_direction) : _body { spawn_position }, length(1), last_direction(initial_direction) {}

        void grow() { ++length; }

        void move(input const& direction) {
            input ddirection = direction;
            if (ddirection == input::none) { ddirection = last_direction; } else { last_direction = ddirection; }

            switch (ddirection) {
                case input::up: { _body.push_front({_body.front().x, _body.front().y - 1}); break; }
                case input::down: { _body.push_front({_body.front().x, _body.front().y + 1}); break; }
                case input::left: { _body.push_front({_body.front().x - 1, _body.front().y}); break; }
                case input::right: { _body.push_front({_body.front().x + 1, _body.front().y}); break; }

                case input::none: {}
            }

            if (_body.size() == length + 1) _body.erase(_body.begin() + length);
            std::cout << "score: " << _body.size() << " x: " << _body.front().x << " y: " << _body.front().y << "\n";
        }

        auto const& body() { return _body; }
    private:
        std::deque<vec2> _body;
        unsigned int length;

        input last_direction;
    };

    void new_food(map& muh_map, map const& muh_mmap) {
        for (auto& v : muh_map.internal_map()) {
            for (auto& i : v) {
                if (i == "*") i = " ";
                break;
            }
        }

        vec2 food_position;
        while (true) {
            std::random_device rd;
            std::mt19937 prng(rd());
            std::uniform_int_distribution<int> uid(1, 23);

            food_position.x = uid(prng);
            food_position.y = uid(prng);

            if (muh_mmap.at(food_position.x, food_position.y) == " ") break;
        }

        muh_map.at(food_position.x, food_position.y) = "*";
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
            bool game_finished = false;

            new_food(snake_map, snake_map);

            while (!game_finished) {
                auto ssnake_map = snake_map;
                for (auto i : snake.body()) {
                    if (ssnake_map.at(i.x, i.y) == "O" || ssnake_map.at(i.x, i.y) == "X") {
                        game_finished = true;
                        break;
                    }

                    if (ssnake_map.at(i.x, i.y) == "*") {
                        new_food(snake_map, ssnake_map);
                        snake.grow();
                    }

                    ssnake_map.at(i.x, i.y) = "O";
                }

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
