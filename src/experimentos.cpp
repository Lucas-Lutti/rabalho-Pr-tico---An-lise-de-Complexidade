#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "dsu_ingenuo.hpp"
#include "dsu_rank_com_compressao.hpp"
#include "dsu_uniao_por_rank.hpp"
#include "operacoes.hpp"

namespace {

using clock = std::chrono::steady_clock;

template <typename Dsu>
void apply_operations(Dsu& dsu, const std::vector<fpaa::Operation>& ops) {
  for (const auto& op : ops) {
    if (op.kind == fpaa::OpKind::kUnion) {
      dsu.unite(op.a, op.b);
    } else {
      dsu.find(op.a);
    }
  }
}

struct Row {
  std::string variant;
  int n = 0;
  int m = 0;
  std::uint32_t seed = 0;
  std::uint64_t time_ns = 0;
  std::uint64_t reads = 0;
  std::uint64_t writes = 0;
};

template <typename Dsu>
Row run_variant(const std::string& name, int n,
                const std::vector<fpaa::Operation>& ops,
                std::uint32_t seed) {
  fpaa::AccessStats stats;
  Dsu dsu(n, &stats);
  (void)seed;

  auto t0 = clock::now();
  apply_operations(dsu, ops);
  auto t1 = clock::now();

  Row r;
  r.variant = name;
  r.n = n;
  r.m = static_cast<int>(ops.size());
  r.seed = seed;
  r.time_ns = static_cast<std::uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
  r.reads = stats.reads;
  r.writes = stats.writes;
  return r;
}

void warmup_same_ops(int n, const std::vector<fpaa::Operation>& ops) {
  fpaa::AccessStats st;
  fpaa::DsuTarjan dsu(n, &st);
  apply_operations(dsu, ops);
}

void write_header(std::ostream& out) {
  out << "variant,n,m,seed,time_ns,reads,writes,total_accesses\n";
}

void write_row(std::ostream& out, const Row& r) {
  out << r.variant << ',' << r.n << ',' << r.m << ',' << r.seed << ','
      << r.time_ns << ',' << r.reads << ',' << r.writes << ','
      << (r.reads + r.writes) << '\n';
}

void run_sweep(std::ostream& out, const std::vector<int>& ns, int m_multiplier,
               double union_fraction, std::uint32_t base_seed) {
  write_header(out);
  for (std::size_t i = 0; i < ns.size(); ++i) {
    int n = ns[i];
    int m = m_multiplier * n;
    std::uint32_t seed = base_seed + static_cast<std::uint32_t>(i) * 7919u;
    auto ops =
        fpaa::generate_operations(n, m, union_fraction, seed);
    warmup_same_ops(n, ops);

    Row a = run_variant<fpaa::DsuNaive>("naive", n, ops, seed);
    Row b = run_variant<fpaa::DsuUnionRank>("union_rank", n, ops, seed);
    Row c = run_variant<fpaa::DsuTarjan>("tarjan", n, ops, seed);
    write_row(out, a);
    write_row(out, b);
    write_row(out, c);
  }
}

void print_usage(const char* argv0) {
  std::cerr << "Uso:\n  " << argv0 << " --all\n  " << argv0
            << " --single <n> <m> <seed> <union_fraction>\n";
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }
  std::string mode = argv[1];
  if (mode == "--all") {
    std::vector<int> ns = {500,  1000,  2000,   4000,
                           8000, 16000, 32000, 64000};
    const int m_mult = 20;
    const double uf = 0.25;
    const std::uint32_t base = 42u;
    std::ofstream out("results/benchmark.csv");
    if (!out) {
      std::cerr << "Não foi possível criar results/benchmark.csv\n";
      return 1;
    }
    run_sweep(out, ns, m_mult, uf, base);
    std::cout << "Gravado results/benchmark.csv\n";
    return 0;
  }
  if (mode == "--single" && argc >= 6) {
    int n = std::stoi(argv[2]);
    int m = std::stoi(argv[3]);
    std::uint32_t seed = static_cast<std::uint32_t>(std::stoul(argv[4]));
    double uf = std::stod(argv[5]);
    auto ops = fpaa::generate_operations(n, m, uf, seed);
    warmup_same_ops(n, ops);
    write_header(std::cout);
    Row a = run_variant<fpaa::DsuNaive>("naive", n, ops, seed);
    Row b = run_variant<fpaa::DsuUnionRank>("union_rank", n, ops, seed);
    Row c = run_variant<fpaa::DsuTarjan>("tarjan", n, ops, seed);
    write_row(std::cout, a);
    write_row(std::cout, b);
    write_row(std::cout, c);
    return 0;
  }
  print_usage(argv[0]);
  return 1;
}
