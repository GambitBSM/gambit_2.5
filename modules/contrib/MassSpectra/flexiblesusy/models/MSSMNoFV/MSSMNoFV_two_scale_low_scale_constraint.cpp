// ====================================================================
// This file is part of FlexibleSUSY.
//
// FlexibleSUSY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// FlexibleSUSY is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlexibleSUSY.  If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

// File generated at Fri 16 Jan 2015 12:51:41

#include "MSSMNoFV_two_scale_low_scale_constraint.hpp"
#include "MSSMNoFV_two_scale_model.hpp"
#include "wrappers.hpp"
#include "logger.hpp"
#include "ew_input.hpp"
#include "gsl_utils.hpp"
#include "minimizer.hpp"
#include "root_finder.hpp"

#include <cassert>
#include <cmath>
#include <limits>

namespace flexiblesusy {

#define INPUTPARAMETER(p) inputPars.p
#define MODELPARAMETER(p) model->get_##p()
#define BETAPARAMETER(p) beta_functions.get_##p()
#define BETA(p) beta_##p
#define SM(p) Electroweak_constants::p
#define STANDARDDEVIATION(p) Electroweak_constants::Error_##p
#define Pole(p) model->get_physical().p
#define MODEL model
#define MODELCLASSNAME MSSMNoFV<Two_scale>

MSSMNoFV_low_scale_constraint<Two_scale>::MSSMNoFV_low_scale_constraint()
   : Constraint<Two_scale>()
   , scale(0.)
   , initial_scale_guess(0.)
   , model(0)
   , inputPars()
   , oneset()
   , MZDRbar(0.)
   , new_g1(0.)
   , new_g2(0.)
   , new_g3(0.)
   , threshold_corrections_loop_order(1)
{
}

MSSMNoFV_low_scale_constraint<Two_scale>::MSSMNoFV_low_scale_constraint(
   MSSMNoFV<Two_scale>* model_,
   const MSSMNoFV_input_parameters& inputPars_, const QedQcd& oneset_)
   : Constraint<Two_scale>()
   , model(model_)
   , inputPars(inputPars_)
   , oneset(oneset_)
   , new_g1(0.)
   , new_g2(0.)
   , new_g3(0.)
{
   initialize();
}

MSSMNoFV_low_scale_constraint<Two_scale>::~MSSMNoFV_low_scale_constraint()
{
}

void MSSMNoFV_low_scale_constraint<Two_scale>::apply()
{
   assert(model && "Error: MSSMNoFV_low_scale_constraint::apply():"
          " model pointer must not be zero");

   model->calculate_DRbar_masses();
   update_scale();
   calculate_DRbar_gauge_couplings();

   const auto TanBeta = INPUTPARAMETER(TanBeta);
   const auto g1 = MODELPARAMETER(g1);
   const auto g2 = MODELPARAMETER(g2);

   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();
   MODEL->set_vd((2*MZDRbar)/(Sqrt(0.6*Sqr(g1) + Sqr(g2))*Sqrt(1 + Sqr(TanBeta)
      )));
   MODEL->set_vu((2*MZDRbar*TanBeta)/(Sqrt(0.6*Sqr(g1) + Sqr(g2))*Sqrt(1 + Sqr(
      TanBeta))));


   model->set_g1(new_g1);
   model->set_g2(new_g2);
   model->set_g3(new_g3);
}

double MSSMNoFV_low_scale_constraint<Two_scale>::get_scale() const
{
   return scale;
}

double MSSMNoFV_low_scale_constraint<Two_scale>::get_initial_scale_guess() const
{
   return initial_scale_guess;
}

void MSSMNoFV_low_scale_constraint<Two_scale>::set_model(Two_scale_model* model_)
{
   model = cast_model<MSSMNoFV<Two_scale>*>(model_);
}

void MSSMNoFV_low_scale_constraint<Two_scale>::set_input_parameters(const MSSMNoFV_input_parameters& inputPars_)
{
   inputPars = inputPars_;
}

void MSSMNoFV_low_scale_constraint<Two_scale>::set_sm_parameters(const QedQcd& oneset_)
{
   oneset = oneset_;
}

void MSSMNoFV_low_scale_constraint<Two_scale>::clear()
{
   scale = 0.;
   initial_scale_guess = 0.;
   model = NULL;
   oneset = QedQcd();
   MZDRbar = 0.;
   new_g1 = 0.;
   new_g2 = 0.;
   new_g3 = 0.;
}

void MSSMNoFV_low_scale_constraint<Two_scale>::initialize()
{
   assert(model && "MSSMNoFV_low_scale_constraint<Two_scale>::"
          "initialize(): model pointer is zero.");

   initial_scale_guess = SM(MZ);

   scale = initial_scale_guess;

   MZDRbar = 0.;
   new_g1 = 0.;
   new_g2 = 0.;
   new_g3 = 0.;
}

void MSSMNoFV_low_scale_constraint<Two_scale>::update_scale()
{
   assert(model && "MSSMNoFV_low_scale_constraint<Two_scale>::"
          "update_scale(): model pointer is zero.");

   scale = SM(MZ);


}

void MSSMNoFV_low_scale_constraint<Two_scale>::calculate_DRbar_gauge_couplings()
{
   assert(oneset.displayMu() == get_scale() && "Error: low-energy data"
          " set is not defined at the same scale as the low-energy"
          " constraint.  You need to run the low-energy data set to this"
          " scale!");

   const double alpha_em = oneset.displayAlpha(ALPHA);
   const double alpha_s  = oneset.displayAlpha(ALPHAS);

   double delta_alpha_em = 0.;
   double delta_alpha_s  = 0.;

   if (model->get_thresholds()) {
      delta_alpha_em = calculate_delta_alpha_em(alpha_em);
      delta_alpha_s  = calculate_delta_alpha_s(alpha_s);
   }

   const double alpha_em_drbar = alpha_em / (1.0 - delta_alpha_em);
   const double alpha_s_drbar  = alpha_s  / (1.0 - delta_alpha_s);
   const double e_drbar        = Sqrt(4.0 * Pi * alpha_em_drbar);

   // interface variables
   MZDRbar = oneset.displayPoleMZ();
   double MWDRbar = oneset.displayPoleMW();

   if (model->get_thresholds()) {
      MZDRbar = model->calculate_MVZ_DRbar(oneset.displayPoleMZ());
      MWDRbar = model->calculate_MVWm_DRbar(oneset.displayPoleMW());
   }

   const double AlphaS = alpha_s_drbar;
   const double EDRbar = e_drbar;

   const double ThetaW = ArcSin(Sqrt(1 - Sqr(MWDRbar)/Sqr(MZDRbar)));
   new_g1 = 1.2909944487358056*EDRbar*Sec(ThetaW);
   new_g2 = EDRbar*Csc(ThetaW);
   new_g3 = 3.5449077018110318*Sqrt(AlphaS);

}

double MSSMNoFV_low_scale_constraint<Two_scale>::calculate_delta_alpha_em(double alphaEm) const
{
   const double currentScale = model->get_scale();
   const auto MCha = MODELPARAMETER(MCha);
   const auto MHpm = MODELPARAMETER(MHpm);
   const auto MSb = MODELPARAMETER(MSb);
   const auto MSc = MODELPARAMETER(MSc);
   const auto MSd = MODELPARAMETER(MSd);
   const auto MSe = MODELPARAMETER(MSe);
   const auto MSm = MODELPARAMETER(MSm);
   const auto MSs = MODELPARAMETER(MSs);
   const auto MSt = MODELPARAMETER(MSt);
   const auto MStau = MODELPARAMETER(MStau);
   const auto MSu = MODELPARAMETER(MSu);
   const auto MFt = MODELPARAMETER(MFt);

   const double delta_alpha_em_SM = 0.15915494309189535*alphaEm*(
      0.3333333333333333 - 1.7777777777777777*FiniteLog(Abs(MFt/currentScale)));

   const double delta_alpha_em = 0.15915494309189535*alphaEm*(
      -1.3333333333333333*FiniteLog(Abs(MCha(0)/currentScale)) -
      1.3333333333333333*FiniteLog(Abs(MCha(1)/currentScale)) - 0.3333333333333333
      *FiniteLog(Abs(MHpm(1)/currentScale)) - 0.1111111111111111*FiniteLog(Abs(MSb
      (0)/currentScale)) - 0.1111111111111111*FiniteLog(Abs(MSb(1)/currentScale))
      - 0.4444444444444444*FiniteLog(Abs(MSc(0)/currentScale)) -
      0.4444444444444444*FiniteLog(Abs(MSc(1)/currentScale)) - 0.1111111111111111*
      FiniteLog(Abs(MSd(0)/currentScale)) - 0.1111111111111111*FiniteLog(Abs(MSd(1
      )/currentScale)) - 0.3333333333333333*FiniteLog(Abs(MSe(0)/currentScale)) -
      0.3333333333333333*FiniteLog(Abs(MSe(1)/currentScale)) - 0.3333333333333333*
      FiniteLog(Abs(MSm(0)/currentScale)) - 0.3333333333333333*FiniteLog(Abs(MSm(1
      )/currentScale)) - 0.1111111111111111*FiniteLog(Abs(MSs(0)/currentScale)) -
      0.1111111111111111*FiniteLog(Abs(MSs(1)/currentScale)) - 0.4444444444444444*
      FiniteLog(Abs(MSt(0)/currentScale)) - 0.4444444444444444*FiniteLog(Abs(MSt(1
      )/currentScale)) - 0.3333333333333333*FiniteLog(Abs(MStau(0)/currentScale))
      - 0.3333333333333333*FiniteLog(Abs(MStau(1)/currentScale)) -
      0.4444444444444444*FiniteLog(Abs(MSu(0)/currentScale)) - 0.4444444444444444*
      FiniteLog(Abs(MSu(1)/currentScale)));

   return delta_alpha_em + delta_alpha_em_SM;

}

double MSSMNoFV_low_scale_constraint<Two_scale>::calculate_delta_alpha_s(double alphaS) const
{
   const double currentScale = model->get_scale();
   const auto MSb = MODELPARAMETER(MSb);
   const auto MSc = MODELPARAMETER(MSc);
   const auto MSd = MODELPARAMETER(MSd);
   const auto MSs = MODELPARAMETER(MSs);
   const auto MSt = MODELPARAMETER(MSt);
   const auto MSu = MODELPARAMETER(MSu);
   const auto MFt = MODELPARAMETER(MFt);
   const auto MGlu = MODELPARAMETER(MGlu);

   const double delta_alpha_s_SM = -0.1061032953945969*alphaS*FiniteLog(Abs(
      MFt/currentScale));

   const double delta_alpha_s = 0.15915494309189535*alphaS*(0.5 - 2*FiniteLog(
      Abs(MGlu/currentScale)) - 0.16666666666666666*FiniteLog(Abs(MSb(0)
      /currentScale)) - 0.16666666666666666*FiniteLog(Abs(MSb(1)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSc(0)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSc(1)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSd(0)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSd(1)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSs(0)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSs(1)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSt(0)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSt(1)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSu(0)/currentScale)) -
      0.16666666666666666*FiniteLog(Abs(MSu(1)/currentScale)));

   return delta_alpha_s + delta_alpha_s_SM;

}

void MSSMNoFV_low_scale_constraint<Two_scale>::calculate_DRbar_yukawa_couplings()
{
   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();
}

void MSSMNoFV_low_scale_constraint<Two_scale>::calculate_Yu_DRbar()
{
   Eigen::Matrix<double,3,3> topDRbar(Eigen::Matrix<double,3,3>::Zero());
   topDRbar(0,0)      = oneset.displayMass(mUp);
   topDRbar(1,1)      = oneset.displayMass(mCharm);
   topDRbar(2,2)      = oneset.displayMass(mTop);

   if (model->get_thresholds())
      topDRbar(2,2) = model->calculate_MFt_DRbar(oneset.displayPoleMt(), 2);

   const auto vu = MODELPARAMETER(vu);
   MODEL->set_Yu((1.4142135623730951*topDRbar)/vu);

}

void MSSMNoFV_low_scale_constraint<Two_scale>::calculate_Yd_DRbar()
{
   Eigen::Matrix<double,3,3> bottomDRbar(Eigen::Matrix<double,3,3>::Zero());
   bottomDRbar(0,0)   = oneset.displayMass(mDown);
   bottomDRbar(1,1)   = oneset.displayMass(mStrange);
   bottomDRbar(2,2)   = oneset.displayMass(mBottom);

   if (model->get_thresholds())
      bottomDRbar(2,2) = model->calculate_MFb_DRbar(oneset.displayMass(mBottom), 2);

   const auto vd = MODELPARAMETER(vd);
   MODEL->set_Yd((1.4142135623730951*bottomDRbar)/vd);

}

void MSSMNoFV_low_scale_constraint<Two_scale>::calculate_Ye_DRbar()
{
   Eigen::Matrix<double,3,3> electronDRbar(Eigen::Matrix<double,3,3>::Zero());
   electronDRbar(0,0) = oneset.displayMass(mElectron);
   electronDRbar(1,1) = oneset.displayMass(mMuon);
   electronDRbar(2,2) = oneset.displayMass(mTau);

   if (model->get_thresholds())
      electronDRbar(2,2) = model->calculate_MFtau_DRbar(oneset.displayMass(mTau), 2);

   const auto vd = MODELPARAMETER(vd);
   MODEL->set_Ye((1.4142135623730951*electronDRbar)/vd);

}

} // namespace flexiblesusy
