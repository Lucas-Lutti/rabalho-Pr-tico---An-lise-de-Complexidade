#include "operacoes.hpp"

#include <random>

namespace fpaa {

std::vector<Operation> generate_operations(int n, int m, double union_fraction,
                                           std::uint32_t seed) {
  std::mt19937 rng(seed);
  std::uniform_real_distribution<double> uni01(0.0, 1.0);
  std::uniform_int_distribution<int> dist(0, n - 1);

  std::vector<Operation> ops;
  ops.reserve(static_cast<std::size_t>(m));
  for (int i = 0; i < m; ++i) {
    Operation op;
    if (uni01(rng) < union_fraction) {
      op.kind = OpKind::kUnion;
      op.a = dist(rng);
      op.b = dist(rng);
    } else {
      op.kind = OpKind::kFind;
      op.a = dist(rng);
      op.b = 0;
    }
    ops.push_back(op);
  }
  return ops;
}

}  // namespace fpaa
