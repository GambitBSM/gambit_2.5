//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  This is a minimal (pure virtual) precursor 
///  to the printer base class, for use only in 
///  ScannerBit. This helps to minimize the parts 
///  of the printers which are essential for 
///  ScannerBit to run.
///
///  Now includes base 'reader' class as well,
///  for retrieving data from printer output
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///   
///  \author Ben Farmer
///          (benjamin.farmer@fysik.su.se)
///  \date 2015 Feb
///
///  *********************************************

#ifndef __base_base_printer_hpp__
#define __base_base_printer_hpp__

// Standard libraries
#include <string>
#include <sstream>
#include <vector>

// Boost
#include <boost/preprocessor/seq/for_each.hpp>

// Gambit
#include "gambit/ScannerBit/printable_types.hpp"
#include "gambit/Utils/standalone_error_handlers.hpp"
#include "gambit/Printers/printer_id_tools.hpp"
#include "gambit/Utils/new_mpi_datatypes.hpp"

namespace Gambit
{
  // Forward declarations needed for some of the _print/_retrieve functions
  class ModelParameters;
  typedef std::map<std::string,double> map_str_dbl; // can't have commas in macro input


  namespace Printers 
  {
    // Convenienece typedefs for printers
    typedef unsigned int      uint;
    typedef unsigned long int ulong;
    typedef long long int          longlong;
    typedef unsigned long long int ulonglong;

    /// Helper template functions to retrieve type IDs for a type.
    /// ID is just a unique integer for each printable type
    template<class T>
    std::size_t getTypeID(void) { return 0; }
    // ID of zero means no ID assigned

    class BaseBasePrinter  
    {
      private:
        int rank;
        std::set<std::string> print_list; // List of names of data that may be printed (i.e. with printme=true)

      public:
        BaseBasePrinter(): rank(0), printer_enabled(true) {}
        virtual ~BaseBasePrinter() {}
        /// Function to signal to the printer to write buffer contents to disk
        //virtual void flush() {}; // TODO: needed?

        /// Signal printer to reset contents, i.e. delete old data in preperation for replacement
        virtual void reset(bool force=false) = 0;

        /// Retrieve/Set MPI rank (setting is useful for e.g. the postprocessor to re-print points from other ranks)
        int  getRank() {return rank;}
        void setRank(int r) {rank = r;}

        /// Retrieve/Set print list for this printer
        /// Required by e.g. postprocessor.
        std::set<std::string> getPrintList() {return print_list;}
        void                  setPrintList(std::set<std::string>& in) {print_list = in;}        
        void                  addToPrintList(std::string& in) {print_list.insert(in);}

        /// Signal printer that scan is finished, and final output needs to be performed
        virtual void finalise(bool abnormal=false) = 0;

        /// "Turn off" printer; i.e. calls to print functions will do nothing while this is active
        void disable() { printer_enabled = false; }

        /// "Turn on" printer; print calls will work as normal.
        void enable() { printer_enabled = true; }

        // Printer dispatch function. If a virtual function override exists for
        // the print type, info is passed on, otherwise the function call is resolved
        // to a default function which raises an informative runtime error explaining
        // that the type is not printable.
        template<typename T>
        void print(T const& in, const std::string& label,
                   const int vertexID, const uint rank,
                   const ulong pointID)
        {
          if(printer_enabled) _print(in, label, vertexID, rank, pointID);
        }

        // Overload which automatically determines a unique ID code
        // based on the label.
        template<typename T>
        void print(T const& in, const std::string& label,
                   const uint rank,
                   const ulong pointID)
        {
          if(printer_enabled) _print(in, label, rank, pointID);
        }

      protected:
        /// Flag to check if print functions are enabled or disabled
        bool printer_enabled;

        /// Default _print function. Throws an error if no matching 
        /// virtual function for the type of the attempted print is
        /// found.
        template<typename T>
        void _print(T const&, const std::string& label,
                    const int vertexID, const uint,
                    const ulong)
        {
          std::ostringstream err;                               
                                                                
          err << "Attempted to print a functor whose return type "
              << "is not registered as being printable. "
              << "If you really want to print this functor, you must "
              << "add its return type to the PRINTABLE_TYPES sequence "
              << "in \"gambit/Elements/printable_types.hpp\". You will then have "
              << "to define a print function for it in whatever printer "
              << "you are using (see documentation for GAMBIT printers)."
              << "\n  Available info for this print attempt..."                
              << "\n   Label      : " << label                  
              << "\n   vertexID   : " << vertexID               
              << "\n   Type       : " << STRINGIFY(T);       
          printer_error().raise(LOCAL_INFO,err.str());         
        }

        /// Same for overloaded function
        template<typename T>
        void _print(T const& in, const std::string& label,
                    const uint rank,
                    const ulong pointID)
        {
           _print(in,label,rank,pointID);
        }


        // Virtual print methods for base printer classes
        #define VPRINT(r,data,elem)                                \
        virtual void _print(elem const&, const std::string& label, \
                           const int vertexID, const uint /*rank*/, \
                           const ulong /*pointID*/)               \
        {                                                         \
          std::ostringstream err;                                 \
                                                                  \
          err << "No print function override has been "           \
              << "\ndefined for this type (for whatever printer"  \
              << "\nclass the current printer comes from)"        \
              << "\n  Dumping available info..."                  \
              << "\n   Label      : " << label                    \
              << "\n   vertexID   : " << vertexID                 \
              << "\n   Type       : " << STRINGIFY(elem);         \
          printer_error().raise(LOCAL_INFO,err.str());          \
        } \
        \
        /* version that assigns vertexID automatically */         \
        virtual void _print(elem const& in, const std::string& label, \
                           const uint rank, \
                           const ulong pointID)               \
        {                                                         \
           _print(in,label,get_param_id(label),rank,pointID); \
        }

        #define ADD_VIRTUAL_PRINTS(TYPES) BOOST_PP_SEQ_FOR_EACH(VPRINT, _, TYPES)

        // Add the base virtual functions for registered printable and
        // retrievable types, to be overloaded in each printer.
        ADD_VIRTUAL_PRINTS(SCANNER_PRINTABLE_TYPES) 

    };

        /// @{ Printer READ interface
        ///    For reading data back *into* Gambit from a printer output file.
        ///    This is mainly designed for performing "reweighting" of scans, 
        ///    e.g. loading up previously scanned points in order to recompute
        ///    some new observables or likelihoods.
        ///
        ///    It is pretty hard to permit generic read-in of ALL data, so for
        ///    now I will focus on just getting the parameter data. If we read
        ///    in other data we will have to do some gnarly stuff like
        ///    automatically wrap it into functors, and to appropriate
        ///    capabilities etc, in order for it to be usable in other in
        ///    other calculations. So for now, the idea will simply be to
        ///    take the parameter values and recompute everything anew that is
        ///    needed for calculating the new likelihoods or whatever.
        ///    Otherwise it is kind of a nightmare to e.g. reconstruct a
        ///    Spectrum object from printer data, would need special module
        ///    functions and so on that know how to put all the data back
        ///    together. I guess it is possible if we give module writers
        ///    access to the printer read interface so that they can do this
        ///    task themselves... but will leave this aside for now.

        ///    So, need:
        ///      1. some way to iterate through printer output
        ///      2. a generic function that can read in a particular entry, 
        ///         knowing e.g. the following:
        ///       Column 6: #NormalDist_parameters @NormalDist::primary_parameters::mu
        ///         and can put it back into some Gambit object, e.g.
        ///       Parameters: 
        ///         NormalDist:
        ///           mu: 
        ///           sigma:
        ///
        ///       I guess this latter part is scannerbits responsibility, i.e.
        ///       it will have the parameter object and need to fill it with
        ///       numbers that it gets from the printer.
        ///
        ///                functor (model) name    parameter name
        ///      str key = act_it->first + "::" + *par_it;
        ///

    class BaseBaseReader
    {
      public:
        virtual ~BaseBaseReader() {};

        virtual void reset() = 0; // Reset 'read head' position to first entry
        virtual ulong get_dataset_length() = 0; // Get length of input dataset (used e.g. by postprocessor to divide post-processing workload via MPI)
        virtual PPIDpair get_current_point() = 0; // Get current rank/ptID pair (i.e. whatever get_next_point() last output)
        virtual ulong    get_current_index() = 0; // Get a linear index which corresponds to the current rank/ptID pair in the iterative sense
        virtual PPIDpair get_next_point() = 0; // Get next rank/ptID pair in data file
        virtual bool eoi() = 0; // Check if 'current point' is past the end of the data file (and thus invalid!)

        /// Printer-retrieve dispatch function. If a virtual function override exists for
        /// the retrieve type, info is passed on, otherwise the function call is resolved
        /// to a default function which raises an informative runtime error explaining
        /// that the type is not retrievable.
        ///
        /// Note; this is not quite enough because the entries in the printer output need
        /// not be one-to-one with the printed object, e.g. vectors currently go into 
        /// a series of indexed output slots. Hard for e.g. scanner plugins to know the
        /// label that they need to ask for...
        ///
        /// Perhaps return a map of results matching the label? Or vector? Perhaps both
        /// in different cases...
        ///
        /// Note, cannot overload based on return type, so need to use an "out" parameter
        /// In addition, there may not be a valid result printed for a given entry, so
        /// an extra output flag "out_valid" must be set to indicate whether the entry
        /// was marked invalid in the old output. Use the return value for this.
        template<typename T>
        bool retrieve(T& out, const std::string& label, const uint rank, const ulong pointID)
        {
          return _retrieve(out, label, rank, pointID);
        }

        /// Overload for 'retrieve' that uses the current point as the input for rank/pointID
        template<typename T>
        bool retrieve(T& out, const std::string& label)
        {
          PPIDpair pt = get_current_point();
          return retrieve(out, label, pt.rank, pt.pointID);
        }

        /// Retrieve and directly print data to new output
        bool retrieve_and_print(const std::string& label, BaseBasePrinter& printer, const uint rank, const ulong pointID)
        {
           /// Just use the same label for input and output
           return retrieve_and_print(label, label, printer, rank, pointID);
        }

        /// Retrieve and directly print data to new output, renaming the output to something new
        /// Implemented in BasePrinter where complete type info is available.
        virtual bool retrieve_and_print(const std::string& in_label, const std::string& out_label, BaseBasePrinter& printer, const uint rank, const ulong pointID) = 0;

        /// Overload for 'retrieve_and_print' that uses the current point as the input for rank/pointID
        bool retrieve_and_print(const std::string& label, BaseBasePrinter& printer)
        {
           /// Uses same label for input and output
           return retrieve_and_print(label, label, printer);
        }
    
        /// As above, but allows for different input/output labels
        bool retrieve_and_print(const std::string& in_label, const std::string& out_label, BaseBasePrinter& printer)
        {
          PPIDpair pt = get_current_point();
          return retrieve_and_print(in_label, out_label, printer, pt.rank, pt.pointID);
        }

        /// Get type information for a data entry, i.e. defines the C++ type which this should be
        /// retrieved as, not what it is necessarily literally stored as in the output.
        /// It isn't human readable, it is just for matching retrieved data to a print type,
        /// mainly for the 'retrieve_and_print' function.
        /// Needs to be implemented in each complete derived Reader class
        virtual std::size_t get_type(const std::string& label) = 0;
 
        /// Get list of output labels that can be retrieved by this printer. 
        /// Needs to be implemented in each complete derived Reader class
        virtual std::set<std::string> get_all_labels() = 0;
  
      protected:
        /// Default _retrieve function. Throws an error if no virtual
        /// function matching the type of the attempted retrieval is
        /// found.        
        template<typename T>
        bool _retrieve(T& out, const std::string& label, const uint rank, const ulong pointID)
        {
          std::ostringstream err;                               
                                                                
          err << "Attempted to retrieve a print result as an unregistered type!"
              << "If you really want to retrieve a result as this type, you must "
              << "add the type to the RETRIEVABLE_TYPES sequence "
              << "in \"gambit/Elements/printable_types.hpp\". You will then have "
              << "to define a retrieve function for it in whatever printer "
              << "you are using (see documentation for GAMBIT printers)."
              << "\n  Available info for this retrieve attempt..."                
              << "\n   Label      : " << label                  
              << "\n   Type       : " << STRINGIFY(T);       
          printer_error().raise(LOCAL_INFO,err.str());         
          return false;
        }
      
        // Virtual retrieval methods for base printer classes
        #define VRETRIEVE(r,data,elem)            \
        virtual bool _retrieve(elem& /*output*/,  \
                       const std::string& label,  \
                       const uint /*rank*/,       \
                       const ulong /*pointID*/    \
                       )                          \
        {                                                         \
          std::ostringstream err;                                 \
                                                                  \
          err << "No retrieve function override has been "           \
              << "\ndefined for this retrieval type (for whatever printer"  \
              << "\nclass the current printer comes from)"        \
              << "\n  Dumping available info..."                  \
              << "\n   Label      : " << label                    \
              << "\n   Type       : " << STRINGIFY(elem);         \
          printer_warning().raise(LOCAL_INFO,err.str());          \
          return false;                                           \
        }

        #define ADD_VIRTUAL_RETRIEVALS(TYPES) BOOST_PP_SEQ_FOR_EACH(VRETRIEVE, _, TYPES)

        // Add the base virtual functions for registered printable and
        // retrievable types, to be overloaded in each printer.
        ADD_VIRTUAL_RETRIEVALS(SCANNER_RETRIEVABLE_TYPES) 
    };
 
  } //end namespace Printers
} // end namespace Gambit


#endif //ifndef __base_base_printer_hpp__
