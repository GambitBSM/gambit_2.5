//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
//
//  Singlet DM
//
//  *********************************************
//
//  Authors
//  =======
//
//  (add name and date if you modify)
//
//  Marcin Chrzaszcz & Martin White
//  2016 October
//  Jihyun Bhom
//  2020 January
//
//  *********************************************

#ifndef __WC_hpp__   
#define __WC_hpp__   


#define MODEL WC
   START_MODEL
   DEFINEPARS(Re_DeltaC7, Im_DeltaC7, Re_DeltaC9, Im_DeltaC9, Re_DeltaC10, Im_DeltaC10, Re_DeltaCQ1, Im_DeltaCQ1, Re_DeltaCQ2, Im_DeltaCQ2)
#undef MODEL  

#define MODEL WC_LUV
   START_MODEL
     DEFINEPARS(Re_DeltaC7_tau, Im_DeltaC7_tau, Re_DeltaC9_tau, Im_DeltaC9_tau, Re_DeltaC10_tau, Im_DeltaC10_tau, Re_DeltaCQ1_tau, Im_DeltaCQ1_tau, Re_DeltaCQ2_tau, Im_DeltaCQ2_tau,
                Re_DeltaC7_mu, Im_DeltaC7_mu, Re_DeltaC9_mu, Im_DeltaC9_mu, Re_DeltaC10_mu, Im_DeltaC10_mu, Re_DeltaCQ1_mu, Im_DeltaCQ1_mu, Re_DeltaCQ2_mu, Im_DeltaCQ2_mu,
                Re_DeltaC7_e, Im_DeltaC7_e, Re_DeltaC9_e, Im_DeltaC9_e, Re_DeltaC10_e, Im_DeltaC10_e, Re_DeltaCQ1_e, Im_DeltaCQ1_e, Re_DeltaCQ2_e, Im_DeltaCQ2_e)
#undef MODEL
                                                                                                                                                         

     
#endif
