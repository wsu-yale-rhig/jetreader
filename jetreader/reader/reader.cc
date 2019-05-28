#include "jetreader/reader/reader.h"
#include "jetreader/lib/assert.h"
#include "jetreader/reader/reader_utils.h"

#include <iostream>

#include "StPicoEvent/StPicoBEmcPidTraits.h"

namespace jetreader {

Reader::Reader(const std::string &input_file)
    : index_(-1), use_primary_tracks_(true),
      StPicoDstReader(input_file.c_str()), use_had_corr_(true),
      had_corr_fraction_(1.0), had_corr_map_(4800), use_mip_corr_(false) {
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

void Reader::init() {
  StPicoDstReader::Init();
  // make sure the event branch is loaded - otherwise, we can't use the data,
  // because we need vertex information, run ID, etc
  JETREADER_ASSERT(chain()->GetBranchStatus("Event"),
                   "Event branch is not loaded, can't process event");
}

void Reader::useMIPCorrection(bool flag) {
  use_mip_corr_ = flag;

  if (use_mip_corr_ == true)
    use_had_corr_ = false;
}

void Reader::useHadronicCorrection(bool flag, double fraction) {
  use_had_corr_ = flag;
  had_corr_fraction_ = fraction;

  if (use_had_corr_ == true) {
    use_mip_corr_ = false;
    JETREADER_ASSERT(had_corr_fraction_ >= 0.0,
                     "Hadronic correction fraction set to: ",
                     had_corr_fraction_, ", less than zero");
    JETREADER_ASSERT(had_corr_fraction_ <= 1.0,
                     "Hadronic correction fraction set to: ",
                     had_corr_fraction_, ", greater than one");
  }
}

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

void Reader::clear() {
  pseudojets_.clear();
  for (auto &c : had_corr_map_)
    c.clear();
}

bool Reader::makeEvent() {
  bool event_status = true;

  // EventSelector contains all event-level cuts - such as vertex position, bad
  // run lists, etc. So if any of those selections aren't passed, we can stop
  // without the relatively slow processing of tracks or towers
  if (!event_selector_->select(picoDst()))
    event_status = false;

  // now process all tracks and towers, after the event selection is passed
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
  for (int track_id = 0; track_id < picoDst()->numberOfTracks(); ++track_id) {
    StPicoTrack *track = picoDst()->track(track_id);
    TrackStatus track_status =
        track_selector_->select(track, vertex, use_primary_tracks_);
    if (track_status == TrackStatus::acceptTrack) {
      pseudojets_.push_back(MakePseudoJet(*track, vertex, use_primary_tracks_));

      // if we accept the track, then we will also use it for hadronic
      // correction/MIPS if it has been matched to a tower
      int pid_trait_id = track->bemcPidTraitsIndex();
      if (pid_trait_id >= 0 && chain()->GetBranchStatus("EmcPidTraits")) {
        StPicoBEmcPidTraits *bemctrait = picoDst()->bemcPidTraits(pid_trait_id);
        if (bemctrait == nullptr)
          continue;
        int match_tow_id = bemctrait->btowId();
        if (match_tow_id >= 0)
          had_corr_map_[match_tow_id - 1].push_back(track_id);
      }
    } else if (track_status == TrackStatus::rejectEvent)
      event_status = false;
  }
  return event_status;
}

bool Reader::selectTowers() {
  bool event_status = true;
  TVector3 vertex = picoDst()->event()->primaryVertex();
  for (unsigned tow_idx = 0; tow_idx < picoDst()->numberOfBTowHits();
       ++tow_idx) {
    StPicoBTowHit *tower = picoDst()->btowHit(tow_idx);
    unsigned tower_id = tow_idx + 1;
    double eta = bemc_helper_.towerEta(tower_id);
    double phi = bemc_helper_.towerPhi(tower_id);
    double corrected_eta =
        bemc_helper_.vertexCorrectedEta(tower_id, vertex.Z());
    TowerStatus tower_status =
        tower_selector_->select(tower, tower_id, corrected_eta);
    if (tower_status == TowerStatus::acceptTower) {
      double e_corr = tower->energy();
      if (use_had_corr_)
        e_corr = towerHadronicCorrection(tow_idx);
      else if (use_mip_corr_)
        e_corr = towerMIPCorrection(tow_idx, eta);
      if (e_corr > 0.0) {
        pseudojets_.push_back(
            MakePseudoJet(*tower, tower_id, eta, phi, corrected_eta, e_corr));
      }
    } else if (tower_status == TowerStatus::rejectEvent)
      event_status = false;
  }
  return event_status;
}

double Reader::towerMIPCorrection(unsigned tow_idx, double tow_eta) {
  // copied from TStarJetPicoReader - has a note saying it may be 0.264 instead
  // of 0.261. MIP value taken from spin group
  // nick: as its written its using eta - shouldn't it be using corrected eta?

  double tow_energy = picoDst()->btowHit(tow_idx)->energy();
  double theta = 2.0 * atan(exp(tow_eta));
  double mip_e = 0.261 * (1. + 0.056 * pow(tow_eta, 2.0)) / sin(theta); // GeV
  int n_tracks = had_corr_map_[tow_idx].size();
  double corrected_e = tow_energy - n_tracks * mip_e;
  return corrected_e;
}

double Reader::towerHadronicCorrection(unsigned tow_idx) {
  // hadronic correction subtracts had_corr_fraction_ percent of the total
  // momentum of each track that points to a tower from that tower's energy.
  // Deciding what tracks point to which towers is done during creation of the
  // StPicoDsts by extrapolating the track helix from the TPC into the barrel.

  double corrected_e = picoDst()->btowHit(tow_idx)->energy();
  for (auto &track_idx : had_corr_map_[tow_idx]) {
    StPicoTrack *track = picoDst()->track(track_idx);
    if (use_primary_tracks_)
      corrected_e -= track->pPtot() * had_corr_fraction_;
    else
      corrected_e -= track->gPtot() * had_corr_fraction_;
  }
  return corrected_e;
}

} // namespace jetreader