#ifndef analysis_user_h
#define analysis_user_h

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class user : public AnalyzerBase
{
  INIT_ANALYSIS(user,"MadAnalysis5job")

 public : 
  virtual bool Initialize(const MA5::Configuration& cfg,
                          const std::map<std::string,std::string>& parameters);
  virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
  virtual bool Execute(SampleFormat& sample, const EventFormat& event);

 private : 
  // Declaring histogram array
  PlotManager plots_;

  // Declaring cut array
  CounterManager cuts_;

  // Declaring shortcut to histograms
  Histo* H0_;
   std::vector<const MCParticleFormat*> _P_aPTorderingfinalstate;
   bool isP__aPTorderingfinalstate(const MCParticleFormat* part) const {
     if ( part==0 ) return false;
     if ( !PHYSICS->Id->IsFinalState(part) ) return false;
     if ( (part->pdgid()!=22) ) return false;
     return true; }
   std::vector<const MCParticleFormat*> _P_e_mPTorderingfinalstate;
   bool isP__e_mPTorderingfinalstate(const MCParticleFormat* part) const {
     if ( part==0 ) return false;
     if ( !PHYSICS->Id->IsFinalState(part) ) return false;
     if ( (part->pdgid()!=11) ) return false;
     return true; }
};
}

#endif