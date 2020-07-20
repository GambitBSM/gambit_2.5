//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  CosmoBit routines relating to BBN.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Selim C. Hotinli
///          (selim.hotinli14@pimperial.ac.uk)
///  \date 2017 Jul
///  \date 2018 May
///  \date 2018 Aug - Sep
///
///  \author Patrick Stoecker
///          (stoecker@physik.rwth-aachen.de)
///  \date 2017 Nov
///  \date 2018 Jan - May
///  \date 2019 Jan, Feb, June, Nov
///
///  \author Janina Renk
///          (janina.renk@fysik.su.se)
///  \date 2018 June
///  \date 2019 Mar,June
///
///  \author Sanjay Bloor
///          (sanjay.bloor12@imperial.ac.uk)
///  \date 2019 June, Nov
///
///  \author Sebastian Hoof
///          (hoof@uni-goettingen.de)
///  \date 2020 Mar
///
///  \author Pat Scott
///          (pat.scott@uq.edu.au)
///  \date 2018 Mar
///  \date 2019 Jul
///  \date 2020 Apr
///
///  *********************************************

#include <memory>  // make_unique pointers
#include <stdint.h> // save memory addresses as int

#include <gsl/gsl_linalg.h>

#include "gambit/Utils/yaml_options.hpp"
#include "gambit/Utils/ascii_table_reader.hpp"
#include "gambit/Utils/ascii_dict_reader.hpp"
#include "gambit/Elements/gambit_module_headers.hpp"
#include "gambit/CosmoBit/CosmoBit_rollcall.hpp"
#include "gambit/CosmoBit/CosmoBit_types.hpp"
#include "gambit/CosmoBit/CosmoBit_utils.hpp"

namespace Gambit
{

  namespace CosmoBit
  {
    using namespace LogTags;

    /// Add parameters of relicparam structure that should be set to non-default values
    /// to the AlterBBN_input map.
    /// If you want to modify a parameter which has not been used in CosmoBit before, simply
    /// add it to the function 'fill_cosmomodel' in 'AlterBBN_<version>.cpp' and to the
    /// set of 'known' parameters 'known_relicparam_options'.
    void AlterBBN_Input(map_str_dbl &result)
    {
      using namespace Pipes::AlterBBN_Input;

      // If we are using some of the "non-standard energy content" models, set the
      // inputs for the AlterBBN_input map according to the parameters of that model.
      // In case we are not using one of these models, we use the default values
      // (i.e. eta inferred from LCDM, Nnu = 3.046 and dNnu = 0)
      if (ModelInUse("etaBBN_rBBN_rCMB_dNurBBN_dNurCMB"))
      {
        const ModelParameters& NP_params = *Dep::etaBBN_rBBN_rCMB_dNurBBN_dNurCMB_parameters;

        double dNurBBN =  NP_params.at("dNur_BBN");

        // Check if the input for dNeff is negative (unphysical)
        // NOTE: CosmoBit performs no sanity checks if you allow negative dNEff; you're on your own.
        static bool allow_negative_delta_N_ur = runOptions->getValueOrDef<bool>(false,"allow_negative_delta_N_ur");

        // Only let the user have negative contributions to dNEff if they've signed off on it.
        if ( (!allow_negative_delta_N_ur) && (dNurBBN < 0.0) )
        {
          std::ostringstream err;
          err << "A negative value for \"dNur_BBN\" is unphysical and is not allowed in CosmoBit by default!\n\n";
          err << "If you want to proceed with negative values, please add\n\n";
          err << "  - module: CosmoBit\n";
          err << "    options:\n";
          err << "      allow_negative_delta_N_ur: true\n\n";
          err << "to the Rules section of the YAML file.";
          CosmoBit_error().raise(LOCAL_INFO,err.str());
        }

        //If check is passed, set inputs.
        result["Nnu"]=3.046*pow(NP_params.at("r_BBN"),4); // contribution from SM neutrinos
        result["dNnu"]=dNurBBN;    // dNnu: within AlterBBN scenarios in which the sum Nnu+dNnu is the same are identical
      }
      else
      {
        result["Nnu"]=3.046; // contribution from SM neutrinos
        result["dNnu"]=0.;   // no extra ur species in standard LCDM model
      }
      result["eta0"] = *Dep::etaBBN;

      // Adopt the default value for the neutron lifetime in seconds if is not passed as a model parameter
      if (ModelInUse("nuclear_params_neutron_lifetime"))
      {
        result["neutron_lifetime"] = Dep::nuclear_params_neutron_lifetime_parameters->at("neutron_lifetime");
      }
      else
      {
        result["neutron_lifetime"] = 879.4; // (PDG 2019 recommendation http://pdg.lbl.gov/2019/listings/rpp2019-list-n.pdf);
      }

      result["failsafe"] = runOptions->getValueOrDef<double>(3,"failsafe");
      result["err"] = runOptions->getValueOrDef<double>(0,"err");

      logger() << "Set AlterBBN with parameters eta = " << result["eta0"] << ", Nnu = " << result["Nnu"] << ", dNnu = " << result["dNnu"] << ", neutron lifetime = " << result["neutron_lifetime"];
      logger() << " and error params: failsafe = " << result["failsafe"] << ", err = " << result["err"] << EOM;
    }


    /// Check the validity of a correlation matrix for AlterBBN likelihood calculations given in the YAML file, and use it to populate a correlation matrix object
    void populate_correlation_matrix(const std::map<std::string, int>& abund_map, std::vector<std::vector<double>>& corr,
                                     std::vector<double>& relerr, bool use_relative_errors, const Options& runOptions)
    {
      std::vector<str> isotope_basis = runOptions.getValue<std::vector<str> >("isotope_basis");
      std::vector<std::vector<double>> tmp_corr = runOptions.getValue<std::vector<std::vector<double>>>("correlation_matrix");
      std::vector<double> tmp_relerr;
      unsigned int nisotopes = isotope_basis.size();

      // Check if the size of the isotope_basis and the size of the correlation matrix agree
      if (nisotopes != tmp_corr.size())
      {
        std::ostringstream err;
        err << "The length of the list \'isotope_basis\' and the size of the correlation matrix \'correlation_matrix\' do not agree";
        CosmoBit_error().raise(LOCAL_INFO, err.str());
      }

      // If the relative errors are also given, then do also a check if the length of the list is correct and if the entries are positive.
      if (use_relative_errors)
      {
        tmp_relerr = runOptions.getValue< std::vector<double> >("relative_errors");
        if (nisotopes != tmp_relerr.size())
        {
          std::ostringstream err;
          err << "The length of the list \'isotope_basis\' and the length of \'relative_errors\' do not agree";
          CosmoBit_error().raise(LOCAL_INFO, err.str());
        }
        for (std::vector<double>::iterator it = tmp_relerr.begin(); it != tmp_relerr.end(); it++)
        {
          if (*it <= 0.0)
          {
            std::ostringstream err;
            err << "One entry for the relative error is not positive";
            CosmoBit_error().raise(LOCAL_INFO, err.str());
          }
        }
      }

      // Check if the correlation matrix is square
      for (std::vector<std::vector<double>>::iterator it = tmp_corr.begin(); it != tmp_corr.end(); it++)
      {
        if (it->size() != nisotopes)
        {
          std::ostringstream err;
          err << "The correlation matrix is not a square matrix";
          CosmoBit_error().raise(LOCAL_INFO, err.str());
        }
      }

      // Check if the entries in the correlation matrix are reasonable
      for (unsigned int ie=0; ie<nisotopes; ie++)
      {
        // Check if the diagonal entries are equal to 1.
        if (std::abs(tmp_corr.at(ie).at(ie) - 1.) > 1e-6)
        {
          std::ostringstream err;
          err << "Not all diagonal elements of the correlation matrix are 1.";
          CosmoBit_error().raise(LOCAL_INFO, err.str());
        }
        for (unsigned int je=0; je<=ie; je++)
        {
          // Check for symmetry
          if (std::abs(tmp_corr.at(ie).at(je) - tmp_corr.at(je).at(ie)) > 1e-6)
          {
            std::ostringstream err;
            err << "The correlation matrix is not symmetric";
            CosmoBit_error().raise(LOCAL_INFO, err.str());
          }
          // Check if the off-diagonal elements are between -1 and 1.
          if (std::abs(tmp_corr.at(ie).at(je)) >= 1. && (ie != je))
          {
            std::ostringstream err;
            err << "The off-diagonal elements of the correlation matrix are not sensible (abs(..) > 1)";
            CosmoBit_error().raise(LOCAL_INFO, err.str());
          }
        }
      }

      // Check if the isotopes in the basis are actually known.
      for (std::vector<str>::iterator it = isotope_basis.begin(); it != isotope_basis.end(); it++)
      {
        if (abund_map.count(*it) == 0)
        {
          std::ostringstream err;
          err << "I do not recognise the element \'" << *it << "\'";
          CosmoBit_error().raise(LOCAL_INFO, err.str());
        }
      }

      // Populate the correlation matrix and relative errors
      for (std::vector<str>::iterator it1 = isotope_basis.begin(); it1 != isotope_basis.end(); it1++)
      {
        int ie  =  abund_map.at(*it1);
        int i = std::distance( isotope_basis.begin(), it1 );
        // If the relative errors are given, fill relerr with the respective values (-1.0 refers to no errors given).
        if (use_relative_errors) relerr.at(ie) = tmp_relerr.at(i);
        for (std::vector<str>::iterator it2 = isotope_basis.begin(); it2 != isotope_basis.end(); it2++)
        {
          int je = abund_map.at(*it2);
          int j = std::distance( isotope_basis.begin(), it2 );
          corr.at(ie).at(je) = tmp_corr.at(i).at(j);
        }
      }
    }

    /// Compute elemental abundances from BBN
    void compute_BBN_abundances(BBN_container &result)
    {
      using namespace Pipes::compute_BBN_abundances;

      // Global variable of AlterBBN (# computed element abundances)
      const static int NNUC = BEreq::get_NNUC();

      // In AlterBBN ratioH and cov_ratioH are arrays of fixed length.
      // With certain compiler versions (gcc 5.4.0) we have seen memory corruption problems
      // if we initialise these as double ratioH[NNUC+1], since the memory was not allocated properly. 
      // Fixed size arrays do not seem to be properly supported even though there are no errors at compile time.
      // Using a unique pointer for ratioH and a 2D vector for cov_ratioH avoids these problems.
      auto deleter = [&](double* ptr){delete [] ptr;};
      std::unique_ptr<double[], decltype(deleter)> ratioH(new double[NNUC+1](), deleter);
      std::unique_ptr<double[], decltype(deleter)> cov_ratioH(new double[(NNUC+1)*(NNUC+1)](), deleter);

      static bool first = true;
      const static bool use_fudged_correlations = (runOptions->hasKey("correlation_matrix") && runOptions->hasKey("isotope_basis"));
      const static bool use_relative_errors = runOptions->hasKey("relative_errors");
      static std::vector<double> relerr(NNUC+1, -1.0);
      static std::vector<std::vector<double>> corr(NNUC+1, std::vector<double>(NNUC+1, 0.0));

      if (first)
      {
        // Init abundance map and allocate arrays in result
        result.set_abund_map(BEreq::get_abund_map_AlterBBN());
        result.init_arr_size(NNUC);

        // Work out which isotopes have been requested downstream
        // From the YAML sub-capabilities
        std::vector<str> v = Downstream::subcaps->getNames();
        // From other dependencies
        if (Downstream::neededFor("helium_abundance")) v.push_back("He4");
        result.set_active_isotopes(std::set<str>(v.begin(), v.end()));
        if (result.get_active_isotopes().empty())
        {
          str err = "No relevant sub-capabilities found for compute_BBN_abundances.  Please specify elements to\n"
                    "compute abundances for in the ObsLikes section of your yaml file as in e.g.\n"
                    "  sub_capabilities: [He4, D, Li7]";
          CosmoBit_error().raise(LOCAL_INFO, err);
        }

        // Process user-defined correlations (if provided)
        if (use_fudged_correlations)
        {
          for (int ie = 1; ie < NNUC; ie++) corr.at(ie).at(ie) = 1.;
          const std::map<std::string, int>& abund_map = result.get_abund_map();
          populate_correlation_matrix(abund_map, corr, relerr, use_relative_errors, *runOptions);
        }

        // Here for a good time, not a long time
        first = false;
      }

      // Fill AlterBBN_input map with the parameters for the model in consideration
      map_str_dbl AlterBBN_input = *Dep::AlterBBN_Input;

      // Call AlterBBN routine to calculate element abundances (& errors -- depending
      // on error calculation settings made with parameters 'err' and failsafe set in
      // 'AlterBBN_Input')
      if (not BEreq::call_nucl_err(AlterBBN_input, &ratioH[0], &cov_ratioH[0]))
      {
        std::ostringstream err;
        err << "AlterBBN calculation for primordial element abundances failed. Invalidating Point.";
        invalid_point().raise(err.str());
      }

      // Fill relative errors
      std::vector<double> err_ratio(NNUC+1,0);
      if (use_fudged_correlations) for (const int& ie : result.get_active_isotope_indices())
      {
        if (use_relative_errors && (relerr.at(ie) > 0.0))
          err_ratio.at(ie) =  relerr.at(ie) * ratioH[ie];
        else
          // Get every diagonal element (row and line 0 is not filled)
          err_ratio.at(ie) = sqrt(cov_ratioH[ie*(NNUC+1)+ie]);
      }

      // Fill abundances and covariance matrix of BBN_container with requested results from AlterBBN
      for (const int& ie : result.get_active_isotope_indices())
      {
        result.set_BBN_abund(ie, ratioH[ie]);
        for (const int& je : result.get_active_isotope_indices())
        {
          if (use_fudged_correlations)
            result.set_BBN_covmat(ie, je, corr.at(ie).at(je) * err_ratio.at(ie) * err_ratio.at(je));
          else
            result.set_BBN_covmat(ie, je, cov_ratioH[ie*(NNUC+1)+je]);
        }
      }
    }

    /// Extract helium-4 abundance from BBN abundance container
    void extract_helium_abundance(double &result)
    {
      result = Pipes::extract_helium_abundance::Dep::BBN_abundances->get_BBN_abund("He4");
    }

    /// Compute the overall log-likelihood from BBN
    void compute_BBN_LogLike(double &result)
    {
      using namespace Pipes::compute_BBN_LogLike;

      double chi2 = 0;
      int ii = 0;
      int ie,je,s;

      BBN_container BBN_res = *Dep::BBN_abundances; // Fill BBN_container with abundance results from AlterBBN
      const std::map<std::string, int>& abund_map = BBN_res.get_abund_map();

      static bool first = true;
      const static str filename = runOptions->getValueOrDef<std::string>("default.dat", "DataFile");
      const static str path_to_file = GAMBIT_DIR "/CosmoBit/data/BBN/" + filename;
      static std::map<std::string,std::vector<double>> dict;
      static int nobs;

      if (first)
      {
        // Read the data
        const ASCIIdictReader data(path_to_file);
        logger() << "BBN data read from file '" << filename << "'." << EOM;

        // Execute initialisation checks on the contents of the datafile
        std::map<std::string,std::vector<double>> td = data.get_dict();
        const str err = "Double entry for one element in BBN data file '" + filename + "'. \nYou can only enter one measurement per element.";
        if (data.duplicated_keys()) CosmoBit_error().raise(LOCAL_INFO, err);
        std::vector<sspair> doppelgangers = {{"Yp", "He4"}, {"D","H2"}};
        for (const sspair& x : doppelgangers)
        {
          if (td.count(x.first) == 0 and td.count(x.second) == 0)
          {
            CosmoBit_error().raise(LOCAL_INFO, err + "\nNote that "+x.first+" and "+x.second+" are the same species!");
          }
        }

        // Check that all isotopes requested in the YAML file exist in the datafile, and keep only the data needed
        const std::vector<str>& v = Downstream::subcaps->getNames();
        for (const str& isotope : std::set<str>(v.begin(), v.end()))
        {
          auto it = td.find(isotope);
          // Check if the isotope has been listed as a subcapability
          if (it == td.end())
          {
            str alt_name = "";
            for (const sspair& pair : doppelgangers)
            {
              if (isotope == pair.first) alt_name = pair.second;
              if (isotope == pair.second) alt_name = pair.first;
            }
            // Check if the isotope's doppelganger has been listed as a subcapability
            if (alt_name != "") it = td.find(alt_name);
          }
          // Throw an error if the isotope is not found in the datafile
          if (it == td.end()) CosmoBit_error().raise(LOCAL_INFO, "Did not find observations for "+isotope+" in "+filename+".");
          // Otherwise, save the corresponding dictionary entry
          else dict[isotope] = it->second;
        }

        // Save the number of observations to include in the likelihood.
        nobs = dict.size();
        if (nobs == 0)
        {
          str err = "No relevant sub-capabilities found for compute_BBN_LogLike.  Please specify elements to\n"
                    "compute likelihoods from in the ObsLikes section of your YAML file as in e.g.\n"
                    "  sub_capabilities: [He4, D]";
          CosmoBit_error().raise(LOCAL_INFO, err);
        }

        // Init out.
        first = false;
      }

      // Init vectors with observations, predictions and covariance matrix
      double prediction[nobs],observed[nobs],sigmaobs[nobs],translate[nobs];
      gsl_matrix *cov = gsl_matrix_alloc(nobs, nobs);
      gsl_matrix *invcov = gsl_matrix_alloc(nobs, nobs);
      gsl_permutation *p = gsl_permutation_alloc(nobs);

      // Iterate through observation dictionary to fill observed, sigmaobs and prediction arrays
      for(std::map<std::string,std::vector<double>>::iterator iter = dict.begin(); iter != dict.end(); ++iter)
      {
        std::string key = iter->first; // iter = ["element key", [mean, sigma]]
        if(abund_map.count(key)!=1)   // throw error if element not contained in abundance map was entered in data file
        {
          std::ostringstream err;
          err << "Unknown element '"<< key <<"' in BBN data file '"<< filename<<"'. \nYou can only enter 'Yp' or 'He4', 'D' or 'H2', 'He3', 'Li7'.";
          CosmoBit_error().raise(LOCAL_INFO, err.str());
        }
        translate[ii]=abund_map.at(key); // to order observed and predicted abundances consistently
        observed[ii]=iter->second[0];
        sigmaobs[ii]=iter->second[1];
        prediction[ii]= BBN_res.get_BBN_abund(key);
        ii++;
      }

      // Fill the covariance matrix
      for(ie=0;ie<nobs;ie++) {for(je=0;je<nobs;je++) gsl_matrix_set(cov, ie, je,pow(sigmaobs[ie],2.)*(ie==je)+BBN_res.get_BBN_covmat(translate[ie], translate[je]));}

      // Compute the LU decomposition and inverse of covariance matrix
      gsl_linalg_LU_decomp(cov,p,&s);
      gsl_linalg_LU_invert(cov,p,invcov);

      // Compute the determinant of the inverse of the covariance matrix
      double det_cov = gsl_linalg_LU_det(cov,s);

      // compute chi2
      for(ie=0;ie<nobs;ie++) for(je=0;je<nobs;je++) chi2+=(prediction[ie]-observed[ie])*gsl_matrix_get(invcov,ie,je)*(prediction[je]-observed[je]);
      result = -0.5*(chi2 + log(pow(2*pi,nobs)*det_cov));

      logger() << "BBN LogLike computed to be: " << result << EOM;

      gsl_matrix_free(cov);
      gsl_matrix_free(invcov);
      gsl_permutation_free(p);
    }

  } // namespace CosmoBit

} // namespace Gambit
