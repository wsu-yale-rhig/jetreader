#ifndef JETREADER_READER_EVENT_SELECTOR_H
#define JETREADER_READER_EVENT_SELECTOR_H

#include "StPicoEvent/StPicoDst.h"

namespace jetreader {

class EventSelector {
public:
  EventSelector() {}

  virtual ~EventSelector() {}

  virtual bool select(StPicoDst *event) { return true; }

private:
};

} // namespace jetreader

#endif // JETREADER_READER_EVENT_SELECTOR_H