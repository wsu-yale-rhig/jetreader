#ifndef JETREADER_READER_READER_H
#define JETREADER_READER_READER_H

#include "jetreader/lib/memory.h"
#include "jetreader/reader/bemc_helper.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/tower_selector.h"
#include "jetreader/reader/track_selector.h"
#include "jetreader/reader/vector_info.h"

#include <map>
#include <string>
#include <vector>

#include "StPicoEvent/StPicoDstReader.h"

#include "fastjet/PseudoJet.hh"

namespace jetreader {

// returned by readEvent(idx)
enum class EventStatus { ioFailure, acceptEvent, rejectEvent };

class Reader : public StPicoDstReader {
public:
  // Reader initialization requires an input file name, and an optional
  // configuration file. The input file can be either a ROOT file containing a
  // PicoDst tree, or a file containing a list of picoDst files.
  Reader(const std::string &input_file);

  ~Reader();

  // Reads until the next event that satisfies event selection criteria is
  // found, or the end of the chain is reached. Returns false when it reaches
  // the end of the chain, or if there is an error during loading.
  bool next();

  // Reads in event at position idx in the chain, regardless of event selection
  // criteria. If loading is successful and event passes event cuts,  returns
  // EventStatus::pass. If the event does not pass event selection, returns
  // EventStatus::fail. If there is an io error, returns EventStatus::ioFailure
  EventStatus readEvent(size_t idx);

  // Initializes event, track and tower selectors and the reader. Must be called
  // before next() or readEvent(). If initialization fails, an exception is
  // raised.
  void init();

  // Switch between primary and global tracks. Primary tracks are the default
  void usePrimaryTracks() { use_primary_tracks_ = true; }
  void useGlobalTracks() { use_primary_tracks_ = false; }

  // turn on hadronic correction or MIP correction for towers. Fraction is the
  // percent of a track's pT to subtract from the corresponding tower ET in
  // hadronic correction. Only one correction scheme can be active at one time
  // - by default, 100% hadronic correction is on
  void useMIPCorrection(bool flag);
  void useHadronicCorrection(bool flag, double fraction = 1.0);
  bool MIPCorrection() const { return use_mip_corr_; }
  bool hadronicCorrection() const { return use_had_corr_; }

  // processes the event and returns a list of selected tracks and towers, which
  // have been converted into PseudoJets
  std::vector<fastjet::PseudoJet> &pseudojets();

  // direct access to event, track and tower selectors
  EventSelector *eventSelector() { return event_selector_.get(); }
  TrackSelector *trackSelector() { return track_selector_.get(); }
  TowerSelector *towerSelector() { return tower_selector_.get(); }

  // It is possible to create custom selector classes by inheriting from
  // EventSelector, TowerSelector or TrackSelector and overriding the select()
  // method. The reader will take ownership of the selector.
  void setEventSelector(EventSelector *selector);
  void setTrackSelector(TrackSelector *selector);
  void setTowerSelector(TowerSelector *selector);

  int currentEntry() { return chain()->GetReadEntry(); }
  int entries() { return chain()->GetEntries(); }

private:
  // used when reading a new event to clear state from previous
  // event
  void clear();

  // called by next() and readEvent() to process tracks and towers into
  // pseudojets. Returning failure indicates that the event should not be used
  // when calling next()
  bool makeEvent();

  // used internally by makeEvent(). These functions return false if the entire
  // event should be rejected, return false otherwise.
  bool selectTracks();
  bool selectTowers();

  // tower E correction schemes - either MIP or hadronic correction
  double towerMIPCorrection(unsigned tow_idx, double tow_eta);
  double towerHadronicCorrection(unsigned tow_idx);

  int64_t index_;

  bool use_primary_tracks_;

  bool use_had_corr_;
  double had_corr_fraction_;
  std::vector<std::vector<unsigned>> had_corr_map_;
  bool use_mip_corr_;

  unique_ptr<EventSelector> event_selector_;
  unique_ptr<TrackSelector> track_selector_;
  unique_ptr<TowerSelector> tower_selector_;

  BemcHelper bemc_helper_;

  std::vector<fastjet::PseudoJet> pseudojets_;
};

} // namespace jetreader

#endif // JETREADER_READER_READER_H