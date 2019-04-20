#ifndef JETREADER_READER_TOWER_SELECTOR_H
#define JETREADER_READER_TOWER_SELECTOR_H

#include "StPicoEvent/StPicoBTowHit.h"

#include <set>
#include <string>

namespace jetreader {

class TowerSelector {
public:
  TowerSelector();

  virtual ~TowerSelector() {}

  // method used by reader to select or reject a tower. Returns true if no
  // selection criteria are failed, returns false otherwise
  virtual bool select(StPicoBTowHit *tower, unsigned id);

  // add bad towers to the bad tower list (a bad tower is generally a tower that
  // is masked out at the analysis level due to faulty hardware, poor
  // calibration, etc)
  void addBadTower(unsigned tower_id);
  void addBadTowers(std::vector<unsigned> tower_ids);
  void addBadTowers(std::string filename);

  // resets all selection criteria to default (off)
  void clear();
  
protected:
  bool checkBadTowers(StPicoBTowHit *tower, unsigned id);

private:
  bool bad_towers_active_;

  std::set<unsigned> bad_towers_;
};

} // namespace jetreader

#endif // JETREADER_READER_TOWER_SELECTOR_H