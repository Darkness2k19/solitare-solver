#include "solver.hpp"
#include "solver/solitare.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <unordered_set>

namespace SolitareSolver {

Solver::Solver(const Solitare &initial_state) {
  Solitare solitare = initial_state;
  for (size_t row_index = 0; row_index < Solitare::kRows; ++row_index) {
    if (solitare.IsCombination(
            row_index, Solitare::kCombination - 1,
            solitare.cards[row_index][Solitare::kCombination - 1])) {
      auto row_begin = solitare.cards[row_index].begin();
      std::fill(row_begin, row_begin + Solitare::kCombination, Card::kEmpty);
    }
  }
  suggestions_.emplace(solitare, 0, 0);
}

size_t Solver::Solve() {
  size_t best_result = static_cast<size_t>(-1);
  std::unordered_set<Solitare> found;
  size_t min_size = -1;
  for (size_t iteration = 1;
       iteration <= kMaximumLookups && !suggestions_.empty(); ++iteration) {
    auto best_suggestion = *suggestions_.begin();
    suggestions_.erase(suggestions_.begin());
    min_size = std::min(min_size, best_suggestion.solitare.GetCardsCount());
    auto [_, is_inserted] = found.emplace(best_suggestion.solitare);
    if (!is_inserted) {
      continue;
    }

    if (best_suggestion.solitare.IsResolved()) {
      best_result = std::min(best_result, best_suggestion.moves);
      break;
    }
    if (best_suggestion.moves > best_result) {
      continue;
    }

    for (const Solitare &next_suggestion :
         best_suggestion.solitare.GetPossibleMovesRange()) {
      suggestions_.emplace(next_suggestion, best_suggestion.moves + 1,
                           iteration);
    }

    while (suggestions_.size() > kMaximumSuggestions) {
      suggestions_.erase(std::prev(suggestions_.end()));
    }
  }
  return best_result;
}

uint32_t Solver::SuggestionPenalty(const Solitare &solitare) {
  if (solitare.IsResolved()) {
    return 0;
  }

  uint32_t penalty = solitare.GetCardsCount() * 15;
  size_t max_height = 0;
  for (size_t row_index = 0; row_index < Solitare::kRows; ++row_index) {
    auto &row = solitare.cards[row_index];
    if (row[0] == Card::kEmpty) {
      continue;
    }

    if (row[0] != Solitare::kCombination - 1) {
      penalty += 100 * (Solitare::kCombination - 1 - row[0].value);
    } else {
      penalty -= 5;
    }
    size_t segment_size = 0;
    for (size_t position = 1; position < Solitare::kMaxCards; ++position) {
      if (row[position] == Card::kEmpty) {
        break;
      }
      size_t prev_segment_size = segment_size;
      if (segment_size != 0) {
        if (row[position] == Solitare::kCombination - segment_size - 1) {
          segment_size = row[position] == 0 ? 0 : segment_size + 1;
          penalty -= 10;
        } else {
          segment_size = 0;
        }
      }
      if (row[position] == Solitare::kCombination - 1) {
        segment_size = 1;
      } else if (segment_size == 0 &&
                 (prev_segment_size != Solitare::kCombination - 1 ||
                  row[position] != 0)) {
        penalty += 14;
      }
    }
    max_height = std::max(max_height, solitare.GetCardsCountInRow(row_index));
  }
  penalty += max_height * 4;
  return penalty;
}

} // namespace SolitareSolver