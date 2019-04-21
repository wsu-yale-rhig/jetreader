#ifndef JETREADER_READER_TOWER_SELECTOR_H
#define JETREADER_READER_TOWER_SELECTOR_H

#include "StPicoEvent/StPicoBTowHit.h"

#include <set>
#include <string>

namespace jetreader {

enum class TowerStatus { rejectEvent, rejectTower, acceptTower };

class TowerSelector {
public:
  TowerSelector();

  virtual ~TowerSelector() {}

  // method used by reader to select or reject a tower. Returns true if no
  // selection criteria are failed, returns false otherwise
  virtual TowerStatus select(StPicoBTowHit *tower, unsigned id);

  // add bad towers to the bad tower list (a bad tower is generally a tower that
  // is masked out at the analysis level due to faulty hardware, poor
  // calibration, etc)
  void addBadTower(unsigned tower_id);
  void addBadTowers(std::vector<unsigned> tower_ids);
  void addBadTowers(std::string filename);

  // add a max ET cut for towers
  void setEtMax(double max);

  // if this flag is turned on, if any tower fails the ET cut then the entire
  // event is rejected. This is turned on by default
  void rejectEventOnEtFailure(bool flag = true);

  // resets all selection criteria to default (off)
  void clear();

  // access to the bad tower list - used by the EventSelector
  const std::set<unsigned> &badTowers() { return bad_towers_; }

protected:
  bool checkBadTowers(StPicoBTowHit *tower, unsigned id);
  bool checkEt(StPicoBTowHit *tower, unsigned eta);

private:
  bool bad_towers_active_;
  bool et_active_;
  bool reject_event_on_et_failure_;

  std::set<unsigned> bad_towers_;

  double max_et_;
};

} // namespace jetreader

#endif // JETREADER_READER_TOWER_SELECTOR_H