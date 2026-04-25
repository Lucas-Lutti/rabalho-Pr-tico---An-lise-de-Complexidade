#pragma once

#include <vector>

namespace fpaa {

enum class OpKind { kUnion, kFind };

struct Operation {
  OpKind kind;
  int a = 0;
  int b = 0;  // apenas para kUnion
};

/** Gera sequência reproduzível: fração `union_fraction` de unions, restante finds. */
std::vector<Operation> generate_operations(int n, int m, double union_fraction,
                                           std::uint32_t seed);

}  // namespace fpaa
