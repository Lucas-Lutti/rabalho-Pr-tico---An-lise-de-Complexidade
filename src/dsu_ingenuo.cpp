#include "dsu_ingenuo.hpp"

namespace fpaa {

DsuNaive::DsuNaive(int n, AccessStats* stats) : stats_(stats) { init(n); }

void DsuNaive::init(int n) {
  n_ = n;
  parent_.resize(static_cast<std::size_t>(n));
  for (int i = 0; i < n_; ++i) {
    parent_[static_cast<std::size_t>(i)] = i;
  }
  if (stats_) {
    stats_->reset();
  }
}

int DsuNaive::parent_at(int i) const {
  if (stats_) {
    ++stats_->reads;
  }
  return parent_[static_cast<std::size_t>(i)];
}

void DsuNaive::parent_set(int i, int v) {
  if (stats_) {
    ++stats_->writes;
  }
  parent_[static_cast<std::size_t>(i)] = v;
}

int DsuNaive::find(int x) {
  while (true) {
    int p = parent_at(x);
    if (p == x) {
      return x;
    }
    x = p;
  }
}

bool DsuNaive::unite(int a, int b) {
  int ra = find(a);
  int rb = find(b);
  if (ra == rb) {
    return false;
  }
  parent_set(ra, rb);
  return true;
}

}  // namespace fpaa
