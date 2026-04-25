#include "dsu_uniao_por_rank.hpp"

namespace fpaa {

DsuUnionRank::DsuUnionRank(int n, AccessStats* stats) : stats_(stats) { init(n); }

void DsuUnionRank::init(int n) {
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

int DsuUnionRank::parent_at(int i) const {
  if (stats_) {
    ++stats_->reads;
  }
  return parent_[static_cast<std::size_t>(i)];
}

void DsuUnionRank::parent_set(int i, int v) {
  if (stats_) {
    ++stats_->writes;
  }
  parent_[static_cast<std::size_t>(i)] = v;
}

int DsuUnionRank::rank_at(int i) const {
  if (stats_) {
    ++stats_->reads;
  }
  return rank_[static_cast<std::size_t>(i)];
}

void DsuUnionRank::rank_set(int i, int v) {
  if (stats_) {
    ++stats_->writes;
  }
  rank_[static_cast<std::size_t>(i)] = v;
}

int DsuUnionRank::find(int x) {
  while (true) {
    int p = parent_at(x);
    if (p == x) {
      return x;
    }
    x = p;
  }
}

bool DsuUnionRank::unite(int a, int b) {
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
