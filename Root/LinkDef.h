#include <RootUtils/Constants.h>
#include <RootUtils/DryCanvas.h>
#include <RootUtils/RatioCanvas.h>
#include <RootUtils/DistributionCanvas.h>
#include <RootUtils/MCDataCanvas.h>
#include <RootUtils/SignalCanvas.h>
#include <RootUtils/DataBackgroundCanvas.h>
#include <RootUtils/EnvelopeCanvas.h>
#include <RootUtils/SystematicsCanvas.h>
#include <RootUtils/DryLegend.h>
#include <RootUtils/SimpleConfiguration.h>
#include <RootUtils/StandaloneJobInfoContainer.h>
#include <RootUtils/MessageService.h>
#include <RootUtils/RootColors.h>
#include <RootUtils/HistogramManager.h>
#include <RootUtils/LocalDatasetLookup.h>
#include <RootUtils/SeabornInterface.h>
#include <RootUtils/SystematicsContainer.h>
#include <RootUtils/SystematicsManager.h>
#include <RootUtils/CommonFunctions.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#endif

#ifdef __CINT__
//#pragma link C++ class std::pair<TH1D*, TH1D*>+;
//#pragma link C++ class std::pair<TString, TString>+;
//#pragma link C++ class std::pair<TString, TString>+;
#pragma link C++ class std::vector<std::vector<int> >+;
#pragma link C++ class std::vector<std::vector<double> >+;
#pragma link C++ class std::vector<std::pair<TString, TString> >+;
#pragma link C++ class std::pair<TString, std::vector<std::pair<TString, TString> > >+;
#pragma link C++ class Root::DryCanvas;
#pragma link C++ class Root::RatioCanvas;
#pragma link C++ class Root::DistributionCanvas;
#pragma link C++ class Root::MCDataCanvas;
#pragma link C++ class Root::SignalCanvas;
#pragma link C++ class Root::DataBackgroundCanvas;
#pragma link C++ class Root::EnvelopeCanvas;
#pragma link C++ class Root::SystematicsCanvas;
#pragma link C++ class Root::DryLegend;
#pragma link C++ class MessageService;
#pragma link C++ class SimpleConfiguration+;
#pragma link C++ class StandaloneJobInfoContainer+;
#pragma link C++ class Root::RootColors+;
#pragma link C++ class Root::SeabornInterface+;
//#pragma link C++ class Root::HistogramManager+;
#pragma link C++ class LocalDatasetLookup+;
#pragma link C++ class Root::SystematicsContainer;
#pragma link C++ class Root::SystematicsManager;
#pragma link C++ namespace Constants;
#pragma link C++ namespace Root;
//#pragma link C++ function Root::CombineHistogramBins;
#endif
