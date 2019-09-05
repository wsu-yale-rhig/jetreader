#ifndef JETREADER_READER_READER_H
#define JETREADER_READER_READER_H

#include "jetreader/lib/memory.h"
#include "jetreader/reader/bemc_helper.h"
#include "jetreader/reader/centrality.h"
#include "jetreader/reader/config/config_manager.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/tower_selector.h"
#include "jetreader/reader/track_selector.h"
#include "jetreader/reader/vector_info.h"

#include <string>
#include <vector>

#include "StPicoEvent/StPicoDstReader.h"

#include "fastjet/PseudoJet.hh"

namespace jetreader {

class ReaderConfigHelper;

class Reader : public StPicoDstReader {
public:
  friend class ReaderConfigHelper;

  // Reader initialization requires an input file name, and an optional
  // configuration file. The input file can be either a ROOT file containing a
  // PicoDst tree, or a file containing a list of picoDst files.
  Reader(const std::string &input_file);

  ~Reader();

  // Loads a YAML config file. The details of the expected configuration file
  // can be found in the jetreader/reader/config headers, in the ConfigManager
  // and the ConfigHelpers. And example is located in the jetreader root
  // directory under docs/other/example_config.yaml.
  void loadConfig(const std::string &yaml_filename);

  // dumps config to file in YAML format. Records the current state of the
  // reader and its selectors.
  void writeConfig(const std::string &yaml_filename);

  // Reads until the next event that satisfies event selection criteria is
  // found, or the end of the chain is reached. Returns false when it reaches
  // the end of the chain, or if there is an error during loading.
  bool next();

  // Reads in event at position idx in the chain, regardless of event selection
  // criteria. If loading is successful and event passes event cuts,  returns
  // EventStatus::acceptEvent. If the event does not pass event selection,
  // returns EventStatus:rejectEvent. If there is an io error, returns
  // EventStatus::ioFailure
  EventStatus readEvent(size_t idx);

  // Initializes event, track and tower selectors and the reader. Must be called
  // before next() or readEvent(). If initialization fails, an exception is
  // raised.
  void init();

  // Switch between primary and global tracks. Primary tracks are the default
  void usePrimaryTracks() { use_primary_tracks_ = true; }
  void useGlobalTracks() { use_primary_tracks_ = false; }
  bool primaryTracks() { return use_primary_tracks_; }
  bool globalTracks() { return !use_primary_tracks_; }

  // turn on hadronic correction or MIP correction for towers. Fraction is the
  // percent of a track's pT to subtract from the corresponding tower ET in
  // hadronic correction. Only one correction scheme can be active at one time
  // - by default, 100% hadronic correction is on
  void useMIPCorrection(bool flag);
  void useHadronicCorrection(bool flag, double fraction = 1.0);
  bool MIPCorrection() const { return use_mip_corr_; }
  bool hadronicCorrection() const { return use_had_corr_; }
  double hadronicCorrectionFraction() const { return had_corr_fraction_; }

  // processes the event and returns a list of selected tracks and towers, which
  // have been converted into PseudoJets
  std::vector<fastjet::PseudoJet> &pseudojets();

  // returns the StRefMultCorr-compatible centrality implementation of the
  // reader. Before centrality9() or centrality16() can be used, the user must
  // call reader.centrality().loadCentralityDef(id) with the proper CentDefId
  // for their given dataset. No default is set, to avoid errors. Event weight
  // and definition parameters can be accessed through the Centrality class.
  Centrality &centrality() { return centrality_; }

  // returns the STAR 16 or 9 bin centrality definition for the current event.
  // special values:
  // -1 = the event is not valid for the given definition, either due to run id,
  // vz, luminosity, etc, or no centrality definition has been loaded
  // 16 (or 9) = the event is in the 80-100% centrality bin
  int centrality16() { return centrality_.centrality16(); }
  int centrality9() { return centrality_.centrality9(); }

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

  int64_t currentEntry() { return chain()->GetReadEntry(); }
  int64_t entries() { return chain()->GetEntries(); }

private:
  // used when reading a new event to clear state from previous
  // event
  void clear();

  // called by next() and readEvent() to process tracks and towers into
  // pseudojets. Returning failure indicates that the event should not be used
  // when calling next()
  EventStatus makeEvent();

  // used internally by makeEvent(). These functions return false if the entire
  // event should be rejected, return true otherwise.
  bool selectTracks();
  bool selectTowers();

  // tower E correction schemes - either MIP or hadronic correction
  double towerMIPCorrection(unsigned tow_idx, double tow_eta);
  double towerHadronicCorrection(unsigned tow_idx);

  // used to speed-up reading through consecutive events in bad runs which won't
  // be processed. Disables large branches such as tracks and towers and scans
  // each event runID without processing the full event. Returns true if a new run
  // is found, returns false at the end of the chain.
  bool findNextGoodRun();

  int64_t index_;

  bool use_primary_tracks_;

  bool use_had_corr_;
  double had_corr_fraction_;
  std::vector<std::vector<unsigned>> had_corr_map_;
  bool use_mip_corr_;

  ConfigManager manager_;

  Centrality centrality_;

  unique_ptr<EventSelector> event_selector_;
  unique_ptr<TrackSelector> track_selector_;
  unique_ptr<TowerSelector> tower_selector_;

  BemcHelper bemc_helper_;

  std::vector<fastjet::PseudoJet> pseudojets_;
};

} // namespace jetreader

#endif // JETREADER_READER_READER_H
