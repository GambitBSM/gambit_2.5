//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  GAMBIT Core diagnostics implementation.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Pat Scott
///  \date 2013 Aug
///  \date 2014 Mar, Aug, Dec
///
///  \author Tomas Gonzalo
///          (t.e.gonzalo@fys.uio.no)
///  \date 2017 Jun
///
///  \author Markus Prim
///          (markus.prim@cern.ch)
///  \date 2020 Dec
///
///  *********************************************

#include "gambit/Core/core.hpp"
#include "gambit/Core/modelgraph.hpp"
#include "gambit/ScannerBit/plugin_loader.hpp"
#include "gambit/Utils/screen_print_utils.hpp"
#include "gambit/Utils/stream_overloads.hpp"
#include "gambit/Utils/table_formatter.hpp"
#include "gambit/Utils/util_functions.hpp"
#include "gambit/cmake/cmake_variables.hpp"

#include <memory>
#include <string>

namespace Gambit
{

  /// Basic module diagnostic function
  void gambit_core::module_diagnostic()
  {
    YAML::Node gambit_bits_yaml = YAML::LoadFile(GAMBIT_DIR "/config/gambit_bits.yaml");
    auto gambit_bits = gambit_bits_yaml["gambit_bits"].as<std::vector<std::string>>();

    // We need to manually add this here, can not be crawled by our script.
    // We want to sort it again to keep alphabetical ordering from the python script after adding bits explicitly here.
    gambit_bits.emplace_back(std::string{"BackendIniBit"});
    std::sort(gambit_bits.begin(), gambit_bits.end());

    table_formatter table("Modules", "Status", "#functions");
    table.padding(1);
    table.capitalize_title();
    table.default_widths(25);

    for (const auto &bit: gambit_bits)
    {
      table << bit;

      auto result = std::find(std::begin(modules), std::end(modules), bit);
      if (result == std::end(modules))
      {
        table.red() << "ditched";
        table << "n/a";
      }
      else
      {
        int nf = 0;
        for (const auto &functor : functorList)
        {
          if (functor->origin() == bit) nf++;
        }
        table.green() << "OK";
        table << nf;
      }
    }

    std::stringstream out;
    out << table.str();
    print_to_screen(out.str(), "module");
  }

  /// Basic module function diagnostic function
  void gambit_core::module_function_diagnostic()
  {
    // TODO: Implement!
    std::cout << "You invoked \"gambit_core::module_function_diagnostic()\", but there is nothing to see here yet." << std::endl;
  }

  /// Basic backend diagnostic function
  void gambit_core::backend_diagnostic()
  {
    bool all_good = true;
    table_formatter table("Backends", "Version", "Path to lib", "Status ", " #func ", "#types ", "#ctors");
    table.padding(1);
    table.capitalize_title();
    table.default_widths(18, 7, 70, 13, 3, 3);

    // Loop over all registered backends
    for (const auto &backend : backend_versions)
    {
      // Loop over all registered versions of this backend
      for (const auto &version : backend.second)
      {
        int nfuncs = 0;
        int ntypes = 0;
        int nctors = 0;

        // Retrieve the status and path info.
        const str path = backendData->path(backend.first, version);          // Get the path of this backend
        const str status = backend_status(backend.first, version, all_good); // Save the status of this backend

        // Count up the number of functions in this version of the backend, using the registered functors.
        for (const auto &functor : backendFunctorList)
        {
          if (functor->origin() == backend.first and functor->version() == version) nfuncs++; // If backend matches, increment the count of the functions in this version
        }

        // Do things specific to versions that provide classes
        if (backendData->classloader.at(backend.first + version))
        {
          if (backendData->classes.count(backend.first + version) != 0)
          {
            const std::set<str> classes = backendData->classes.at(backend.first + version); // Retrieve classes loaded by this version
            ntypes = classes.size();                                                        // Get the number of classes loaded by this backend
            for (const auto &class_ : classes)                                              // class is a C++ keyword, so use class_ here which allows the same readability.
            {
              nctors += backendData->factory_args.at(backend.first + version + class_).size(); // Add the number of factories for this class to the total
            }
          }
        }

        // Print the info
        const str firstentry = (&version == std::addressof(*backend.second.begin()) ? backend.first : "");
        table << firstentry << version << path;
        if (status == "OK")
          table.green() << status;
        else
          table.red() << status;
        table << " " + std::to_string(nfuncs) << std::to_string(ntypes) << std::to_string(nctors);
      }
    }

    std::stringstream out;
    out << "All relative paths are given with reference to " << GAMBIT_DIR << ".";
    if (all_good)
      out << endl
          << endl
          << "\033[032m"
          << "All your backend are belong to us."
          << "\033[0m" << endl;
    out << endl;
    out << table.str();
    print_to_screen(out.str(), "backend");
  }

  /// Basic backend function diagnostic function
  void gambit_core::backend_function_diagnostic()
  {
    // TODO: Implement!
    std::cout << "You invoked \"gambit_core::backend_function_diagnostic()\", but there is nothing to see here yet." << std::endl;
  }

  /// Basic model diagnostic function
  void gambit_core::model_diagnostic()
  {
    table_formatter table("Model", "Parent", "Parameters");
    table.default_widths(35);
    table.padding(1);
    table.capitalize_title();

    for (const auto &functor : primaryModelFunctorList)
    {
      const str model = functor->origin();
      const str parentof = modelInfo->get_parent(model);
      const int nparams = functor->valuePtr()->getNumberOfPars();
      table << model << parentof << nparams;
    }

    std::stringstream out;
#ifdef HAVE_GRAPHVIZ
    // Create and spit out graph of the model hierarchy.
    const str graphfile = Utils::runtime_scratch() + "GAMBIT_model_hierarchy.gv";
    ModelHierarchy modelGraph(*modelInfo, primaryModelFunctorList, graphfile, false);
    out << endl << "Created graphviz model hierarchy graph in " + graphfile + "." << endl;
    out << endl << "To get postscript plot of model hierarchy, please run: " << endl;
    out << GAMBIT_DIR << "/Core/scripts/./graphviz.sh " + graphfile << endl;
#else
    out << endl << "To get postscript plot of model hierarchy, please install graphviz, rerun cmake and remake GAMBIT." << endl;
#endif
    out << table.str();
    print_to_screen(out.str(), "model");
  }

  /// Basic capability diagnostic function
  void gambit_core::capability_diagnostic()
  {

    // Default, list-format output header
    table_formatter table("Capabilities", "Available in (modules/models)", "Available in (backends)");
    table.padding(1);
    table.capitalize_title();
    table.default_widths(35, 25);
    for (const auto &capability : capabilities)
    {
      // Make sets of matching modules and backends
      std::set<str> modset;
      for (const auto &functor : functorList)
      {
        if (functor->capability() == capability) modset.insert(functor->origin());
      }
      std::set<str> beset;
      for (const auto &functor : backendFunctorList)
      {
        if (functor->capability() == capability) beset.insert(functor->origin());
      }

      // Make strings out of the sets
      std::string mods("");
      for (const auto &mod : modset)
      {
        if (&mod != std::addressof(*modset.begin())) mods += ", ";
        mods += mod;
      }
      std::string bes("");
      for (const auto &be : beset)
      {
        if (&be != std::addressof(*beset.begin())) bes += ", ";
        bes += be;
      }

      // Identify the primary model parameters with their models.
      if (mods.empty() and bes.empty()) mods = capability.substr(0, capability.length() - 11);

      // Print the entry in the table (list format)
      table << capability << mods << bes;
    }
    std::stringstream out;
    out << table.str();
    print_to_screen(out.str(), "capability");
  }

  /// Basic scanner diagnostic function
  void gambit_core::scanner_diagnostic()
  {
    // Import scanner plugin info from ScannerBit
    const std::string output = Scanner::Plugins::plugin_info().print_all("scanner");
    print_to_screen(output, "scanners");
  }

  /// Basic test function diagnostic function
  void gambit_core::test_function_diagnostic()
  {
    const std::string output = Scanner::Plugins::plugin_info().print_all("objective");
    print_to_screen(output, "objectives");
  }

  void gambit_core::prior_diagnostic()
  {
    const std::string output = Scanner::Plugins::plugin_info().print_priors("priors");
    print_to_screen(output, "priors");
  }

  void gambit_core::ff_prior_diagnostic(const str &command)
  {
    if (command != "priors")
    {
      const std::string output = Scanner::Plugins::plugin_info().print_priors(command);
      print_to_screen(output, command);
    }
  }

  /// Free-form module diagnostic function
  void gambit_core::ff_module_diagnostic(const str &command)
  {
    std::stringstream out;
    for (const auto &module : modules)
    {
      if (command == module)
      {
        out << "Information for module " << module << "." << std::endl << std::endl;
        table_formatter table("", "", "", "LOOP MANAGER:", "DEPENDENCIES / BACKEND REQUIREMENTS");
        table.new_titles("Function", "Capability", "Result Type", " IS  NEEDS", "[type]         {type}");
        table.padding(1);
        table.capitalize_title();
        table.default_widths(30, 35, 35, 19, 27);

        for (const auto &functor : functorList)
        {
          if (functor->origin() == module) // Module matches
          {
            const str f = functor->name();
            const str c = functor->capability();
            const str t = functor->type();
            const str islm = functor->canBeLoopManager() ? "Yes" : "No ";
            const str nlm = functor->loopManagerCapability();
            const std::set<sspair> deps = functor->dependencies();
            const std::set<sspair> reqs = functor->backendreqs();
            table.no_newline() << f << c << t << (" " + islm + " " + nlm);

            if (not deps.empty())
            {
              for (const auto &dep : deps)
              {
                if (&dep != std::addressof(*deps.begin()))
                  table.no_newline() << ""
                                     << ""
                                     << ""
                                     << "" << dep.first + " [" + dep.second + "]";
                else
                  table << dep.first + " [" + dep.second + "]";
              }
            }
            if (not reqs.empty())
            {
              for (const auto &req : reqs)
              {
                if (&req != std::addressof(*reqs.begin()) or not deps.empty())
                  table.no_newline() << ""
                                     << ""
                                     << ""
                                     << "" << req.first + " {" + req.second + "}";
                else
                  table << req.first + " {" + req.second + "}";
              }
            }
            if (reqs.empty() and deps.empty()) table << "";
            table.newline(table.row_pos() - 1);
          }
        }
        out << table.str();
        break;
      }
    }
    print_to_screen(out.str(), command);
  }

  /// Free-form module function diagnostic function
  void gambit_core::ff_module_function_diagnostic(const str &command)
  {
    std::stringstream out;

    // Iterate over all module functions to see if command matches one of them
    for (const auto &functor : functorList)
    {
      const str indent = "  ";
      const str &name = functor->name();
      if (command == name)
      {
        out << "Information for module function \"" << name << "\"." << std::endl << std::endl;

        // Basic information about the module function (all module function will have this)
        out << indent << "module:      "  << functor->origin() << std::endl;
        out << indent << "capability:  "  << functor->capability() << std::endl;
        out << indent << "result type: "  << functor->type() << std::endl << std::endl;

        // List models that are allowed
        const auto& allowed_models = functor->getAllowedModels();
        if (allowed_models.size() > 0)
        {
          out << indent << "allowed models:\n";
          for (const auto &model : allowed_models)
            out << indent << indent << model << std::endl;
        }
        else
        {
          out << indent << "ALL models allowed" << std::endl;
        }

        // List dependencies (if any)
        const auto& dependencies = functor->dependencies();
        if (dependencies.size() > 0)
        {
          out << "\n" << indent << "dependencies:\n";
          for (const auto &dep : dependencies)
            out << indent << indent << dep.first << " [" << dep.second  << "]" << std::endl;
        }

        // List backend requirements (if any)
        const auto& be_reqs = functor->backendreqs();
        if (be_reqs.size() > 0)
        {
          out << "\n" << indent << "backend requirements:\n";
          for (const auto &be_req : be_reqs)
            out << indent << indent << be_req.first << " {" << be_req.second  << "}" << std::endl;
        }

        // Conditional dependencies and backend requirements need a bit of tideous work.
        const auto& all_cond_models = functor->getConditionalModels();

        // All models in "allowed_models" are also part of "all_cond_models",
        // as these models have a conditional dependency on their respective ModelParameters.
        // We are only interested in the other condtional models here.
        std::set<str> proper_cond_models{};
        std::set_difference(all_cond_models.begin(), all_cond_models.end(),
                            allowed_models.begin(), allowed_models.end(),
                            std::inserter(proper_cond_models, proper_cond_models.begin()));

        if (proper_cond_models.size() > 0)
        {
          // Go through all proper conditional models
          // and collect all conditional dependencies and backend requirements
          // in maps that contains the (name,type) pairs as keys and a set of models
          // associated with it as value. (Inverted logic of the original maps).
          std::map< std::pair<str,str>, std::set<str> > cond_deps_map{};
          std::map< std::pair<str,str>, std::set<str> > cond_be_reqs_map{};

          for (const auto &cond_model : proper_cond_models)
          {
            for (const auto &cond_dep: functor->model_conditional_dependencies(cond_model))
            {
              // If the "(name,type)"" pair of the conditional dependency is not yet a key,
              // then create it, with an empty set.
              if (cond_deps_map.count(cond_dep) == 0) cond_deps_map[cond_dep] = std::set<str>{};

              // Add the model to the set.
              cond_deps_map[cond_dep].insert(cond_model);
            }
            for (const auto &cond_be_req: functor->model_conditional_backend_reqs(cond_model))
            {
              // If the "(name,type)"" pair of the conditional backend requirement is not yet a key,
              // then create it, with an empty set.
              if (cond_be_reqs_map.count(cond_be_req) == 0) cond_be_reqs_map[cond_be_req] = std::set<str>{};

              // Add the model to the set.
              cond_be_reqs_map[cond_be_req].insert(cond_model);
            }
          }

          // Now that we have collected all conditional dependencies and backend requirements, we can print them.
          if (cond_deps_map.size() > 0)
          {
            out << "\n" << indent << "conditional dependencies:\n";
            for (const auto &cond_dep : cond_deps_map)
            {
              out << indent << indent << cond_dep.first.first << " [" << cond_dep.first.second  << "]" << std::endl;
              for (const auto &cond_model : cond_dep.second)
                out << indent << indent << indent << cond_model << std::endl;
            }
          }

          if (cond_be_reqs_map.size() > 0)
          {
            out << "\n" << indent << "conditional backend requirements:\n";
            for (const auto &cond_be_req : cond_be_reqs_map)
            {
              out << indent << indent << cond_be_req.first.first << " {" << cond_be_req.first.second  << "}" << std::endl;
              for (const auto &cond_model : cond_be_req.second)
                out << indent << indent << indent << cond_model << std::endl;
            }
          }

        }

        out << std::endl;
        break;
      }
    }
    print_to_screen(out.str(), command);
  }

  /// Free-form backend diagnostic function
  void gambit_core::ff_backend_diagnostic(const str &command)
  {
    std::stringstream out;
    // Iterate over all backends to see if command matches one of them
    for (const auto &backend : backend_versions)
    {
      if (command == backend.first)
      {
        bool has_classloader = false;
        out << "Information for backend " << backend.first << "." << std::endl << std::endl;

        // Loop over all registered versions of this backend
        for (const auto &version : backend.second)
        {
          bool who_cares;
          const str path = backendData->corrected_path(backend.first, version); // Save the path of this backend
          const str status = backend_status(backend.first, version, who_cares); // Save the status of this backend
          out << "Version: " << version << std::endl;
          out << "Path to library: " << path << std::endl;
          out << "Library status: " << status << std::endl;

          table_formatter back_table("  Function/Variable", "Capability", "Type", "Status");
          back_table.capitalize_title();
          back_table.default_widths(27, 35, 40, 40);
          back_table.padding(1);
          back_table.top_line(true);
          back_table.bottom_line(true);
          // Loop over all the backend functions and variables
          for (const auto &functor : backendFunctorList)
          {
            if ((functor->origin() == backend.first) and (functor->version() == version))
            {
              const str f = functor->name();
              const str c = functor->capability();
              const str t = functor->type();
              const int s = functor->status();
              back_table << ("  " + f) << c << t;
              if (s == -5)
                back_table.red() << "Mathematica absent";
              else if (s == -2)
                back_table.red() << "Function absent";
              else if (s == -1)
                back_table.red() << "Backend absent";
              else if (s >= 0)
                back_table.green() << "Available";
              else
                back_table << "";
            }
          }
          if (back_table.rows() > 0) out << back_table.str();
          table_formatter class_table("  Class", "Constructor overload", "Status");
          class_table.capitalize_title();
          class_table.default_widths(46, 60, 60);
          class_table.padding(1);
          class_table.top_line(true);
          class_table.bottom_line(true);
          // If this version has classes to offer, print out info on them too
          if (backendData->classloader.at(backend.first + version))
          {
            std::set<str> classes;
            if (backendData->classes.find(backend.first + version) != backendData->classes.end()) classes = backendData->classes.at(backend.first + version);
            has_classloader = true;
            // Loop over the classes
            for (const auto &class_ : classes) // class is a C++ keyword, so use class_ here which allows the same readability.
            {
              const std::set<str> ctors = backendData->factory_args.at(backend.first + version + class_);
              // Loop over the constructors in each class
              for (const auto &ctor : ctors)
              {
                str args = ctor;
                boost::replace_all(args, "my_ns::", "");
                const str ss = backendData->constructor_status.at(backend.first + version + class_ + args);
                const str firstentry = (&ctor == std::addressof(*ctors.begin()) ? class_ : "");
                class_table << ("  " + firstentry) << args;
                if (ss == "OK")
                  class_table.green() << status;
                else
                  class_table.red() << status;
              }
            }
          }
          if (class_table.rows() > 0) out << class_table.str();
        }
        // Tell the user what the default version is for classes of this backend (if there are any).
        if (has_classloader)
        {
          const std::map<str, str> defs = backendData->default_safe_versions;
          const str my_def = (defs.find(backend.first) != defs.end() ? backendData->version_from_safe_version(backend.first, defs.at(backend.first)) : "none");
          out << std::endl << "Default version for loaded classes: " << my_def << std::endl << std::endl;
        }
        break;
      }
    }
    print_to_screen(out.str(), command);
  }

  /// Free-form backend function diagnostic function
  void gambit_core::ff_backend_function_diagnostic(const str &command)
  {
    std::stringstream out;

    // Iterate over all backend functions to see if command matches one of them
    for (const auto &functor : backendFunctorList)
    {
      const str indent = "  ";
      const str &name = functor->name();
      if (command == name)
      {
        out << "Information for backend function \"" << name << "\"." << std::endl << std::endl;

        // Basic information about the module function (all module function will have this)
        out << indent << "backend:     "  << functor->origin() << std::endl;
        out << indent << "version:     "  << functor->version() << std::endl;
        out << indent << "capability:  "  << functor->capability() << std::endl;
        out << indent << "result type: "  << functor->type() << std::endl << std::endl;

        // List models that are allowed
        const auto& allowed_models = functor->getAllowedModels();
        if (allowed_models.size() > 0)
        {
          out << indent << "allowed models:\n";
          for (const auto &model : allowed_models)
            out << indent << indent << model << std::endl;
        }
        else
        {
          out << indent << "ALL models allowed" << std::endl;
        }

        out << std::endl;
        //break;
      }
    }
    print_to_screen(out.str(), command);
  }

  /// Free-form model diagnostic function
  void gambit_core::ff_model_diagnostic(const str &command)
  {
    std::stringstream out;
    // Iterate over all models to see if command matches one of them
    for (const auto &functor : primaryModelFunctorList)
    {
      const str model = functor->origin();
      if (command == model)
      {
        out << "Information for model " << model << "." << endl << endl;

        // Retrieve info on this capability from the database file
        const model_info mod = get_model_info(model);

        // Need copies of lineage and descendant vectors with self-reference removed
        std::vector<str> lin_X = mod.lineage;
        std::vector<str> des_X = mod.descendants;

        // Erase element matching name
        lin_X.erase(std::remove(lin_X.begin(), lin_X.end(), mod.name), lin_X.end());
        des_X.erase(std::remove(des_X.begin(), des_X.end(), mod.name), des_X.end());

        out << "  Parent Model: " << mod.parent << std::endl;
        out << "  Number of parameters: " << mod.nparams << std::endl;
        out << "  Parameter names:" << mod.parameters << std::endl;
        out << "  'Ancestor' models:" << lin_X << std::endl;
        out << "  'Descendant' models:" << des_X << std::endl;
        out << "  Description: " << endl << mod.description << std::endl;

        break;
      }
    }
    print_to_screen(out.str(), command);
  }

  /// Free-form capability diagnostic function
  void gambit_core::ff_capability_diagnostic(const str &command)
  {
    std::stringstream out;
    // Iterate over all capabilities to see if command matches one of them
    for (const auto &capability : capabilities)
    {
      if (command == capability)
      {
        out << "Information for capability " << capability << "." << std::endl << std::endl;

        // Retrieve info on this capability from the database file
        const capability_info cap = get_capability_info(capability);
        std::vector<std::pair<str, std::map<str, std::set<std::pair<str, str>>>>> origins;
        origins.push_back(std::pair<str, std::map<str, std::set<std::pair<str, str>>>>("modules", cap.modset));
        origins.push_back(std::pair<str, std::map<str, std::set<std::pair<str, str>>>>("backends", cap.beset));
        // Loop over {modules, backends}
        for (const auto &origin : origins)
        {
          if (not origin.second.empty())
          {
            out << "  Available in " << origin.first << ": " << std::endl;
            // Loop over modules or backends
            for (const auto &module_or_backend : origin.second)
            {
              out << "    " << module_or_backend.first << ": " << std::endl;
              // Loop over matching module/backend functions
              for (const auto &function : module_or_backend.second)
              {
                // Spit out: function name [function type]
                out << "      function " << function.first << " [type " << function.second << "]" << std::endl;
              }
            }
            out << std::endl;
          }
        }
        out << "  Description: " << std::endl << cap.description << std::endl;
        break;
      }
    }
    print_to_screen(out.str(), command);
  }

  /// Free-form scanner diagnostic function
  void gambit_core::ff_scanner_diagnostic(const str &command)
  {
    const std::string output = Scanner::Plugins::plugin_info().print_plugin("scanner", command);
    print_to_screen(output, command);
  }

  /// Free-form test function diagnostic function
  void gambit_core::ff_test_function_diagnostic(const str &command)
  {
    const std::string output = Scanner::Plugins::plugin_info().print_plugin("objective", command);
    print_to_screen(output, command);
  }

} // namespace Gambit
