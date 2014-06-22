//Abstract class for accessing general spectrum information.

#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "Phys.hpp"
#include <map>

class Spectrum : public scaledep::RunningPars, public Phys {
public:
   // returns the lightest stable particle (lsp) mass 
   //  gives 3 integers to specify the state 
   // for most general case of a particle type with mass matrix 
   // row and col set to -1 when not needed 
   //(row only is used for vector)
   //particle_type = 0 (neutralino), 1(Sneutrino), 2(up squark), 
   //3(down squarks), 4(charged slepton), 5(Chargino), 6(gluino)
   // Add more for 
   virtual double get_lsp_mass(int & particle_type, int & row, int & col) const = 0;
   //There may be more than one *new* stable particle
   // this method will tell you how many.
   //If more than zero you probbaly *need* to know what model
   // you are working on, so we don't give all stable particles
   virtual int get_numbers_stable_particles() const = 0;  
   
};

// If we were allowed to use later C++11 compilers we could use template aliases to save some effort, but as
// it is we'll just have to redo these typedefs in the derived classes. Can do this with a macro.
#define REDO_TYPEDEFS(SpecType) \
   typedef double(SpecType::*FSptr)(void) const; /* Function pointer signature for FlexiSUSY class member functions */ \
   typedef std::map<std::string, FSptr> fmap; /* Typedef for map of strings to function pointers              */

// Need the templating so that the calls to the FlexiSUSY functions know which FlexiSUSY class to use
template <class SpecType>
class Spec : public Spectrum
{
   REDO_TYPEDEFS(SpecType)
  
   private:
      // Need to implement these two functions in each derived class, but they are trivial. Maybe there is some way to
      // avoid having to do this step, but I can't think of it just now.
      virtual fmap& get_mass2_map() const = 0;  
      virtual SpecType& get_bound_spec() const = 0; 

   public:
      virtual double get_mass2_par(std::string) const;
};


// Have to re-write these two functions for each derived class, so that reference to the correct member variables is retrieved.
// Need these functions though so that the original definition of get_mass2_par can be re-used.
// Maybe do this with another macro...
#define REDEFINE_TRIVIAL_MEMBER_FUNCTIONS(ClassName,SpecType) \
  ClassName::fmap& ClassName::get_mass2_map() const {return mass2_map;} \
  SpecType&       ClassName::get_bound_spec() const {return model;} \
  ClassName::fmap  ClassName::mass2_map(ClassName::fill_mass2_map());

// Should now never have to override this I think
template <class SpecType>
double Spec<SpecType>::get_mass2_par(std::string mass) const
{
   SpecType spec(get_bound_spec()); // Get correct bound spectrum for whatever class this is
   fmap& mass2map(get_mass2_map()); // Get correct map for whatever class this is
   
   typename fmap::iterator it = mass2map.find(mass); // Find desired FlexiSUSY function

   if( it==mass2map.end() )
   {
      std::cout << "No mass2 with string reference '"<<mass<<"' exists!" <<std::endl;
      return -1;
   }
   else
   {
       // Get function out of map and call it on the bound flexiSUSY object
       FSptr f = it->second;
       return (spec.*f)();
   }
}


#endif
