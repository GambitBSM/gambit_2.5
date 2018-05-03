//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Rollcall header for the developement
///  version of the CosmoBit module.
///
///  Compile-time registration of available
///  functions and variables from this backend.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Selim C. Hotinli
///          (selim.hotinli14@imperial.ac.uk)
///  \date 2017 Jul
///  \date 2017 Nov
///
///  \author Patrick Stoecker
///          (stoecker@physik.rwth-aachen.de)
///  \date 2017 Nov
///  \date 2018 Jan,Feb, Mar
///
///  *********************************************

#ifndef __CosmoBit_rollcall_hpp__
#define __CosmoBit_rollcall_hpp__

#include "gambit/CosmoBit/CosmoBit_types.hpp"

#define MODULE CosmoBit
START_MODULE

  #define CAPABILITY injection_spectrum
  START_CAPABILITY
    #define FUNCTION injection_spectrum_ToyModel
    START_FUNCTION(DarkAges::injectionSpectrum)
    ALLOW_MODELS(TestDecayingDM)
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY DM_mass
  START_CAPABILITY
    #define FUNCTION DM_mass_ToyModel
    START_FUNCTION(double)
    ALLOW_MODELS(TestDecayingDM)    
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY DM_fraction
  START_CAPABILITY
    #define FUNCTION DM_fraction_ToyModel
    START_FUNCTION(double)
    ALLOW_MODELS(TestDecayingDM)    
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY lifetime
  START_CAPABILITY
    #define FUNCTION lifetime_ToyModel
    START_FUNCTION(double)
    ALLOW_MODELS(TestDecayingDM)    
    #undef FUNCTION
  #undef CAPABILITY
  
  #define CAPABILITY f_effective
  START_CAPABILITY
    #define FUNCTION f_effective_func
    START_FUNCTION(double)
    ALLOW_MODELS(TestDecayingDM)    
    BACKEND_REQ(DA_efficiency_function, (DarkAges_tag),DarkAges::fz_table,())
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY PyTest_cap_1
  START_CAPABILITY
    #define FUNCTION PyTest_func_1
    START_FUNCTION(double)
    BACKEND_REQ(PyArrayTest_Py_to_cpp, (PyArrayTest_tag), std::vector<double>, ())
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY PyTest_cap_2
  START_CAPABILITY
    #define FUNCTION PyTest_func_2
    START_FUNCTION(double)
    BACKEND_REQ(PyArrayTest_cpp_to_Py, (PyArrayTest_tag), double, (int))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY lnL_A_planck
  START_CAPABILITY
    #define FUNCTION lnL_A_planck_gaussian
    START_FUNCTION(double)
    ALLOW_MODELS(Planck_TTTEEE,Planck_TT,Planck_lite)
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_vanilla_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_vanilla_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(LCDM,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//   BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_LCDMtensor_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_LCDMtensor_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(LCDMtensor,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_LCDMtensor_inflation_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_LCDMtensor_inflation_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(LCDMtensor,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    BACKEND_REQ(multimodecode_gambit_driver_test,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,  int& ,  int& ,double& ,double& ,double& ,  int& ,int& ,double& ,double& ,double&))
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//    BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_1quadInf_rLCDMtensor_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_1quadInf_rLCDMtensor_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(inf_SR1quad_LCDMt,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    BACKEND_REQ(multimodecode_gambit_driver_test,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,  int& ,  int& ,double& ,double& ,double& ,  int& ,int& ,double& ,double& ,double&))
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//    BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_1quarInf_rLCDMtensor_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_1quarInf_rLCDMtensor_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(inf_1quarInf_LCDMt,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    BACKEND_REQ(multimodecode_gambit_driver_test,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,  int& ,  int& ,double& ,double& ,double& ,  int& ,int& ,double& ,double& ,double&))
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//    BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_1mono32Inf_rLCDMtensor_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_1mono32Inf_rLCDMtensor_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(inf_1mono32Inf_LCDMt,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    BACKEND_REQ(multimodecode_gambit_driver_test,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,  int& ,  int& ,double& ,double& ,double& ,  int& ,int& ,double& ,double& ,double&))
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//    BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_1linearInf_rLCDMtensor_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_1linearInf_rLCDMtensor_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(inf_1linearInf_LCDMt,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    BACKEND_REQ(multimodecode_gambit_driver_test,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,  int& ,  int& ,double& ,double& ,double& ,  int& ,int& ,double& ,double& ,double&))
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//    BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_smashInf_rLCDMtensor_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_smashInf_rLCDMtensor_lowp_TT_loglike
    START_FUNCTION(double)
    ALLOW_MODELS(inf_smashInf_LCDMt,plik_dx11dr2_HM_v18_TT)
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    BACKEND_REQ(multimodecode_gambit_driver_test,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,  int& ,  int& ,double& ,double& ,double& ,  int& ,int& ,double& ,double& ,double&))
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//    BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    BACKEND_REQ(class_output_initialize,(class_tag),int,(Class::background*,Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*))
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY class_set_parameter
  START_CAPABILITY
    #define FUNCTION class_set_parameter_LCDM
    START_FUNCTION(CosmoBit::Class_container)
    ALLOW_MODELS(LCDM)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    #undef FUNCTION

    #define FUNCTION class_set_parameter_LCDM_SingletDM
    START_FUNCTION(CosmoBit::Class_container)
    DEPENDENCY(mwimp,double)
    DEPENDENCY(sigmav,double)
    ALLOW_MODEL_DEPENDENCE(LCDM,SingletDM)
    MODEL_GROUP(cosmo,(LCDM))
    MODEL_GROUP(dark,(SingletDM))
    ALLOW_MODEL_COMBINATION(cosmo,dark)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    #undef FUNCTION

    #define FUNCTION class_set_parameter_LCDMtensor
    START_FUNCTION(CosmoBit::Class_container)
    ALLOW_MODELS(LCDMtensor)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    #undef FUNCTION

    #define FUNCTION class_set_parameter_inf_SR1quad_LCDMt
    START_FUNCTION(CosmoBit::Class_container)
    ALLOW_MODELS(inf_SR1quad_LCDMt)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    #undef FUNCTION

    #define FUNCTION class_set_parameter_inf_1quarInf_LCDMt
    START_FUNCTION(CosmoBit::Class_container)
    ALLOW_MODELS(inf_1quarInf_LCDMt)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    #undef FUNCTION

    #define FUNCTION class_set_parameter_inf_1mono32Inf_LCDMt
    START_FUNCTION(CosmoBit::Class_container)
    ALLOW_MODELS(inf_1mono32Inf_LCDMt)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    #undef FUNCTION

    #define FUNCTION class_set_parameter_inf_1linearInf_LCDMt
    START_FUNCTION(CosmoBit::Class_container)
    ALLOW_MODELS(inf_1linearInf_LCDMt)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    #undef FUNCTION

    #define FUNCTION class_set_parameter_inf_smashInf_LCDMt
    START_FUNCTION(CosmoBit::Class_container)
    ALLOW_MODELS(inf_smashInf_LCDMt)
    BACKEND_REQ(class_parser_initialize,(class_tag),int,(Class::file_content*,int ,char*, char*))
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY class_run
  START_CAPABILITY
    #define FUNCTION class_run_func
    START_FUNCTION(CosmoBit::Class_container)
    DEPENDENCY(class_set_parameter,CosmoBit::Class_container)
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY class_run_full_pk_ugly
  START_CAPABILITY
    #define FUNCTION class_run_func_full_pk_ugly
    START_FUNCTION(CosmoBit::Class_container)
    DEPENDENCY(class_set_parameter,CosmoBit::Class_container)
    BACKEND_REQ(multimodecode_gambit_driver,(modecode_tag), void, (gambit_inflation_observables*,int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int& ,double& ,double*,double*,double*,double*,double& ,double&))
    BACKEND_REQ(class_input_initialize,(class_tag),int,(Class::file_content*, Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::transfers*, Class::primordial*, Class::spectra*, Class::nonlinear*, Class::lensing*, Class::output*, char*))
    BACKEND_REQ(class_background_initialize,(class_tag),int,(Class::precision*, Class::background*))
    BACKEND_REQ(class_thermodynamics_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*))
    BACKEND_REQ(class_perturb_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*))
    BACKEND_REQ(class_primordial_initialize,(class_tag),int,(Class::precision*, Class::perturbs*, Class::primordial*))
    BACKEND_REQ(class_nonlinear_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::primordial*, Class::nonlinear*))
    BACKEND_REQ(class_transfer_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::thermo*, Class::perturbs*, Class::nonlinear*, Class::transfers*))
    BACKEND_REQ(class_spectra_initialize,(class_tag),int,(Class::precision*, Class::background*, Class::perturbs*, Class::primordial*, Class::nonlinear*, Class::transfers*, Class::spectra*))
    BACKEND_REQ(class_lensing_initialize,(class_tag),int,(Class::precision*, Class::perturbs*,Class::spectra*, Class::nonlinear*,Class::lensing*))
    #undef FUNCTION
  #undef CAPABILITY


  #define CAPABILITY class_get_spectra
  START_CAPABILITY
    #define FUNCTION class_get_spectra_func
    START_FUNCTION(CosmoBit::Class_container)
    DEPENDENCY(class_run,CosmoBit::Class_container)
    BACKEND_REQ(class_output_total_cl_at_l,(class_tag),int, (Class::spectra*, Class::lensing* , Class::output*, int, double* ))
    BACKEND_REQ(class_lensing_free,(class_tag),int, (Class::lensing*))
    BACKEND_REQ(class_spectra_free,(class_tag),int, (Class::spectra*))
    BACKEND_REQ(class_transfer_free,(class_tag),int, (Class::transfers*))
    BACKEND_REQ(class_nonlinear_free,(class_tag),int, (Class::nonlinear*))
    BACKEND_REQ(class_primordial_free,(class_tag),int, (Class::primordial*))
    BACKEND_REQ(class_perturb_free,(class_tag),int, (Class::perturbs*))
    BACKEND_REQ(class_thermodynamics_free,(class_tag),int, (Class::thermo*))
    BACKEND_REQ(class_background_free,(class_tag),int, (Class::background*))
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_Planck_lowp_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_Planck_lowp_TT_loglike
    START_FUNCTION(double)
    DEPENDENCY(class_get_spectra,CosmoBit::Class_container)
    ALLOW_MODELS(Planck_TTTEEE,Planck_TT,Planck_lite)
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//    BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_lowp_TT,(clik_tag),clik_object*,())
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_Planck_high_TT_loglike
  START_CAPABILITY
    #define FUNCTION function_Planck_high_TT_loglike
    START_FUNCTION(double)
    DEPENDENCY(class_get_spectra,CosmoBit::Class_container)
    ALLOW_MODELS(Planck_TT)
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//   BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT,(clik_tag),clik_object*,())
    #undef FUNCTION

    #define FUNCTION function_Planck_high_TTTEEE_loglike
    START_FUNCTION(double)
    DEPENDENCY(class_get_spectra,CosmoBit::Class_container)
    ALLOW_MODELS(Planck_TTTEEE)
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//   BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TTTEEE,(clik_tag),clik_object*,())
    #undef FUNCTION

    #define FUNCTION function_Planck_high_TT_lite_loglike
    START_FUNCTION(double)
    DEPENDENCY(class_get_spectra,CosmoBit::Class_container)
    ALLOW_MODELS(Planck_lite)
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//   BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TT_lite,(clik_tag),clik_object*,())
    #undef FUNCTION

    #define FUNCTION function_Planck_high_TTTEEE_lite_loglike
    START_FUNCTION(double)
    DEPENDENCY(class_get_spectra,CosmoBit::Class_container)
    ALLOW_MODELS(Planck_lite)
    BACKEND_REQ(clik_compute_loglike, (clik_tag), double ,(clik_object*,double*,clik_error**))
    BACKEND_REQ(clik_initialize, (clik_tag), clik_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(clik_get_list_cls, (clik_tag), void , (clik_object*,int,clik_error**))
    BACKEND_REQ(clik_get_extra_parameter_names, (clik_tag), int ,(clik_object*,parname**,clik_error**))
    BACKEND_REQ(clik_get_lmax, (clik_tag), void ,(clik_object*,int,clik_error**))
    BACKEND_REQ(clik_cleanup, (clik_tag), void ,(clik_object**))
    BACKEND_REQ(clik_get_version, (clik_tag), char* , (clik_object*,clik_error**))
//   BACKEND_REQ(data_initialize, (clik_tag), void, ())
    BACKEND_REQ(return_high_TTTEEE_lite,(clik_tag),clik_object*,())
    #undef FUNCTION
  #undef CAPABILITY

  #define CAPABILITY compute_Planck_lensing_loglike
  START_CAPABILITY
    #define FUNCTION function_Planck_lensing_loglike
    START_FUNCTION(double)
    DEPENDENCY(class_get_spectra,CosmoBit::Class_container)
    ALLOW_MODELS(Planck_TTTEEE,Planck_TT,Planck_lite)
    BACKEND_REQ(clik_lensing_compute_loglike, (clik_tag), double ,(clik_lensing_object*,double*,clik_error**))
    BACKEND_REQ(clik_lensing_initialize, (clik_tag), clik_lensing_object* ,(char*,clik_error**))
    BACKEND_REQ(clik_initialize_error, (clik_tag), clik_error* ,())
    BACKEND_REQ(return_lensing,(clik_tag),clik_lensing_object*,())
    #undef FUNCTION
  #undef CAPABILITY

#undef MODULE

#endif /* defined __CosmoBit_rollcall_hpp__ */
