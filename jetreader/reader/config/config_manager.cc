#include "jetreader/reader/config/config_manager.h"
#include "jetreader/lib/assert.h"
#include "jetreader/reader/config/event_selector_config_helper.h"
#include "jetreader/reader/config/reader_config_helper.h"
#include "jetreader/reader/config/tower_selector_config_helper.h"
#include "jetreader/reader/config/track_selector_config_helper.h"
#include "jetreader/reader/reader.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

namespace jetreader {

ConfigManager::ConfigManager(Reader *rdr) : reader_(rdr){};

void ConfigManager::loadConfig(const std::string &filename) {
  JETREADER_ASSERT(reader_ != nullptr,
                   "attempted to load a config, but reader is unspecified");

  // attempt to parse filename
  YAML::Node config = YAML::LoadFile(filename);

  for (auto &&entry : config) {
    std::string key = entry.first.as<std::string>();
    if (key == readerKey()) {
      loadReaderConfig(entry.second);
    } else if (key == towerSelectorKey()) {
      loadTowerSelectorConfig(entry.second);
    } else if (key == trackSelectorKey()) {
      loadTrackSelectorConfig(entry.second);
    } else if (key == eventSelectorKey()) {
      loadEventSelectorConfig(entry.second);
    }
  }
}

void ConfigManager::writeConfig(const std::string &filename) {
  JETREADER_ASSERT(reader_ != nullptr,
                   "attempted to write a config, but reader is unspecified");
  
  // read config from ConfigHelpers
  YAML::Node config;
  config[readerKey()] = readReaderConfig();
  config[towerSelectorKey()] = readTowerSelectorConfig();
  config[trackSelectorKey()] = readTrackSelectorConfig();
  config[eventSelectorKey()] = readEventSelectorConfig();

  // write to file
  std::ofstream out;
  out.open(filename);
  out << config;
  out.close();
}

void ConfigManager::loadReaderConfig(YAML::Node &node) {
  if (node.size() == 0)
    return;

  ReaderConfigHelper helper;
  helper.loadConfig(*reader_, node);
}

void ConfigManager::loadTowerSelectorConfig(YAML::Node &node) {
  if (node.size() == 0)
    return;
  if (reader_->towerSelector() == nullptr) {
    TowerSelector *sel = new TowerSelector();
    reader_->setTowerSelector(sel);
  }

  TowerSelectorConfigHelper helper;
  helper.loadConfig(*reader_->towerSelector(), node);
}

void ConfigManager::loadTrackSelectorConfig(YAML::Node &node) {
  if (node.size() == 0)
    return;
  if (reader_->trackSelector() == nullptr) {
    TrackSelector *sel = new TrackSelector();
    reader_->setTrackSelector(sel);
  }

  TrackSelectorConfigHelper helper;
  helper.loadConfig(*reader_->trackSelector(), node);
}

void ConfigManager::loadEventSelectorConfig(YAML::Node &node) {
  if (node.size() == 0)
    return;
  if (reader_->eventSelector() == nullptr) {
    EventSelector *sel = new EventSelector();
    reader_->setEventSelector(sel);
  }

  EventSelectorConfigHelper helper;
  helper.loadConfig(*reader_->eventSelector(), node);
}

YAML::Node ConfigManager::readReaderConfig() {
  ReaderConfigHelper helper;
  return helper.readConfig(*reader_);
}

YAML::Node ConfigManager::readTowerSelectorConfig() {
  TowerSelectorConfigHelper helper;
  return helper.readConfig(*reader_->towerSelector());
}

YAML::Node ConfigManager::readTrackSelectorConfig() {
  TrackSelectorConfigHelper helper;
  return helper.readConfig(*reader_->trackSelector());
}

YAML::Node ConfigManager::readEventSelectorConfig() {
  EventSelectorConfigHelper helper;
  return helper.readConfig(*reader_->eventSelector());
}

} // namespace jetreader