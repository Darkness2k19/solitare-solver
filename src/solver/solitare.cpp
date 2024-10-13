#include "solitare.hpp"

#include <iostream>
#include <stdexcept>
#include <tuple>

namespace SolitareSolver {

bool Solitare::operator==(const Solitare &other) const {
  std::array<size_t, kRows> equal_to;
  std::array<size_t, kRows> other_equal_to;

  std::array<size_t, kRows> cnts;
  std::array<size_t, kRows> other_cnts;
  for (size_t row_index = 0; row_index < kRows; ++row_index) {
    cnts[row_index] = 0;
    other_cnts[row_index] = 0;
  }
  for (size_t row_index = 0; row_index < kRows; ++row_index) {
    equal_to[row_index] = row_index;
    for (size_t other_index = 0; other_index < row_index; ++other_index) {
      if (cards[row_index] == cards[other_index]) {
        equal_to[row_index] = other_index;
        break;
      }
    }
    ++cnts[equal_to[row_index]];
    other_equal_to[row_index] = kRows;
    for (size_t other_index = 0; other_index < row_index; ++other_index) {
      if (cards[row_index] == other.cards[other_index]) {
        other_equal_to[row_index] = other_index;
        break;
      }
    }
    if (other_equal_to[row_index] == kRows) {
      return false;
    }
    ++other_cnts[other_equal_to[row_index]];
  }
  for (size_t row_index = 0; row_index < kRows; ++row_index) {
    if (cnts[row_index] != other_cnts[row_index]) {
      return false;
    }
  }
  return true;
}

void Solitare::Print() const {
  for (const auto &row : cards) {
    for (const auto &card : row) {
      std::cout << '0' + card.value << ' ';
    }
    std::cout << '\n';
  }
}

bool Solitare::IsResolved() const {
  for (const auto &row : cards) {
    if (row[0] != Card::kEmpty) {
      return false;
    }
  }
  return true;
}

bool Solitare::IsCombination(size_t row, size_t position, Card added) const {
  if (row >= kRows) {
    throw std::runtime_error("row is out of range");
  }
  if (position > kMaxCards) {
    throw std::runtime_error("card position is out of range");
  }
  if (position < kCombination - 1) {
    return false;
  }
  if (added != 0) {
    return false;
  }
  for (size_t index = 1; index < kCombination; ++index) {
    if (cards[row][position - index] != index) {
      return false;
    }
  }
  return true;
}

size_t Solitare::GetCardsCount() const {
  size_t amount = 0;
  for (size_t row_index = 0; row_index < kRows; ++row_index) {
    amount += GetCardsCountInRow(row_index);
  }
  return amount;
}

size_t Solitare::GetCardsCountInRow(size_t row) const {
  for (size_t position = 0; position < kMaxCards; ++position) {
    if (cards[row][position] == Card::kEmpty) {
      return position;
    }
  }
  return kMaxCards;
}

void Solitare::MoveCard(size_t from_row, size_t to_row) {
  if (from_row == to_row) {
    throw std::runtime_error("cannot move card from row to itself");
  }
  size_t from_count = GetCardsCountInRow(from_row);
  if (from_count == 0) {
    throw std::runtime_error("from row is empty");
  }
  size_t to_count = GetCardsCountInRow(to_row);
  if (IsCombination(to_row, to_count, cards[from_row][from_count - 1])) {
    for (size_t index = 1; index < kCombination; ++index) {
      cards[to_row][to_count - index] = Card::kEmpty;
    }
  } else {
    if (to_count == kMaxCards) {
      throw std::runtime_error("to row is full");
    }
    cards[to_row][to_count] = cards[from_row][from_count - 1];
  }
  cards[from_row][from_count - 1] = Card::kEmpty;
}

bool Solitare::SolitareMovesIterator::FindNext() {
  if (!solitare_ptr) {
    return false;
  }
  std::array<size_t, kRows> equal_to;
  for (size_t row_index = 0; row_index < kRows; ++row_index) {
    equal_to[row_index] = row_index;
    for (size_t other_index = 0; other_index < row_index; ++other_index) {
      if (solitare_ptr->cards[row_index] == solitare_ptr->cards[other_index]) {
        equal_to[row_index] = other_index;
        break;
      }
    }
  }

  ++end_row;
  for (; begin_row < kRows; ++begin_row) {
    size_t begin_count = solitare_ptr->GetCardsCountInRow(begin_row);
    if (begin_count == 0 || equal_to[begin_row] != begin_row) {
      continue;
    }
    Card begin_card = solitare_ptr->cards[begin_row][begin_count - 1];
    for (; end_row < kRows; ++end_row) {
      if (begin_row == end_row || equal_to[end_row] != end_row) {
        continue;
      }

      size_t end_count = solitare_ptr->GetCardsCountInRow(end_row);
      if (end_count == kMaxCards) {
        if (solitare_ptr->IsCombination(end_row, end_count, begin_card)) {
          return true;
        }
        continue;
      }
      if (end_count == 0) {
        return true;
      }
      if (begin_card < solitare_ptr->cards[end_row][end_count - 1]) {
        return true;
      }
    }
    end_row = 0;
  }
  return false;
}

bool Solitare::SolitareMovesIterator::operator==(
    const SolitareMovesIterator &other) const {
  if (solitare_ptr == nullptr) {
    return other.solitare_ptr == nullptr;
  }
  return std::tie(solitare_ptr, begin_row, end_row) ==
         std::tie(other.solitare_ptr, other.begin_row, other.end_row);
}

Solitare::SolitareMovesIterator &Solitare::SolitareMovesIterator::operator++() {
  if (!FindNext()) {
    solitare_ptr = nullptr;
  }
  return *this;
}

Solitare Solitare::SolitareMovesIterator::operator*() const {
  if (solitare_ptr == nullptr) {
    throw std::runtime_error("iterator has no solitare");
  }
  Solitare result(solitare_ptr->cards);
  result.MoveCard(begin_row, end_row);
  return result;
}

auto Solitare::SolitareMovesGenerator::begin() const
    -> Solitare::SolitareMovesIterator {
  return begin_;
}
auto Solitare::SolitareMovesGenerator::end() const
    -> Solitare::SolitareMovesIterator {
  return end_;
}

Solitare::SolitareMovesGenerator Solitare::GetPossibleMovesRange() const {
  return {SolitareMovesIterator{this}, SolitareMovesIterator{nullptr}};
}

} // namespace SolitareSolver