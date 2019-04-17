#ifndef JETREADER_READER_READER_H
#define JETREADER_READER_READER_H

#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/track_selector.h"
#include "jetreader/reader/tower_selector.h"

#include <string>

#include "StPicoEvent/StPicoDstReader.h"

namespace jetreader {

class Reader : public StPicoDstReader {
public:
  // Reader initialization requires an input file name, and an
  // optional configuration file. The input file can be either
  // a ROOT file containing a PicoDst tree, or a file 
  // containing a list of picoDst files.
  Reader(const std::string &input_file);

  ~Reader() {}

  // Reads until the next event that satisfies event selection
  // criteria is found, or the end of the chain is reached.
  // Returns false when it reaches the end of the chain.
  bool next();

  // reads in event at position idx in the chain, regardless 
  // of event selection criteria. If loading is successful,
  // returns true, otherwise, returns false.
  bool readEvent(size_t idx);

private:
// current event counter
size_t current_event_;

EventSelector event_selector_;
TrackSelector track_selector_;
TowerSelector tower_selector_;

};

} // namespace jetreader

#endif // JETREADER_READER_READER_H