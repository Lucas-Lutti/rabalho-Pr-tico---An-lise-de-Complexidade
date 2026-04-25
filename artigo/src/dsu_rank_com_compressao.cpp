#include "dsu_rank_com_compressao.hpp"

namespace fpaa {

DsuTarjan::DsuTarjan(int n, AccessStats* stats) : stats_(stats) { init(n); }

void DsuTarjan::init(int n) {
  n_ = n;
  parent_.resize(static_cast<std::size_t>(n));
  rank_.resize(static_cast<std::size_t>(n));
  for (int i = 0; i < n_; ++i) {
    parent_[static_cast<std::size_t>(i)] = i;
    rank_[static_cast<std::size_t>(i)] = 0;
  }
  if (stats_) {
    stats_->reset();
  }
}

int DsuTarjan::parent_at(int i) const {
  if (stats_) {
    ++stats_->reads;
  }
  return parent_[static_cast<std::size_t>(i)];
}

void DsuTarjan::parent_set(int i, int v) {
  if (stats_) {
    ++stats_->writes;
  }
  parent_[static_cast<std::size_t>(i)] = v;
}

int DsuTarjan::rank_at(int i) const {
  if (stats_) {
    ++stats_->reads;
  }
  return rank_[static_cast<std::size_t>(i)];
}

void DsuTarjan::rank_set(int i, int v) {
  if (stats_) {
    ++stats_->writes;
  }
  rank_[static_cast<std::size_t>(i)] = v;
}

int DsuTarjan::find(int x) {
  int root = x;
  for (;;) {
    int p = parent_at(root);
    if (p == root) {
      break;
    }
    root = p;
  }
  while (x != root) {
    int p = parent_at(x);
    parent_set(x, root);
    x = p;
  }
  return root;
}

bool DsuTarjan::unite(int a, int b) {
  int ra = find(a);
  int rb = find(b);
  if (ra == rb) {
    return false;
  }
  int rka = rank_at(ra);
  int rkb = rank_at(rb);
  if (rka < rkb) {
    parent_set(ra, rb);
  } else if (rka > rkb) {
    parent_set(rb, ra);
  } else {
    parent_set(rb, ra);
    rank_set(ra, rka + 1);
  }
  return true;
}

}  // namespace fpaa
