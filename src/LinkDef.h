#include "MyTools/RootUtils/interface/Constants.h"
#include "MyTools/RootUtils/interface/DryCanvas.h"
#include "MyTools/RootUtils/interface/RatioCanvas.h"
#include "MyTools/RootUtils/interface/DistributionCanvas.h"
#include "MyTools/RootUtils/interface/MCDataCanvas.h"
#include "MyTools/RootUtils/interface/SignalCanvas.h"
#include "MyTools/RootUtils/interface/DataBackgroundCanvas.h"
#include "MyTools/RootUtils/interface/EnvelopeCanvas.h"
#include "MyTools/RootUtils/interface/SystematicsCanvas.h"
#include "MyTools/RootUtils/interface/DryLegend.h"
#include "MyTools/RootUtils/interface/SimpleConfiguration.h"
#include "MyTools/RootUtils/interface/StandaloneJobInfoContainer.h"
#include "MyTools/RootUtils/interface/MessageService.h"
#include "MyTools/RootUtils/interface/RootColors.h"
#include "MyTools/RootUtils/interface/HistogramManager.h"
#include "MyTools/RootUtils/interface/LocalDatasetLookup.h"
#include "MyTools/RootUtils/interface/SeabornInterface.h"
#include "MyTools/RootUtils/interface/SystematicsContainer.h"
#include "MyTools/RootUtils/interface/SystematicsManager.h"
#include "MyTools/RootUtils/interface/CommonFunctions.h"
#include "MyTools/RootUtils/interface/CanvasHelpers.h"

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
#pragma link C++ class Root::HistogramManager+;
#pragma link C++ class LocalDatasetLookup+;
#pragma link C++ class Root::SystematicsContainer;
#pragma link C++ class Root::SystematicsManager;
#pragma link C++ namespace Constants;
#pragma link C++ namespace Root;
#pragma link C++ function Root::CMSLabel;
#pragma link C++ function Root::myText;
#pragma link C++ function Root::myBoxText;
#pragma link C++ function Root::myMarkerText;
#pragma link C++ function Root::SetCanvasStyle;
//#pragma link C++ function Root::CombineHistogramBins;
#endif
