#ifndef JETREADER_READER_TRACK_SELECTOR_H
#define JETREADER_READER_TRACK_SELECTOR_H

#include "StPicoEvent/StPicoTrack.h"

namespace jetreader {

class TrackSelector {
public:
  TrackSelector() {}

  virtual ~TrackSelector() {}

  virtual bool select(StPicoTrack *track) { return true; }

private:
};

} // namespace jetreader

#endif // JETREADER_READER_TRACK_SELECTOR_H