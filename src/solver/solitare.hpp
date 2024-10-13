#pragma once

#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <unordered_set>

namespace SolitareSolver {

struct Card {
  static constexpr uint8_t kEmpty = static_cast<uint8_t>(-1);

  Card() : Card(kEmpty) {}
  Card(uint8_t value) : value(value) {}

  friend bool operator==(const Card &card, const Card &value) = default;
  friend std::strong_ordering operator<=>(const Card &card,
                                          const Card &value) = default;

  uint8_t value;
};

struct Solitare {
  static constexpr size_t kMaxCards = 16;
  static constexpr size_t kRows = 8;
  static constexpr size_t kCombination = 9;

  std::array<std::array<Card, kMaxCards>, kRows> cards;

  bool operator==(const Solitare &other) const;

  bool IsResolved() const;
  bool IsCombination(size_t row, size_t position, Card added) const;

  size_t GetCardsCount() const;
  size_t GetCardsCountInRow(size_t row) const;
  void MoveCard(size_t from_row, size_t to_row);

  void Print() const;

  struct SolitareMovesIterator {
    SolitareMovesIterator(const Solitare *solitare_ptr)
        : solitare_ptr(solitare_ptr), begin_row(0), end_row(-1) {
      FindNext();
    }

    bool operator==(const SolitareMovesIterator &other) const;

    SolitareMovesIterator &operator++();

    Solitare operator*() const;

  private:
    bool FindNext();

  private:
    const Solitare *solitare_ptr;
    size_t begin_row;
    size_t end_row;
  };

  struct SolitareMovesGenerator {
    SolitareMovesIterator begin_;
    SolitareMovesIterator end_;

    SolitareMovesIterator begin() const;
    SolitareMovesIterator end() const;
  };

  SolitareMovesGenerator GetPossibleMovesRange() const;
};

} // namespace SolitareSolver

template <> struct std::hash<SolitareSolver::Solitare> {
  uint32_t operator()(const SolitareSolver::Solitare &value) const {
    uint32_t result = 0;
    for (size_t row_index = 0; row_index < SolitareSolver::Solitare::kRows;
         ++row_index) {
      for (size_t position = 0; position < SolitareSolver::Solitare::kMaxCards;
           ++position) {
        result += ((row_index + 1) * SolitareSolver::Solitare::kMaxCards +
                   position + 1) *
                  value.cards[row_index][position].value;
      }
    }
    return result;
  }
};