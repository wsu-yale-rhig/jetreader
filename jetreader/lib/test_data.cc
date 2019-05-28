#include "jetreader/lib/test_data.h"

#include "jetreader/lib/assert.h"

namespace jetreader {

#if defined(WIN32) || defined(_WIN32)

// not implemented for windows
std::string GetTestFile() { return std::string(); }

#else

#include <unistd.h>

std::string GetTestFile() {
  // get hostname
  int hostname_length = 256;
  char hostname[hostname_length];
  gethostname(hostname, hostname_length);
  std::string host(hostname);
  std::transform(host.begin(), host.end(), host.begin(), ::tolower);

  // check for any known machines
  std::string filepath;
  auto fail = std::string::npos;

  if (host.find("warrior") != fail || host.find("rhi1") != fail ||
      host.find("rhi2") != fail || host.find("rhi3") != fail ||
      host.find("rhi4") != fail || host.find("wsu") != fail)
    filepath = "/nfs/rhi/STAR/Data/jetreader_test_pico/"
               "jetreader_p18ih_auau_2014_test.picoDst.root";
  else if (host.find("gauss") != fail)
    filepath = "/Users/nick/physics/data/pico_test/"
               "jetreader_p18ih_auau_2014_test.picoDst.root";
  else
    JETREADER_THROW("Could not identify hostname: no input file found");

  return filepath;
}

#endif

void TurnOffBranches(jetreader::Reader &r) {
  std::set<std::string> branches{
      "Track",          "EmcTrigger",   "MtdTrigger",    "BTowHit",
      "BTofHit",        "MtdHit",       "BbcHit",        "EpdHit",
      "FmsHit",         "EmcPidTraits", "BTofPidTraits", "MtdPidTraits",
      "TrackCovMatrix", "BEmcSmdEHit",  "BEmcSmdPHit"};

  for (auto &branch : branches)
    r.SetStatus(branch.c_str(), 0);
}

void TurnOffMostBranches(jetreader::Reader &r) {
  std::set<std::string> branches{
      "EmcTrigger",    "MtdTrigger",   "BTofHit",        "MtdHit",
      "BbcHit",        "EpdHit",       "FmsHit",         "EmcPidTraits",
      "BTofPidTraits", "MtdPidTraits", "TrackCovMatrix", "BEmcSmdEHit",
      "BEmcSmdPHit"};

  for (auto &branch : branches)
    r.SetStatus(branch.c_str(), 0);
}

} // namespace jetreader