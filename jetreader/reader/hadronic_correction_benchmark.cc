#include "benchmark/benchmark.h"

#include <map>
#include <random>
#include <unordered_map>
#include <vector>
#include <iostream>

// testing the relative efficiency of using a vector or a map for storing
// information. Does not estimate the size of the structure, which would also be
// a good comparison to have.

// current conclusion: vectors are significantly faster than maps - unless it
// becomes apparent that memory becomes an issue, we should use the vector
// method

static void BM_HadCorrMap(benchmark::State &state) {
  std::map<unsigned, std::vector<unsigned>> tower_map;

  // RNG parameters needed
  std::uniform_int_distribution<int> nmatch_dist(0, 1500);
  std::uniform_int_distribution<int> tow_dist(0, 4800);
  std::random_device r;
  std::mt19937 gen(r());
  size_t total = 0;
  for (auto _ : state) {
    tower_map.clear();

    // number of matches in this event
    unsigned npidtraits = nmatch_dist(gen);

    // fill the associative container
    for (int i = 0; i < npidtraits; ++i) {
      unsigned towid = tow_dist(gen);
      tower_map[towid].push_back(i);
    }

    // loop over all containers
    for (auto &c : tower_map) {
      for (auto &t : c.second)
        total += t;
    }
  }
}

static void BM_HadCorrUnorderedMap(benchmark::State &state) {
  std::unordered_map<unsigned, std::vector<unsigned>> tower_map;

  // RNG parameters needed
  std::uniform_int_distribution<int> nmatch_dist(0, 1500);
  std::uniform_int_distribution<int> tow_dist(0, 4800);
  std::random_device r;
  std::mt19937 gen(r());
  size_t total = 0;
  for (auto _ : state) {
    tower_map.clear();

    // number of matches in this event
    unsigned npidtraits = nmatch_dist(gen);

    // fill the associative container
    for (int i = 0; i < npidtraits; ++i) {
      unsigned towid = tow_dist(gen);
      tower_map[towid].push_back(i);
    }

    // loop over all containers
    for (auto &c : tower_map) {
      for (auto &t : c.second)
        total += t;
    }
  }
}

static void BM_HadCorrVector(benchmark::State &state) {
  std::vector<std::vector<unsigned>> tower_map(4800);

  // RNG parameters needed
  std::uniform_int_distribution<int> nmatch_dist(0, 1500);
  std::uniform_int_distribution<int> tow_dist(0, 4799);
  std::uniform_real_distribution<double> prob(0, 1.0);
  std::random_device r;
  std::mt19937 gen(r());
  size_t total = 0;
  
  for (auto _ : state) {
    // clear the container
    for (auto &c : tower_map)
      c.clear();

    // number of matches in this event
    unsigned npidtraits = nmatch_dist(gen);

    // fill the associative container
    for (int i = 0; i < npidtraits; ++i) {
      unsigned towid = tow_dist(gen);
      tower_map[towid].push_back(i);
    }
    for (auto &c : tower_map) {
      for (auto &t : c)
        total += t;
    }
  }
}

BENCHMARK(BM_HadCorrVector);
BENCHMARK(BM_HadCorrMap);
BENCHMARK(BM_HadCorrUnorderedMap);
BENCHMARK_MAIN();