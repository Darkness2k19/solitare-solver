#include "reader.hpp"
#include <istream>
#include <stdexcept>

#include <solver/solitare.hpp>

// #include "utility/graph.hpp"

namespace Utility {

Reader::Reader(std::istream &in) : in_(&in) {}

SolitareSolver::Solitare Reader::Read() {
  if (in_ == nullptr) {
    throw std::runtime_error("no stream to read from");
  }

  std::istream &in = *in_;

  SolitareSolver::Solitare result;
  for (auto &row : result.cards) {
    for (size_t position = 0; position < SolitareSolver::Solitare::kCombination;
         ++position) {
      std::string sym;
      in >> sym;
      if (sym.size() == 1 && '6' <= sym[0] && sym[0] <= '9') {
        row[position].value = sym[0] - '6';
      } else if (sym == "10") {
        row[position].value = 4;
      } else if (sym == "J") {
        row[position].value = 5;
      } else if (sym == "Q") {
        row[position].value = 6;
      } else if (sym == "K") {
        row[position].value = 7;
      } else if (sym == "A") {
        row[position].value = 8;
      } else {
        throw std::runtime_error("incorrect symbol");
      }
    }
  }
  return result;
}

} // namespace Utility