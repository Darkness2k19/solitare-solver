#pragma once

#include "solitare.hpp"

#include <set>
#include <tuple>

namespace SolitareSolver {

struct Move {};

class Solver {
  static constexpr size_t kMaximumSuggestions = 1e3;
  static constexpr size_t kMaximumLookups = 1e7;

public:
  Solver(const Solitare &initial_state);

  size_t Solve();

private:
  static uint32_t SuggestionPenalty(const Solitare &solitare);

  struct Suggestion {
    uint32_t penalty;
    size_t moves;
    size_t index;
    Solitare solitare;

    Suggestion(const Solitare &solitare, size_t moves, size_t index)
        : penalty(SuggestionPenalty(solitare)), moves(moves), index(index),
          solitare(solitare) {}

    bool operator<(const Suggestion &other) const {
      auto result =
          std::tie(penalty, moves) <=> std::tie(other.penalty, other.moves);
      if (result != 0) {
        return result < 0;
      }
      if (solitare != other.solitare) {
        return index < other.index;
      }
      return false;
    }
  };

private:
  std::set<Suggestion> suggestions_;
};

} // namespace SolitareSolver