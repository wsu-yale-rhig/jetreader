#include "jetreader/reader/tower_selector.h"

#include "jetreader/lib/assert.h"
#include "jetreader/lib/parse_csv.h"

#include <cmath>
#include <iostream>

namespace jetreader {

TowerSelector::TowerSelector() { clear(); }

TowerStatus TowerSelector::select(StPicoBTowHit *tower, unsigned id,
                                  double eta) {
  if ((bad_towers_active_ && !checkBadTowers(tower, id)) ||
      (et_min_active_ && !checkEtMin(tower, eta)))
    return TowerStatus::rejectTower;

  if ((et_max_active_ && !checkEtMax(tower, eta))) {
    if (reject_event_on_et_failure_)
      return TowerStatus::rejectEvent;
    else
      return TowerStatus::rejectTower;
  }
  return TowerStatus::acceptTower;
}

void TowerSelector::addBadTower(unsigned tower_id) {
  bad_towers_.insert(tower_id);
  bad_towers_active_ = true;
}

void TowerSelector::addBadTowers(std::vector<unsigned> tower_ids) {
  for (auto &tow : tower_ids)
    bad_towers_.insert(tow);
  if (bad_towers_.size() > 0)
    bad_towers_active_ = true;
}

void TowerSelector::addBadTowers(std::string filename) {
  auto towers = ParseCsv<unsigned>(filename);
  for (auto &line : towers) {
    for (auto &entry : line)
      bad_towers_.insert(entry);
  }
  if (bad_towers_.size() > 0)
    bad_towers_active_ = true;

  bad_tower_files_.insert(filename);
}

void TowerSelector::setEtMax(double max) {
  JETREADER_ASSERT(max > 0, "ET cut must be greater than zero");
  max_et_ = max;
  et_max_active_ = true;
}

void TowerSelector::rejectEventOnEtFailure(bool flag) {
  reject_event_on_et_failure_ = flag;
}

void TowerSelector::setEtMin(double min) {
  JETREADER_ASSERT(min > 0, "ET cut must be greater than zero");
  min_et_ = min;
  et_min_active_ = true;
}

void TowerSelector::clear() {
  bad_towers_active_ = false;
  et_max_active_ = false;
  reject_event_on_et_failure_ = true;
  et_min_active_ = false;

  bad_towers_.clear();
  bad_tower_files_.clear();

  max_et_ = 0.0;
  min_et_ = 0.0;
}

bool TowerSelector::checkBadTowers(StPicoBTowHit *tower, unsigned id) {
  return (bad_towers_.find(id) == bad_towers_.end());
}

bool TowerSelector::checkEtMax(StPicoBTowHit *tower, double eta) {
  double et = tower->energy() / cosh(eta);
  return et < max_et_;
}

bool TowerSelector::checkEtMin(StPicoBTowHit *tower, double eta) {
  double et = tower->energy() / cosh(eta);
  return et > min_et_;
}

} // namespace jetreader