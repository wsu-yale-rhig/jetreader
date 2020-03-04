#include "jetreader/reader/reader.h"
#include "jetreader/lib/assert.h"
#include "jetreader/reader/reader_utils.h"

#include <iostream>

#include "StPicoEvent/StPicoArrays.h"
#include "StPicoEvent/StPicoBEmcPidTraits.h"

namespace jetreader {

Reader::Reader(const std::string &input_file)
    : index_(-1), use_primary_tracks_(true),
      StPicoDstReader(input_file.c_str()), use_had_corr_(true),
      had_corr_fraction_(1.0), had_corr_map_(4800), use_mip_corr_(false),
      manager_(this) {
  event_selector_ = make_unique<EventSelector>();
  track_selector_ = make_unique<TrackSelector>();
  tower_selector_ = make_unique<TowerSelector>();
}

Reader::~Reader() {}

void Reader::loadConfig(const std::string &yaml_filename) {
  try {
    manager_.loadConfig(yaml_filename);
  } catch (std::exception &e) {
    std::cerr << "error loading config file: " << yaml_filename
              << "; caught exception: " << e.what() << std::endl;
  }
}

void Reader::writeConfig(const std::string &yaml_filename) {
  try {
    manager_.writeConfig(yaml_filename);
  } catch (std::exception &e) {
    std::cerr << "error writing config file: " << yaml_filename
              << "; caught exception: " << e.what() << std::endl;
  }
}

bool Reader::next() {

  if (chain() == nullptr) {
    JETREADER_THROW("No input file loaded: next() failed");
  }

  // last valid index in the chain, make sure we don't try to load past this
  int64_t last_event_index = chain()->GetEntries() - 1;

  // loop to find the next accepted event, or until we hit the end of the chain.
  // for the special case of when we find a bad run index, we will attempt to
  // speed-up running through the event chain by disabling all branches except
  // for the Event branch.
  while (chain()->GetReadEvent() < last_event_index) {
    EventStatus load_status = readEvent(++index_);

    switch (load_status) {
    case EventStatus::acceptEvent:
      return true;
    case EventStatus::rejectEvent:
      continue;
    case EventStatus::rejectRun:
      if (findNextGoodRun())
        return true;
      break;
    }
  }
  return false;
}

EventStatus Reader::readEvent(size_t idx) {
  // clear last event - prevents accumulation of stale pseudojets or tower-track
  // matches.
  clear();

  if (chain() == nullptr)
    JETREADER_THROW("No input file loaded: readEvent() failed");

  if (idx >= chain()->GetEntries() || idx < 0)
    JETREADER_THROW("Requested index: ", idx, "out of bounds: ", " chain has ",
                    chain()->GetEntries(), "events");

  // attempt to load the requested event
  index_ = idx;

  int load_status = chain()->GetEntry(idx);
  JETREADER_ASSERT(load_status > 0, "failure attempting to load event ", idx,
                   " in the chain, returned status ", load_status);

  // load the centrality first so that it is always calculated, and we never get
  // event de-syncs for whatever reason
  if (centrality_.isValid()) {
    centrality_.setEvent(
        picoDst()->event()->runId(), picoDst()->event()->refMult(),
        picoDst()->event()->ZDCx(), picoDst()->event()->primaryVertex().Z());
  }

  return makeEvent();
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

EventStatus Reader::makeEvent() {

  // EventSelector contains all event-level cuts - such as vertex position,
  // bad run lists, etc. So if any of those selections aren't passed, we can
  // stop without the relatively slow processing of tracks or towers
  EventStatus event_status = event_selector_->select(picoDst()->event());

  if (event_status != EventStatus::acceptEvent)
    return event_status;

  // now process all tracks and towers, after the event selection is passed
  if (chain()->GetBranchStatus("Track"))
    if (!selectTracks())
      return EventStatus::rejectEvent;
  if (chain()->GetBranchStatus("BTowHit"))
    if (!selectTowers())
      return EventStatus::rejectEvent;

  return EventStatus::acceptEvent;
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
      int match_tower_id = track->bemcTowerIndex();
      if (match_tower_id >= 0)
        had_corr_map_[match_tower_id].push_back(track_id);

    } else if (track_status == TrackStatus::rejectEvent) {
      event_status = false;
    }
  }
  return event_status;
}

bool Reader::selectTowers() {
  int init_size = pseudojets_.size();
  bool event_status = true;
  TVector3 vertex = picoDst()->event()->primaryVertex();
  for (unsigned tow_idx = 0; tow_idx < picoDst()->numberOfBTowHits();
       ++tow_idx) {
    StPicoBTowHit tower = *picoDst()->btowHit(tow_idx);
    unsigned tower_id = tow_idx + 1;
    double eta = bemc_helper_.towerEta(tower_id);
    double phi = bemc_helper_.towerPhi(tower_id);
    double corrected_eta =
        bemc_helper_.vertexCorrectedEta(tower_id, vertex.Z());
    TowerStatus tower_status =
        tower_selector_->select(&tower, tower_id, corrected_eta);
    if (tower_status == TowerStatus::acceptTower) {
      double e_corr = tower.energy();
      if (use_had_corr_)
        e_corr = towerHadronicCorrection(tow_idx);
      else if (use_mip_corr_)
        e_corr = towerMIPCorrection(tow_idx, eta);
      // check if corrected ET is still valid
      tower.setEnergy(e_corr);
      if (e_corr > 0.0 &&
          tower_selector_->select(&tower, tower_id, corrected_eta) ==
              TowerStatus::acceptTower) {
        pseudojets_.push_back(MakePseudoJet(tower, tower_id, eta, phi,
                                            corrected_eta, e_corr,
                                            had_corr_map_[tow_idx]));
      }
    } else if (tower_status == TowerStatus::rejectEvent) {
      event_status = false;
    }
  }
  return event_status;
}

double Reader::towerMIPCorrection(unsigned tow_idx, double tow_eta) {
  // copied from TStarJetPicoReader - has a note saying it may be 0.264
  // instead of 0.261. MIP value taken from spin group nick: as its written
  // its using eta - shouldn't it be using corrected eta?

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

bool Reader::findNextGoodRun() {
  std::vector<std::pair<std::string, int>> status_map;

  for (int i = 0; i < StPicoArrays::NAllPicoArrays; ++i) {
    std::string branchname = StPicoArrays::picoArrayNames[i];
    bool status = chain()->GetBranchStatus(branchname.c_str());
    status_map.push_back({branchname, status});

    // if i == 0, branch should be "Event" which we need on. Otherwise, we turn
    // all branches off to speedup reading
    if (i != 0) {
      chain()->SetBranchStatus(branchname.c_str(), 0);
    } else {
      chain()->SetBranchStatus(branchname.c_str(), 1);
    }
  }

  bool found_good_run = false;
  int current_event = index_;
  EventStatus event_status = event_selector_->select(picoDst()->event());

  while (event_status == EventStatus::rejectRun) {
    // attempt to load next entry
    ++current_event;

    int load_status = chain()->GetEntry(current_event);
    JETREADER_ASSERT(load_status > 0, "Failure attempting to load event ",
                     current_event, " in the chain, returned status ",
                     load_status);
    event_status = event_selector_->select(picoDst()->event());

    if (event_status != EventStatus::rejectRun) {
      found_good_run = true;
      break;
    }

    // check if we're at the end of the chain - if so, break
    if (current_event >= chain()->GetEntries() - 1)
      break;
  }

  // put branches back to their original state and reload the current event
  for (auto &branch : status_map)
    chain()->SetBranchStatus(branch.first.c_str(), branch.second);
  readEvent(current_event);

  return found_good_run;
}

} // namespace jetreader