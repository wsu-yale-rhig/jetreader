#ifndef JETREADER_READER_EVENT_SELECTOR_H
#define JETREADER_READER_EVENT_SELECTOR_H

#include "StPicoEvent/StPicoEvent.h"

namespace jetreader {

class EventSelector {
public:
  EventSelector() {}

  bool select(StPicoEvent *event) { return true; }

private:
};

} // namespace jetreader

#endif // JETREADER_READER_EVENT_SELECTOR_H