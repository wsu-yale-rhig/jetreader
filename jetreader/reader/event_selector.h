#ifndef JETREADER_READER_EVENT_SELECTOR_H
#define JETREADER_READER_EVENT_SELECTOR_H

#include "StPicoEvent/StPicoDst.h"

#include <set>
#include <string>

namespace jetreader {

enum class MultType { REFMULT, REFMULT2, REFMULT3, REFMULT4, GREFMULT };

class EventSelector {
public:
  EventSelector();

  virtual ~EventSelector() {}

  // Primary method to check an event. Returns true if no active selection
  // critera are failed, returns false otherwise
  virtual bool select(StPicoDst *dst);

  // Select on primary vertex position (vz = direction along the beam pipe,
  // vx/vy are in the transverse plane relative to the beam line)
  void setVxRange(double min, double max);
  void setVyRange(double min, double max);
  void setVzRange(double min, double max);

  // Select on |vz - VPD vz| (vpd = vertex position detector)
  void setdVzMax(double max);

  // Select on refmult. Can choose refmult1 - refmult4 or grefmult (refmult, in
  // any form, is a measure of the charged track multiplicity in the event -
  // correlates with centrality)
  void setRefMultRange(unsigned min, unsigned max,
                       MultType mult = MultType::REFMULT);

  // Add trigger IDs - once any trigger ID is added, any event without at least
  // one of the added trigger IDs will be rejected (trigger ID = )
  void addTriggerId(unsigned id);
  void addTriggerIds(std::string id_string);

  // add a list of runs to reject (a run is a contiguous set of events that were
  // recorded at one time at STAR. Generally lasts 30 minutes to a few hours,
  // depending on data-taking rates)
  void addBadRuns(std::vector<unsigned> bad_runs);
  void addBadRuns(std::string bad_run_file);

  // selectors for maximum track pT and tower ET in the event. If any track or
  // tower is found above this threshold, the whole event is rejected
  void setTrackPtMax(double max, bool use_globals = false);
  void setTowerEtMax(double max);

  // function to deactivate and reset all cuts
  void clear();

protected:
  bool checkVx(StPicoDst *dst);
  bool checkVy(StPicoDst *dst);
  bool checkVz(StPicoDst *dst);
  bool checkdVz(StPicoDst *dst);
  bool checkRefMult(StPicoDst *dst);
  bool checkTriggerId(StPicoDst *dst);
  bool checkRunId(StPicoDst *dst);
  bool checkMaxPt(StPicoDst *dst);
  bool checkMaxEt(StPicoDst *dst);

private:
  bool trigger_ids_active_;
  bool bad_run_ids_active_;
  bool vx_active_;
  bool vy_active_;
  bool vz_active_;
  bool dvz_active_;
  bool refmult_active_;
  bool max_pt_active_;
  bool max_et_active_;

  std::set<unsigned> trigger_ids_;

  std::set<unsigned> bad_run_ids_;

  double vx_min_;
  double vx_max_;
  double vy_min_;
  double vy_max_;
  double vz_min_;
  double vz_max_;
  double dvz_max_;

  MultType refmult_type_;
  unsigned refmult_min_;
  unsigned refmult_max_;

  bool use_globals_;
  double max_pt_;
  double max_et_;
};

} // namespace jetreader

#endif // JETREADER_READER_EVENT_SELECTOR_H