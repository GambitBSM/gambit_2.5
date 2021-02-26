//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  ColliderBit sources for model @MODEL@.
///
///  Automatically generated by GUM.
///
///  \author The GAMBIT Collaboration
///          http://gambit.hepforge.org
///  \date @DATETIME@
///
///  *********************************************

#include "gambit/ColliderBit/getPy8Collider.hpp"
#include "gambit/ColliderBit/generateEventPy8Collider.hpp"


namespace Gambit
{
  namespace ColliderBit
  {

    // Get spectrum and decays for Pythia
    GET_SPECTRUM_AND_DECAYS_FOR_PYTHIA_NONSUSY(getSpectrumAndDecaysForPythia_@MODEL@, @MODEL@_spectrum)

    // Get Monte Carlo event generator
    GET_SPECIFIC_PYTHIA(getPythia_@MODEL@, Pythia_@MODEL@_default, _@MODEL@)
    GET_PYTHIA_AS_BASE_COLLIDER(getPythia_@MODEL@AsBase)

    // Run event generator
    GET_PYTHIA_EVENT(generateEventPythia_@MODEL@)

  }
}
