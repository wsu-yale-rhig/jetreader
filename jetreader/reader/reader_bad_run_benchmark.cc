#include "benchmark/benchmark.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/reader.h"

#include <vector>


void ReaderLoadAndRun() {
  std::string filename = jetreader::GetTestFile();
  jetreader::Reader reader(filename.c_str());
  reader.init();
  while(reader.next()) {
    continue;
  }
}

static void BM_Reader(benchmark::State &state) {
  for (auto _ : state)
    ReaderLoadAndRun();
}

void ReaderLoadAndRunBadRuns() {
  std::string filename = jetreader::GetTestFile();
  jetreader::Reader reader(filename.c_str());
  std::vector<unsigned> bad_run;
  bad_run.push_back(15095020);
  reader.eventSelector()->addBadRuns(bad_run);
  reader.init();
  while(reader.next()) {
    continue;
  }
}

static void BM_BadRunReader(benchmark::State &state) {
  for (auto _ : state)
    ReaderLoadAndRunBadRuns();
}

BENCHMARK(BM_Reader);
BENCHMARK(BM_BadRunReader);
BENCHMARK_MAIN();