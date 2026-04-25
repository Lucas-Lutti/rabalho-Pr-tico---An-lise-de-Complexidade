/** DSU ingênuo — TP1 FPAA (PUC Minas). */
#pragma once

#include <vector>

#include "estatisticas_acesso.hpp"

namespace fpaa {

/** DSU ingênuo: lista/árvore sem rank e sem compressão de caminho. */
class DsuNaive {
 public:
  explicit DsuNaive(int n, AccessStats* stats = nullptr);

  void init(int n);
  int find(int x);
  bool unite(int a, int b);
  bool connected(int a, int b) { return find(a) == find(b); }

  AccessStats* stats() { return stats_; }

 private:
  int parent_at(int i) const;
  void parent_set(int i, int v);

  int n_ = 0;
  std::vector<int> parent_;
  AccessStats* stats_ = nullptr;
};

}  // namespace fpaa
