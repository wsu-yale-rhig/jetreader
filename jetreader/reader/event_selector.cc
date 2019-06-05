#include "jetreader/reader/event_selector.h"

#include "jetreader/lib/assert.h"
#include "jetreader/lib/parse_csv.h"
#include "jetreader/reader/trigger_lookup.h"

#include "StPicoEvent/StPicoEvent.h"

namespace jetreader {

EventSelector::EventSelector() { clear(); }

bool EventSelector::select(StPicoDst *dst) {
  if ((vx_active_ && !checkVx(dst)) || (vy_active_ && !checkVy(dst)) ||
      (vz_active_ && !checkVz(dst)) || (dvz_active_ && !checkdVz(dst)) ||
      (vr_active_ && !checkVr(dst)) ||
      (refmult_active_ && !checkRefMult(dst)) ||
      (trigger_ids_active_ && !checkTriggerId(dst)) ||
      (bad_run_ids_active_ && !checkRunId(dst)))
    return false;
  return true;
}

void EventSelector::setVxRange(double min, double max) {
  JETREADER_ASSERT(max > min, "max Vx must be greater than min Vx");
  vx_min_ = min;
  vx_max_ = max;
  vx_active_ = true;
}

void EventSelector::setVyRange(double min, double max) {
  JETREADER_ASSERT(max > min, "max Vy must be greater than min Vy");
  vy_min_ = min;
  vy_max_ = max;
  vy_active_ = true;
}
void EventSelector::setVzRange(double min, double max) {
  JETREADER_ASSERT(max > min, "max Vz must be greater than min Vz");
  vz_min_ = min;
  vz_max_ = max;
  vz_active_ = true;
}

void EventSelector::setdVzMax(double max) {
  JETREADER_ASSERT(max > 0.0, "max dVz must be greater than 0.0");
  dvz_max_ = max;
  dvz_active_ = true;
}

void EventSelector::setVrMax(double max) {
  JETREADER_ASSERT(max > 0.0, "max Vr must be greater than 0.0");
  vr_max_ = max;
  vr_active_ = true;
}

void EventSelector::setRefMultRange(unsigned min, unsigned max, MultType mult) {
  JETREADER_ASSERT(max > min, "max refmult must be greater than min refmult");
  refmult_min_ = min;
  refmult_max_ = max;
  refmult_active_ = true;
  refmult_type_ = mult;
}

void EventSelector::addTriggerId(unsigned id) {
  trigger_ids_.insert(id);
  trigger_ids_active_ = true;
}
void EventSelector::addTriggerIds(std::string id_string) {
  auto result = GetTriggerIDs(id_string);
  for (auto &entry : result)
    addTriggerId(entry);

  if (trigger_ids_.size() > 0)
    trigger_ids_active_ = true;

  trigger_id_strings_.insert(id_string);
}

void EventSelector::addBadRuns(std::vector<unsigned> bad_runs) {
  for (auto &entry : bad_runs)
    bad_run_ids_.insert(entry);
  if (bad_run_ids_.size() > 0)
    bad_run_ids_active_ = true;
}
void EventSelector::addBadRuns(std::string bad_run_file) {
  auto runs = ParseCsv<unsigned>(bad_run_file);
  for (auto &line : runs) {
    for (auto &entry : line)
      bad_run_ids_.insert(entry);
  }
  if (bad_run_ids_.size() > 0)
    bad_run_ids_active_ = true;

  bad_run_id_files_.insert(bad_run_file);
}

void EventSelector::clear() {
  trigger_ids_active_ = false;
  bad_run_ids_active_ = false;
  vx_active_ = false;
  vy_active_ = false;
  vz_active_ = false;
  dvz_active_ = false;
  vr_active_ = false;
  refmult_active_ = false;

  trigger_ids_.clear();
  bad_run_ids_.clear();
  bad_run_id_files_.clear();

  vx_min_ = 0.0;
  vx_max_ = 0.0;
  vy_min_ = 0.0;
  vy_max_ = 0.0;
  vz_min_ = 0.0;
  vz_max_ = 0.0;
  dvz_max_ = 0.0;
  vr_max_ = 0.0;
  refmult_type_ = MultType::refMult;
  refmult_min_ = 0;
  refmult_max_ = 0;
}

bool EventSelector::checkVx(StPicoDst *dst) {
  double vx = dst->event()->primaryVertex().X();
  return vx > vx_min_ && vx < vx_max_;
}

bool EventSelector::checkVy(StPicoDst *dst) {
  double vy = dst->event()->primaryVertex().Y();
  return vy > vy_min_ && vy < vy_max_;
}

bool EventSelector::checkVz(StPicoDst *dst) {
  double vz = dst->event()->primaryVertex().Z();
  return vz > vz_min_ && vz < vz_max_;
}

bool EventSelector::checkdVz(StPicoDst *dst) {
  double dvz = abs(dst->event()->vzVpd() - dst->event()->primaryVertex().Z());
  return dvz < dvz_max_;
}

bool EventSelector::checkVr(StPicoDst *dst) {
  double vx = dst->event()->primaryVertex().X();
  double vy = dst->event()->primaryVertex().Y();
  double vr = sqrt(vx * vx + vy * vy);
  return vr < vr_max_;
}

bool EventSelector::checkRefMult(StPicoDst *dst) {
  int refmult = 0;
  switch (refmult_type_) {
  case MultType::refMult:
    refmult = dst->event()->refMult();
    break;
  case MultType::refMult2:
    refmult = dst->event()->refMult2();
    break;
  case MultType::refMult3:
    refmult = dst->event()->refMult3();
    break;
  case MultType::refMult4:
    refmult = dst->event()->refMult4();
    break;
  case MultType::gRefMult:
    refmult = dst->event()->grefMult();
    break;
  }
  return refmult > refmult_min_ && refmult < refmult_max_;
}

bool EventSelector::checkTriggerId(StPicoDst *dst) {
  bool triggered = false;
  for (auto &trigger : trigger_ids_)
    if (dst->event()->isTrigger(trigger))
      triggered = true;
  return triggered;
}

bool EventSelector::checkRunId(StPicoDst *dst) {
  unsigned runid = dst->event()->runId();
  return bad_run_ids_.find(runid) == bad_run_ids_.end();
}

} // namespace jetreader