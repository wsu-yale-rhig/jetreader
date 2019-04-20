#include "jetreader/reader/reader.h"
#include "jetreader/lib/assert.h"
#include "jetreader/reader/reader_utils.h"

#include <iostream>

namespace jetreader {

Reader::Reader(const std::string &input_file)
    : index_(-1), use_primary_tracks_(true),
      StPicoDstReader(input_file.c_str()) {
  event_selector_ = make_unique<EventSelector>();
  track_selector_ = make_unique<TrackSelector>();
  tower_selector_ = make_unique<TowerSelector>();
}

Reader::~Reader() {}

bool Reader::next() {
  if (chain() == nullptr) {
    JETREADER_THROW("No input file loaded: next() failed");
  }

  // clear last event
  clear();

  // last valid index in the chain
  int64_t last_event_index = chain()->GetEntries() - 1;

  // we count the number of IO errors - if it passes
  // a threshold (10, for now), we exit
  int io_errors = 0;

  while (chain()->GetReadEvent() < last_event_index) {
    // attempt to load the next event
    bool status = readEvent(++index_);

    if (status) {
      if (event_selector_->select(picoDst())) {
        return true;
      } else {
        continue;
      }
    } else {
      if (++io_errors >= 10)
        JETREADER_THROW("IO failure for ", io_errors, " events: exiting");
    }
  }
  return false;
}

bool Reader::readEvent(size_t idx) {
  if (chain() == nullptr) {
    JETREADER_THROW("No input file loaded: readEvent() failed");
  }

  // clear last event
  clear();

  if (idx >= chain()->GetEntries() || idx < 0) {
    JETREADER_THROW("Requested index: ", idx, "out of bounds: ", " chain has ",
                    chain()->GetEntries(), "events");
  }

  index_ = idx;
  return chain()->GetEntry(idx);
}

void Reader::init() { StPicoDstReader::Init(); }

std::vector<fastjet::PseudoJet> &Reader::pseudojets() {
  // make sure the event was loaded through readEvent(), not directly through
  // the chain
  if (chain()->GetReadEvent() != index_) {
    readEvent(chain()->GetReadEvent());
  }

  // if the event has already been processed, we don't have to redo the work
  if (pseudojets_.size() > 0) {
    return pseudojets_;
  }
  bool status = false;
  if (chain()->GetBranchStatus("Track")) {
    selectTracks();
    status = true;
  }
  if (chain()->GetBranchStatus("BTowHit")) {
    // selectTowers();
    status = true;
  }

  if (!status) {
    std::cerr << "requesting pseudojets, but Tracks and Barrel Calorimeter hit "
                 "branches have been disabled"
              << std::endl;
  }

  return pseudojets_;
}

void Reader::setEventSelector(EventSelector *selector) {
  event_selector_ = unique_ptr<EventSelector>(selector);
}

void Reader::setTrackSelector(TrackSelector *selector) {
  track_selector_ = unique_ptr<TrackSelector>(selector);
}

void Reader::setTowerSelector(TowerSelector *selector) {
  tower_selector_ = unique_ptr<TowerSelector>(selector);
}

void Reader::clear() { pseudojets_.clear(); }

void Reader::selectTracks() {
  TVector3 vertex = picoDst()->event()->primaryVertex();
  if (use_primary_tracks_) {
    for (unsigned i = 0; i < picoDst()->numberOfTracks(); ++i) {
      StPicoTrack *track = picoDst()->track(i);
      if (track->isPrimary() && track_selector_->select(track, vertex)) {
        pseudojets_.push_back(MakePseudoJet(*track, vertex, true));
      }
    }
  } else {
    for (unsigned i = 0; i < picoDst()->numberOfTracks(); ++i) {
      StPicoTrack *track = picoDst()->track(i);
      if (track_selector_->select(track, vertex)) {
        pseudojets_.push_back(MakePseudoJet(*track, vertex, false));
      }
    }
  }
}

void Reader::selectTowers() {
  TVector3 vertex = picoDst()->event()->primaryVertex();
  for (unsigned i = 0; i < picoDst()->numberOfBTowHits(); ++i) {
    StPicoBTowHit *tower = picoDst()->btowHit(i);
    if (tower_selector_->select(tower)) {
      pseudojets_.push_back(MakePseudoJet(*tower, vertex, i));
    }
  }
}

} // namespace jetreader