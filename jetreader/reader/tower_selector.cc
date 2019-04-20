#include "jetreader/reader/tower_selector.h"

#include "jetreader/lib/parse_csv.h"

namespace jetreader {

TowerSelector::TowerSelector() { clear(); }

bool TowerSelector::select(StPicoBTowHit *tower, unsigned id) {
  if ((bad_towers_active_ && !checkBadTowers(tower, id)))
    return false;
  return true;
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

void TowerSelector::clear() {
  bad_towers_active_ = false;

  bad_towers_.clear();
}

bool TowerSelector::checkBadTowers(StPicoBTowHit *tower, unsigned id) {
  return (bad_towers_.find(id) == bad_towers_.end());
}

} // namespace jetreader