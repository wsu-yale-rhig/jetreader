#include "jetreader/reader/tower_selector.h"

#include "jetreader/lib/parse_csv.h"

namespace jetreader {

TowerSelector::TowerSelector() { clear(); }

TowerStatus TowerSelector::select(StPicoBTowHit *tower, unsigned id) {
  if ((bad_towers_active_ && !checkBadTowers(tower, id)))
    return TowerStatus::rejectTower;

  if ((et_active_ && !checkEt(tower, id))) {
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
}

void TowerSelector::setEtMax(double max) {
  JETREADER_ASSERT(max > 0, "ET cut must be greater than zero");
  max_et_ = max;
  et_active_ = true;
}

void TowerSelector::rejectEventOnEtFailure(bool flag) {
  reject_event_on_et_failure_ = flag;
}

void TowerSelector::clear() {
  bad_towers_active_ = false;
  et_active_ = false;
  reject_event_on_et_failure_ = true;

  bad_towers_.clear();

  max_et_ = 0.0;
}

bool TowerSelector::checkBadTowers(StPicoBTowHit *tower, unsigned id) {
  return (bad_towers_.find(id) == bad_towers_.end());
}

bool TowerSelector::checkEt(StPicoBTowHit *tower, unsigned id) {
  double et = tower->energy();
  return et < max_et_;
}

} // namespace jetreader