#pragma once

#include <istream>

#include <solver/solitare.hpp>

namespace Utility {

class Reader {
public:
  Reader() = default;
  Reader(std::istream &in);

  SolitareSolver::Solitare Read();

private:
  std::istream *in_ = nullptr;
};

} // namespace Utility