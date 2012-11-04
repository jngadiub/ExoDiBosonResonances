#include "ExoDiBosonResonances/EDBRCommon/interface/OverlapTest.h"

#include <algorithm>
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"

using namespace cmg::helper;

void
OverlapTest::readInput(const edm::Event & iEvent, const edm::EventSetup &iSetup)
{
    iEvent.getByLabel(src_, candidates_);
    isPreselected_.resize(candidates_->size());
    size_t idx = 0;
    for (edm::View<reco::Candidate>::const_iterator it = candidates_->begin(); it != candidates_->end(); ++it, ++idx) {
        isPreselected_[idx] = presel_(*it);
    }
    // Yes, I could use std::transform. But would people like it?
    // http://www.sgi.com/tech/stl/transform.html
}

bool
OverlapTest::fillOverlapsForItem(const reco::Candidate &item) const
{
    size_t idx = 0;
    int matches=0;
    for (edm::View<reco::Candidate>::const_iterator it = candidates_->begin(); it != candidates_->end(); ++it, ++idx) {
        if (!isPreselected_[idx]) continue;
        double dr = reco::deltaR(item, *it);
        if (dr < deltaR_) {
	  if (checkRecoComponents_) {
	    OverlapChecker overlaps;
	    if (!overlaps(item, *it)) continue;
	  }
	  if (!pairCut_(pat::DiObjectProxy(item,*it))) continue;
  	  matches++;
        }
    }
    // see if we matched anything
    if (matches==0) return false;

    return true;
}
