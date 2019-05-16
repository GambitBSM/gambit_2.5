"""
Master module for all Models related routines.
"""

import numpy as np
import re

from setup import *
from files import *

def add_to_model_hierarchy(spectrum_name, model_name, model_params):
    """
    Adds a model to the model hierarchy. This means we create any
    new header files in the model directory, i.e.
    Models/include/gambit/Models/models/<new_model>.hpp, and edit any
    parent/children headers. Writes translation functions etc. in
    Models/src/models/<new_model>.cpp if needed.
    """

    print("Writing new spectrum, {0}".format(spectrum_name))

    towrite_header = blame_gum("/// Header file for {0}".format(model_name))
    towrite_header += (
                   "#ifndef __{0}_hpp__\n"
                   "#define __{0}_hpp__\n"
                   "\n"
    ).format(model_name)
    towrite_source = blame_gum("/// Source file for {0}".format(model_name))

    module = "Models"

    towrite_header += (
    			   "#define MODEL {0}"
    			   "\n"
    			   "  START_MODEL\n"
    			   "\n"

    			   ).format(model_name)

    # Don't want the SM-like Higgs mass a fundamental parameter
    bsm_params = [x for x in model_params if x.name != 'h0_1' and x.sm == False]

    params = []

    for i in bsm_params:
        if i.shape == 'scalar' or i.shape == None: params.append(i.gb_in)
        elif re.match("m[2-9]x[2-9]", i.shape): 
            size = int(i.shape[-1])
            for j in xrange(size):
                for k in xrange(size):
                    params.append(i.gb_in + str(j+1) + 'x' + str(k+1))
        elif re.match("v[2-9]", i.shape): 
            size = int(i.shape[-1])
            for j in xrange(size):
                params.append(i.gb_in + str(j+1))

    # No double counting (also want to preserve the order)
    norepeats = []
    [norepeats.append(i) for i in params if not i in norepeats]


    # Chunk this up into groups of no more than 9, so the DEFINEPARS macro works.
    definepars = [norepeats[i:i+9] for i in range(0,len(norepeats),9)]

    for i in range(len(definepars)):
        towrite_header += "  DEFINEPARS({0})\n".format(', '.join(definepars[i]))

    towrite_header += (
                   "\n"
                   "#undef MODEL\n"
                   "\n"
                   "#endif\n"
    )

    return towrite_header

def find_parents_params(parent):
    """
    Returns all params in the parent model.
    """

    module = "Models"
    fname = "models/" + parent + ".hpp"
    location = full_filename(fname, module)

    lookup = "#define MODEL " + parent
    term = "#undef MODEL"

    num = find_string(fname, module, lookup)[1]

    parent_params = []

    with open(location, 'r') as f:
        for num, line in enumerate(f, 1+num):
            if "DEFINEPARS" in line:
                params = re.compile( "\((.*)\)" ).search(line).group(1)
                parent_params += params.split(",")
            if term in line:
                break

    return parent_params

def find_tree_root(parent):
    """
    Traces up a model tree to find the 'root' of the model tree,
    i.e. which existing Spectrum a new model is allowed to use.
    """

    module = "Models"
    fname = "models/" + newmodel + ".hpp"

    newmodel = parent
    root = False
    lookup = "#define PARENT "

    while root == False:
        location = full_filename(fname, module)
        if lookup in open(location, 'r').read():
            lines = open(location, 'r').readlines()
            for line in lines:
              if lookup in line:
                newmodel = line.split(' ')[-1].strip('\n')
        else:
            root = True

    return newmodel

def write_spectrumcontents(gambit_model_name, model_parameters):
    """
    Writes SpectrumContents for a new model:
    Models/src/SpectrumContents/<gambit_model>.cpp.
    """

    intro = (
          "///  Class defining the parameters that SubSpectrum\n"
          "///  objects providing " + gambit_model_name + "\n"
          "///  spectrum data must provide."
    )

    towrite = blame_gum(intro)

    towrite += (
            "#ifndef __{0}_contents_hpp__\n"
            "#define __{0}_contents_hpp__\n\n"
            "#include \"gambit/Models/SpectrumContents/RegisteredSpectra.hpp\""
            "\n"
            "\n"
            "namespace Gambit\n"
            "{{\n"
            "SpectrumContents::{0}::{0}()\n"
            "{{\n"
            "setName(\"{0}\");\n"
            "\n"
            "std::vector<int> scalar = initVector(1);"
            " // i.e. get(Par::Tag, \"name\")\n"
            "std::vector<int> v2     = initVector(2);"
            " // i.e. get(Par::Tag, \"name\", i)\n"
            "std::vector<int> v3     = initVector(3);   // \"\n"
            "std::vector<int> v4     = initVector(4);   // \"\n"
            "std::vector<int> v6     = initVector(6);   // \"\n"
            "std::vector<int> m2x2   = initVector(2,2);"
            " // i.e. get(Par::Tag, \"name\", i, j)\n"
            "std::vector<int> m3x3   = initVector(3,3); // \"\n"
            "std::vector<int> m4x4   = initVector(4,4); // \"\n"
            "std::vector<int> m6x6   = initVector(6,6); // \"\n"
            "\n"
    ).format(gambit_model_name)

    # Now add each parameter to the model file.
    for i in np.arange(len(model_parameters)):
        if not isinstance(model_parameters[i], SpectrumParameter):
            raise GumError(("\n\nModel Parameters at position " + i +
                            "not passed as instance of class "
                            "SpectrumParameter."))

        mp = model_parameters[i]

        # Default shape to 'scalar'
        if mp.shape:
            shape = mp.shape
        else:
            shape = "scalar"

        # If we've extracted some information about the block, then
        # add the information to the addParameter macro
        if mp.block:
            extra = ", \"" + mp.block + "\", " + str(mp.index)
        else: extra = ""

        # Write the addParameter macro to initialise each SpectrumParameter
        # object within the SubSpectrum.
        towrite += (
                "addParameter(Par::{0}, \"{1}\", {2}{3});\n"
                ).format(mp.tag.replace("\"",""), 
                         mp.name, shape, extra)


    towrite += (
            "\n"
            "} // namespace Models\n"
            "} // namespace Gambit\n"
            "#endif\n"
    )

    contents = indent(towrite)
    return contents

def write_subspectrum_wrapper(gambit_model_name, model_parameters):
    """
    Writes spectrum object wrapper for new model:
    Models/include/gambit/Models/SimpleSpectra/<new_model_name>SimpleSpec.hpp.
    """

    # Classes make life easier
    class SpecGetAndSet:

        def __init__(self, shape, size, param, getter, setter):
            self.shape = shape
            self.size = size
            self.param = param
            self.getter = getter
            self.setter = setter

    spectrumparameters = []

    modelSS = gambit_model_name + "SimpleSpec"
    modelclass = gambit_model_name + "Model"

    # Go through model, and create down all members of the model object,
    # all getter functions, all setter functions, all sizes...

    for i in np.arange(len(model_parameters)):
        if not isinstance(model_parameters[i], SpectrumParameter):
            raise GumError(("\n\nModel Parameters at position " + i +
                            " not passed as instance of class "
                            "SpectrumParameter."))

        if model_parameters[i].sm:
        	e = ""
        else:
        	e = gambit_model_name + "_"

        paramname = e + model_parameters[i].fullname

        if model_parameters[i].tag == "Pole_Mass":
            paramname += "_Pole_Mass"

        shape = "scalar"
        size = 1

        if model_parameters[i].shape:
            if re.match("v[2-9]", model_parameters[i].shape):
                shape = "vector"
                size = model_parameters[i].shape[-1]
            elif re.match("m[2-9]x[2-9]", model_parameters[i].shape):
                # Assuming all matrices will be square...
                shape = "matrix"
                size = model_parameters[i].shape[-1]

        getter = "get_" + model_parameters[i].fullname

        if model_parameters[i].tag == "Pole_Mass":
            getter += "PoleMass"

        setter = "set_" + model_parameters[i].fullname

        if model_parameters[i].tag == "Pole_Mass":
            setter += "PoleMass"

        x = SpecGetAndSet(shape, size, paramname, getter, setter)
        spectrumparameters.append(x)

    intro_message = (
            "///  A simple SubSpectrum wrapper for\n"
            "///  " + gambit_model_name + ". No RGEs included."
    )

    towrite = blame_gum(intro_message)

    towrite += (
            "#ifndef __{0}_hpp__\n"
            "#define __{0}_hpp__\n"
            "\n"
            "#include \"gambit/Elements/spec.hpp\"\n"
            "#include \"gambit/Models/SpectrumContents/"
            "RegisteredSpectra.hpp\"\n"
            "\n"
            "namespace Gambit\n"
            "{{\n"
            "namespace Models\n"
            "{{\n"
            "/// Simple {1} model object.\n"
            "struct {2}\n"
            "{{\n"
    ).format(modelSS, gambit_model_name, modelclass)

    # Now add each parameter to the model file.

    for i in range(0, len(spectrumparameters)):

        sp = spectrumparameters[i]

        if sp.shape == "scalar":
            size = ""
        elif sp.shape == "vector":
            size = "[{0}]".format(sp.size)
        elif sp.shape == "matrix":
            size = "[{0}][{0}]".format(sp.size)

        towrite += "double {0}{1};\n".format(sp.param, size)

    towrite += (
            "}};\n"
            "\n"
            "/// Forward declare the wrapper class so that we can use it\n"
            "/// as the template parameter for the SpecTraits specialisation.\n"
            "class {0};\n"
            "}}"
            "\n"
            "\n"
            "/// Specialisation of traits class needed to inform "
            "base spectrum class of the Model and Input types\n"
            "template <> \n"
            "struct SpecTraits<Models::{0}> : DefaultTraits\n"
            "{{\n"
            "static std::string name() {{ return \"{0}\"; }}\n"
            "typedef SpectrumContents::{1} Contents;\n"
            "}};\n"
            "\n"
            "namespace Models\n"
            "{{\n"
            "class {0} : public Spec<{0}>\n"
            "{{\n"
            "private:\n {2}  params;\n"
            "typedef {0} Self;\n"
            "\n"
            "public:\n"
            "/// Constructors & destructors\n"
            "{0}(const {2}& p)\n"
            " : params(p)\n"
            "{{}}\n"
            "\n"
            "static int index_offset() {{return -1;}}\n"
            "\n"
            "/// Construct the SubSpectrumContents\n"
            "const SpectrumContents::{1} contents;\n"
            "\n"
            "/// Add SLHAea object using the SimpleSpec_to_SLHAea routine\n"
            "void add_to_SLHAea(int /*slha_version*/, SLHAea::Coll& slha) const\n"
            "{{\n"
            "// Add SPINFO data if not already present\n"
            "SLHAea_add_GAMBIT_SPINFO(slha);\n"
            "\n"
            "// All blocks given in the SimpleSpec\n"
            "\nadd_SimpleSpec_to_SLHAea(*this, slha, contents);\n"
            "}}\n"
            "\n" 
            "/// Wrapper functions to parameter object.\n"
    ).format(modelSS, gambit_model_name, modelclass)

    # Would be neater (here) to write get_x and set_x at the same time,
    # but following current format...

    # Getter functions
    for i in np.arange(len(spectrumparameters)):

        sp = spectrumparameters[i]

        if sp.shape == "scalar":
            size = ""
            indices = ""
        if sp.shape == "vector":
            size = "int i"
            indices = "[i]"
        elif sp.shape == "matrix":
            size = "int i, int j"
            indices = "[i][j]"

        towrite += "double {0}({1}) const {{ return params.{2}{3}; }}\n".format(sp.getter, size, sp.param, indices)

    towrite += "\n"

    # Setter functions
    for i in np.arange(len(spectrumparameters)):

        sp = spectrumparameters[i]

        if sp.shape == "scalar":
            size = ""
            indices = ""
        if sp.shape == "vector":
            size = ", int i"
            indices = "[i]"
        elif sp.shape == "matrix":
            size = ", int i, int j"
            indices = "[i][j]"

        towrite += "void {0}(double in{1}) {{ params.{2}{3}=in; }}\n".format(sp.setter, size, sp.param, indices)

    towrite += (
            "\n"
            "/// Map fillers\n"
            "static GetterMaps fill_getter_maps()\n"
            "{\n"
            "GetterMaps getters;\n"
            "\n"
    )

    # Add necessary function pointer maps.
    v = False
    m = False
    sizes = []
    sizes = []
    for i in range(0, len(spectrumparameters)):
        sp = spectrumparameters[i]
        if sp.shape == "vector":
            v = True
            sizes.append(sp.size)
        elif sp.shape == "matrix":
            m = True
            sizes.append(sp.size)

    if m:
        towrite += "typedef typename MTget::FInfo2W FInfo2W;\n"
    if v:
        towrite += "typedef typename MTget::FInfo1W FInfo1W;\n"

    # Remove all duplicates. These values tell us which indices we need to
    # include for the FInfo routines.
    sizes = list(set(sizes))

    for i in np.arange(len(sizes)):
        fnname = "i" + "".join(str(j) for j in np.arange(int(sizes[i])))

        towrite += (
                "static const int {0}v[] = {{{1}}};\n"
                "static const std::set<int> {0}({0}v, Utils::endA({0}v));"
                "\n"
        ).format(fnname, ",".join(str(j) for j in np.arange(int(sizes[i]))))

    towrite += "\nusing namespace Par;\n\n"

    # Now add getter maps
    for i in np.arange(len(model_parameters)):
        sp = spectrumparameters[i]
        mp = model_parameters[i]

        if sp.shape == "scalar":
            size = "0"
            finf = " &Self::{}".format(sp.getter)
        elif sp.shape == "vector":
            size = "1"
            index = "i" + "".join(str(j) for j in np.arange(int(sp.size)))
            finf = "FInfo1W(&Self::{0}, {1})".format(sp.getter, index)
        elif sp.shape == "matrix":
            size = "2"
            index = "i" + "".join(str(j) for j in np.arange(int(sp.size)))
            finf = "FInfo2W(&Self::{0}, {1}, {1})".format(sp.getter, index)

        towrite += (
                "getters[{0}].map{1}"
                "W[\"{2}\"] = {3};\n"
        ).format(mp.tag, size, mp.name, finf)

    towrite += (
            "\n"
            "return getters;\n"
            "}\n"
            "\n"
            "static SetterMaps fill_setter_maps()\n"
            "{\n"
            "SetterMaps setters;\n"
            "\n"
    )

    if m:
        towrite += "typedef typename MTset::FInfo2W FInfo2W;\n"
    if v:
        towrite += "typedef typename MTset::FInfo1W FInfo1W;\n"

    # Remove all duplicates. These values tell us which indices we need to
    # include for the FInfo routines.
    sizes = list(set(sizes))

    for i in np.arange(len(sizes)):
        fnname = "i" + "".join(str(j) for j in np.arange(int(sizes[i])))

        towrite += (
                "static const int {0}v[] = {{{1}}};\n"
                "static const std::set<int> {0}({0}v, Utils::endA({0}v));"
                "\n"
        ).format(fnname, ",".join(str(j) for j in np.arange(int(sizes[i]))))

    towrite += "\nusing namespace Par;\n\n"

    for i in range(0, len(model_parameters)):
        sp = spectrumparameters[i]
        mp = model_parameters[i]

        if sp.shape == "scalar":
            size = "0"
            finf = " &Self::{}".format(sp.setter)
        elif sp.shape == "vector":
            size = "1"
            index = "i" + "".join(str(j) for j in np.arange(int(sp.size)))
            finf = "FInfo1W(&Self::{0}, {1})".format(sp.setter, index)
        elif sp.shape == "matrix":
            size = "2"
            index = "i" + "".join(str(j) for j in np.arange(int(sp.size)))
            finf = "FInfo2W(&Self::{0}, {1}, {1})".format(sp.setter, index)

        towrite += (
                "setters[{0}].map{1}"
                "W[\"{2}\"] = {3};\n"
        ).format(mp.tag, size, mp.name, finf)

    towrite += (
            "\n"
            "return setters;\n"
            "}\n"
            "};\n"
            "}\n"
            "} // namespace Gambit\n"
            "#endif\n"
    )

    contents = indent(towrite)
    return contents

def add_to_registered_spectra(gambit_model):
    """
    Adds new model entry to RegisteredSpectra.hpp
    """

    lookup = "SubSpectrumContents"
    newentry = "    struct {0:21}: SubSpectrumContents {{ {0}(); }};\n".format(gambit_model)
    filename = "SpectrumContents/RegisteredSpectra.hpp"
    module = "Models"
    location = full_filename(filename, module)
    linenum = 0 # Position of last entry in RegisteredSpectra
    with open(location) as f:
        for num, line in enumerate(f, 1):
            if lookup in line:
                linenum = num
            if newentry in line:
                raise GumError(("\n\nModel {0} already exists in GAMBIT.").format(gambit_model))

    return newentry, linenum

def add_masses_to_params(parameters, bsm_particle_list, gambit_pdgs, add_higgs):
    """
    Adds the pole masses to the list of parameters. 
    If the parameter name exists already, it is removed.
    Double counting is known to occur in the following circumstances:

      1) FeynRules: a shared tree-level mass term for a multiplet.

    """

    parameters_by_name = [x.name for x in parameters]

    for i in xrange(len(bsm_particle_list)):
        p = bsm_particle_list[i]

        # Check to see if the parameter name is in the list of model parameters currently.
        # If it is, remove it
        if p.mass in parameters_by_name:
            for i, o in enumerate(parameters):
                if o.name == p.mass: 
                    del parameters[i]
                    break

        # Overwrite the parameter name for the Higgs mass, to match the name within GAMBIT, 
        # if this is a 1HDM.
        if p.PDG_code == 25: 
            if add_higgs:
                p.mass = "mH"

        # Add the new parameter to the list of model parameters.
        x = SpectrumParameter(pdg_to_particle(p.PDG_code, gambit_pdgs),
                              "Pole_Mass", gb_input=p.mass)
        parameters.append(x)

    return parameters