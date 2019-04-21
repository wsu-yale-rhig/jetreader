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
  // clear last event
  clear();

  if (chain() == nullptr) {
    JETREADER_THROW("No input file loaded: next() failed");
  }

  // last valid index in the chain
  int64_t last_event_index = chain()->GetEntries() - 1;

  // we count the number of IO errors - if it passes
  // a threshold (10, for now), we exit
  int io_errors = 0;

  while (chain()->GetReadEvent() < last_event_index) {
    // attempt to load the next event
    EventStatus load_status = readEvent(++index_);

    switch (load_status) {
    case EventStatus::acceptEvent:
      return true;
    case EventStatus::rejectEvent:
      continue;
    case EventStatus::ioFailure:
      if (++io_errors >= 10)
        JETREADER_THROW("IO failure for ", io_errors, " events: exiting");
    }
  }
  return false;
}

EventStatus Reader::readEvent(size_t idx) {
  // clear last event
  clear();

  if (chain() == nullptr)
    JETREADER_THROW("No input file loaded: readEvent() failed");

  if (idx >= chain()->GetEntries() || idx < 0)
    JETREADER_THROW("Requested index: ", idx, "out of bounds: ", " chain has ",
                    chain()->GetEntries(), "events");

  // attempt to load the requested event
  index_ = idx;
  if (chain()->GetEntry(idx) == 0)
    return EventStatus::ioFailure;

  // process event
  if (makeEvent() == true)
    return EventStatus::acceptEvent;
  else
    return EventStatus::rejectEvent;
}

void Reader::init() { StPicoDstReader::Init(); }

std::vector<fastjet::PseudoJet> &Reader::pseudojets() {
  // make sure the event was loaded through readEvent(), not directly through
  // the chain; this prevents loading an event in the chain and getting a
  // "stale" set of pseudojets
  if (chain()->GetReadEvent() != index_) {
    readEvent(chain()->GetReadEvent());
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

bool Reader::makeEvent() {
  bool event_status = true;

  // first, check event with EventSelector
  if (!event_selector_->select(picoDst()))
    event_status = false;

  // now process all tracks and towers, if they are loaded
  if (chain()->GetBranchStatus("Track"))
    if (!selectTracks())
      event_status = false;
  if (chain()->GetBranchStatus("BTowHit"))
    if (!selectTowers())
      event_status = false;

  return event_status;
}

bool Reader::selectTracks() {
  bool event_status = true;
  TVector3 vertex = picoDst()->event()->primaryVertex();
  if (use_primary_tracks_) {
    for (unsigned i = 0; i < picoDst()->numberOfTracks(); ++i) {
      StPicoTrack *track = picoDst()->track(i);
      if (track->isPrimary()) {
        TrackStatus track_status = track_selector_->select(track, vertex);
        if (track_status == TrackStatus::acceptTrack)
          pseudojets_.push_back(MakePseudoJet(*track, vertex, true));
        else if (track_status == TrackStatus::rejectEvent)
          event_status = false;
      }
    }
  } else {
    for (unsigned i = 0; i < picoDst()->numberOfTracks(); ++i) {
      StPicoTrack *track = picoDst()->track(i);
      TrackStatus track_status = track_selector_->select(track, vertex, false);
      if (track_status == TrackStatus::acceptTrack)
        pseudojets_.push_back(MakePseudoJet(*track, vertex, false));
      else if (track_status == TrackStatus::rejectEvent)
        event_status = false;
    }
  }
  return event_status;
}

bool Reader::selectTowers() {
  bool event_status = true;
  TVector3 vertex = picoDst()->event()->primaryVertex();
  for (unsigned i = 0; i < picoDst()->numberOfBTowHits(); ++i) {
    StPicoBTowHit *tower = picoDst()->btowHit(i);
    TowerStatus tower_status = tower_selector_->select(tower, i);
    if (tower_status == TowerStatus::acceptTower)
      pseudojets_.push_back(MakePseudoJet(*tower, vertex, i));
    else if (tower_status == TowerStatus::rejectEvent)
      event_status = false;
  }
  return event_status;
}

} // namespace jetreader