#ifndef JETREADER_READER_TOWER_SELECTOR_H
#define JETREADER_READER_TOWER_SELECTOR_H

#include "StPicoEvent/StPicoBTowHit.h"

namespace jetreader {

class TowerSelector {
public:
  TowerSelector() {}

  virtual ~TowerSelector() {}

  virtual bool select(StPicoBTowHit *tower) { return true; }

private:
};

} // namespace jetreader

#endif // JETREADER_READER_TOWER_SELECTOR_H