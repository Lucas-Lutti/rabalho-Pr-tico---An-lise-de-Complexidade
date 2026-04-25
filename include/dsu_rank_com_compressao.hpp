/** DSU união por rank + compressão de caminho — TP1 FPAA (PUC Minas). */
#pragma once

#include <vector>

#include "estatisticas_acesso.hpp"

namespace fpaa {

/** Union by rank + path compression (análise amortizada O(α(n)) por operação). */
class DsuTarjan {
 public:
  explicit DsuTarjan(int n, AccessStats* stats = nullptr);

  void init(int n);
  int find(int x);
  bool unite(int a, int b);
  bool connected(int a, int b) { return find(a) == find(b); }

  AccessStats* stats() { return stats_; }

 private:
  int parent_at(int i) const;
  void parent_set(int i, int v);
  int rank_at(int i) const;
  void rank_set(int i, int v);

  int n_ = 0;
  std::vector<int> parent_;
  std::vector<int> rank_;
  AccessStats* stats_ = nullptr;
};

}  // namespace fpaa
