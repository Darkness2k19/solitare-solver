#include "solver/solitare.hpp"
#include <iostream>

#include <solver/solver.hpp>
#include <utility/reader.hpp>

int main() {
  Utility::Reader reader(std::cin);

  SolitareSolver::Solitare solitare = reader.Read();
  SolitareSolver::Solver solver(solitare);

  size_t result = solver.Solve();

  if (result == static_cast<size_t>(-1)) {
    std::cout << "No solution\n";
  } else if (result == 0) {
    std::cout << "Solitare is already solved\n";
  } else {
    std::cout << "Solution in " << result << " moves exists\n";
  }

  return 0;
}
