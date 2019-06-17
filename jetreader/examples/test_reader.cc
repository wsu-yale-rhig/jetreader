// test binary for development

#include "jetreader/lib/assert.h"
#include "jetreader/lib/test_data.h"
#include "jetreader/reader/reader.h"

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TMath.h"

#include "fastjet/PseudoJet.hh"

int main() {

  // creating reader with test StPicoDst
  std::string test_file = jetreader::GetTestFile();
  jetreader::Reader reader(test_file);

  // set some basic event cuts/track cuts/tower cuts using the Selectors
  reader.eventSelector()->setVzRange(-15, 15);
  reader.eventSelector()->setVrMax(0.5);
  reader.eventSelector()->setdVzMax(3.0);

  reader.trackSelector()->setDcaMax(3.0);
  reader.trackSelector()->setNHitsMin(15);
  reader.trackSelector()->setNHitsFracMin(0.52);
  reader.trackSelector()->setPtMax(30.0);

  reader.towerSelector()->setEtMax(30.0);

  // turn off branches that don't exist in this tree (not necessary) and
  // initialize the reader
  reader.SetStatus("BEmcSmdEHit", false);
  reader.SetStatus("BEmcSmdPHit", false);
  reader.Init();
  reader.writeConfig("example.yaml");

  std::cout << "number of events in chain: " << reader.tree()->GetEntries()
            << std::endl;

  // create an output file for results
  TFile out("out.root", "RECREATE");

  // example histograms

  // event-level
  TH1D *vz = new TH1D("vz", ";v_{z} [cm]", 30, -30, 30);
  TH1D *vr = new TH1D("vr", ";v_{R} [cm]", 50, 0, 0.5);
  TH1D *dvz = new TH1D("dvz", ";dv_{z} [cm]", 50, -5, 5);

  // direct track access histograms
  TH1D *dcav1 = new TH1D("dcav1", ";DCA [cm]", 100, 0, 3.0);
  TH1D *nhitv1 = new TH1D("nhitv1", ";N_{hit}", 50, 0, 50);
  TH1D *nhitpossv1 = new TH1D("nhitpossv1", ";N_{hit} possible", 50, 0, 50);
  TH1D *ptv1 = new TH1D("ptv1", ";p_{T} [GeV/c]", 100, 0, 15);
  TH1D *etav1 = new TH1D("etav1", ";#eta", 100, -1, 1);
  TH1D *phiv1 = new TH1D("phiv1", ";#phi", 100, -TMath::Pi(), TMath::Pi());

  // direct tower access histograms
  TH1D *tower_ev1 = new TH1D("ev1", "", 100, 0, 15);

  // track/tower histograms via processed pseudojets
  TH1D *dcav2 = new TH1D("dcav2", ";DCA [cm]", 100, 0, 3.0);
  TH1D *nhitv2 = new TH1D("nhitv2", ";N_{hit}", 50, 0, 50);
  TH1D *nhitpossv2 = new TH1D("nhitpossv2", ";N_{hit} possible", 50, 0, 50);
  TH1D *ptv2 = new TH1D("ptv2", ";p_{T} [GeV/c]", 100, 0, 15);
  TH1D *etav2 = new TH1D("etav2", ";#eta", 100, -1, 1);
  TH1D *phiv2 = new TH1D("phiv2", ";#phi", 100, -TMath::Pi(), TMath::Pi());

  TH1D *tower_ev2 = new TH1D("ev2", "", 100, 0, 15);
  TH1D *tower_etv2 = new TH1D("etv2", "", 100, 0, 15);
  TH1D *tower_etarawv2 = new TH1D("towetarawv2", "", 100, -1, 1);
  TH1D *tower_etav2 = new TH1D("towetav2", "", 100, -1, 1);
  TH1D *tower_phiv2 = new TH1D("towphiv2", "", 100, TMath::Pi(), TMath::Pi());

  int event = 0;
  while (reader.next()) {
    if (event % 100 == 0)
      std::cout << "event: " << event << std::endl;
    event++;
    // fill header information
    vz->Fill(reader.picoDst()->event()->primaryVertex().Z());

    double vx = reader.picoDst()->event()->primaryVertex().X();
    double vy = reader.picoDst()->event()->primaryVertex().Y();
    double vr_val = sqrt(vx * vx + vy * vy);
    vr->Fill(vr_val);

    double dvz_val = reader.picoDst()->event()->primaryVertex().Z() -
                     reader.picoDst()->event()->vzVpd();
    dvz->Fill(dvz_val);

    // process tracks by hand through the StPicoEvent (no cuts)
    for (int i = 0; i < reader.picoDst()->numberOfTracks(); ++i) {
      StPicoTrack *track = reader.picoDst()->track(i);
      dcav1->Fill(
          track->gDCA(reader.picoDst()->event()->primaryVertex()).Mag());
      nhitv1->Fill(track->nHitsFit());
      nhitpossv1->Fill(track->nHitsPoss());
      ptv1->Fill(track->pPt());
      etav1->Fill(track->pMom().Eta());
      phiv1->Fill(track->pMom().Phi());
    }

    // process towers by hand through the StPicoEvent (no cuts)
    for (int i = 0; i < reader.picoDst()->numberOfBTowHits(); ++i) {
      StPicoBTowHit *hit = reader.picoDst()->btowHit(i);
      tower_ev1->Fill(hit->energy());
    }

    // process tracks and towers now through the selected pseudojets
    std::vector<fastjet::PseudoJet> container = reader.pseudojets();

    for (auto &track : container) {
      jetreader::VectorInfo track_info =
          track.user_info<jetreader::VectorInfo>();
      if (track_info.isPrimary()) {
        dcav2->Fill(track_info.dca());
        nhitv2->Fill(track_info.nhits());
        nhitpossv2->Fill(track_info.nhitsPoss());
        ptv2->Fill(track.pt());
        etav2->Fill(track.eta());
        phiv2->Fill(track.phi());
      } else if (track_info.isBemcTower()) {
        tower_ev2->Fill(track.E());
        tower_etv2->Fill(track.pt());
        tower_etav2->Fill(track.eta());
        tower_phiv2->Fill(track.phi_std());
        tower_etarawv2->Fill(track_info.towerRawEta());
      } else if (track_info.isGlobal()) {
        std::cout << "global track?" << std::endl;
      } else {
        JETREADER_THROW("Should not be here; vector labeled as not primary, "
                        "not global, and not bemc tower");
      }
    }
  }

  std::cout << event << " good events out of " << reader.tree()->GetReadEntry()
            << "\n";
  std::cout << "accepted : " << (double)event / reader.tree()->GetReadEntry()
            << "% of events\n";

  out.Write();
  out.Close();

  return 0;
}