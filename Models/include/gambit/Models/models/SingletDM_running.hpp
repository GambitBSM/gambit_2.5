//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
//
//  Scalar singlet dark matter with running mass
//  and quartic coupling
//  *********************************************
//
//  Authors
//  =======
//
//  (add name and date if you modify)
//  Christoph Weniger
//  2014 January
//
//  James McKay
//  2015 September
//
//  *********************************************

#ifndef __SingletDM_running_hpp__
#define __SingletDM_running_hpp__

#include "gambit/Models/models/SSDM.hpp"


#define MODEL SingletDM_running
#define PARENT SSDM
  START_MODEL
  INTERPRET_AS_PARENT__FUNCTION(SingletDM_running_to_SSDM)
  DEFINEPARS(mS2, lambda_hS)
#undef PARENT

#undef MODEL

#endif
