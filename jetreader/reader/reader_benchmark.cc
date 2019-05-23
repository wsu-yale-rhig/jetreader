#include "benchmark/benchmark.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/reader.h"

constexpr unsigned EVENTS = 500;

void StPicoDstReaderLoadAndRun() {
  std::string filename = jetreader::GetTestFile();
  StPicoDstReader reader(filename.c_str());
  reader.Init();
  for (int i = 0; i < EVENTS; ++i)
    reader.readPicoEvent(i);
}

static void BM_StPicoDstReader(benchmark::State &state) {
  for (auto _ : state)
    StPicoDstReaderLoadAndRun();
}

double StPicoDstReaderLoadAndRunTowersTracks() {
  std::string filename = jetreader::GetTestFile();
  StPicoDstReader reader(filename.c_str());
  reader.Init();
  double total = 0.0;
  for (int i = 0; i < EVENTS; ++i) {
    reader.readPicoEvent(i);
    for (int j = 0; j < reader.picoDst()->numberOfBTowHits(); ++j) {
      total += reader.picoDst()->btowHit(j)->energy();
    }
    for (int j = 0; j < reader.picoDst()->numberOfTracks(); ++j) {
      total += reader.picoDst()->track(j)->pPt();
    }
  }
  return total;
}

static void BM_StPicoDstReaderWithTowersTracks(benchmark::State &state) {
  for (auto _ : state)
    StPicoDstReaderLoadAndRunTowersTracks();
}

void JetReaderLoadAndRun() {
  std::string filename = jetreader::GetTestFile();
  jetreader::Reader reader(filename);
  reader.init();
  for (int i = 0; i < EVENTS; ++i) {
    reader.next();
  }
}

static void BM_JetReader(benchmark::State &state) {
  for (auto _ : state)
    JetReaderLoadAndRun();
}

BENCHMARK(BM_StPicoDstReader);
BENCHMARK(BM_StPicoDstReaderWithTowersTracks);
BENCHMARK(BM_JetReader);
BENCHMARK_MAIN();