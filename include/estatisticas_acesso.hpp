#pragma once

#include <cstdint>

namespace fpaa {

/** Contagem de acessos de leitura/escrita a parent/rank durante Find/Union. */
struct AccessStats {
  std::uint64_t reads = 0;
  std::uint64_t writes = 0;

  void reset() {
    reads = 0;
    writes = 0;
  }

  std::uint64_t total() const { return reads + writes; }
};

}  // namespace fpaa
