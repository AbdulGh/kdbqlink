#include <string_view>
#include <algorithm>
#include <cstdio>
#include <array>
#include <span>
#include <cassert>
#include <optional>
#include <functional>
#include <iostream>

template <typename T, std::size_t N>
class Trie
{
  private:
  struct Node {
    bool has_value {false};
    std::size_t index {0};
    std::array<std::size_t, 127> jumps {};

    void print() {
      std::cout << "has_value: " << has_value << "\n";
      std::cout << "index: " << index << "\n";
      for (char c = 'a'; c <= 'z'; ++c) {
        std::cout << c << " ";
      }
      std::cout << "\n";
      for (char c = 'a'; c <= 'z'; ++c) {
        std::cout << jumps[c] << " ";
      }
      std::cout << std::endl;
    }
  };

  std::vector<Node> jump_table {};
  std::array<T, N> values;

  public:
  using key_t = std::string_view;
  using value_t = T;
  using kv_t = std::pair<key_t, value_t>;

  constexpr Trie(std::array<kv_t, N> incoming) {
    std::sort(incoming.begin(), incoming.end(), [](auto& l, auto& r) { return l.first < r.first; });
    for (std::size_t i = 0; i < values.size(); ++i) {
      values[i] = std::move(incoming[i].second); //values_view?
    }

    auto build = [&incoming, this](std::size_t l, std::size_t r, size_t depth, auto& me) -> std::size_t {
      std::size_t my_write_head = std::size(jump_table);
      jump_table.push_back(Node{});

      Node& my_node = jump_table[my_write_head];
      my_node.index = l;
      if (depth >= std::size(incoming[l].first)) {
        my_node.has_value = true;
        ++l;
      }

      while (l < r) {
        char current_char = incoming[l].first[depth];
        std::size_t with_char = l;
        while (with_char < r && incoming[with_char].first[depth] == current_char) {
          ++with_char;
        }
        //Remember that my_node is invalid as soon as we call the first me(...)
        jump_table[my_write_head].jumps[current_char] = me(l, with_char, depth + 1, me);
        l = with_char;
      }

      return my_write_head;
    };

    build(0, std::size(incoming), 0, build);
  }

  constexpr std::optional<std::reference_wrapper<T>> operator[](std::string_view key) {
    std::size_t current_jt_pos = 0;
    std::size_t current_index = 0;

    while (current_index < std::size(key)) {
      current_jt_pos = jump_table[current_jt_pos].jumps[key[current_index]];
      if (current_jt_pos == 0) {
        return std::nullopt;
      }
      ++current_index;
    }

    if (jump_table[current_jt_pos].has_value) {
      return std::make_optional(std::ref(values[jump_table[current_jt_pos].index]));
    }
    return std::nullopt;
  }
};
