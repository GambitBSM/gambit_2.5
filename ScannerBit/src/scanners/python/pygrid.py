import scannerbit
import scanner_pyplugin as splug
import numpy as np

# Figure out if we can use mpi4py
try:
    from mpi4py import MPI
    if scannerbit.with_mpi:
        with_mpi = True
    else:
        with_mpi = False
except ImportError:
    if scannerbit.with_mpi:
        if scannerbit.numtasks() > 1:
            raise Exception(f"GAMBIT is compiled with MPI parallelisation enabled (WITH_MPI=1), "
                            f"but {__file__} failed to import the python module mpi4py. "
                            f"If you want to run this scanner with multiple MPI processes, "
                            f"please install mpi4py.")
        else:
            print(f"WARNING: GAMBIT is compiled with MPI parallelisation enabled (WITH_MPI=1), "
                  f"but {__file__} failed to import the python module mpi4py. If you want to "
                  f"run this scanner with multiple MPI processes, please install mpi4py.")
    else:
        print(f"WARNING: The scanner plugin failed to import mpi4py in {__file__}, "
              f"but that's OK since GAMBIT anyway is running in serial mode (-DWITH_MPI=0).", flush=True)
    with_mpi = False

class scanner_plugin:
    """
Python implimentation of a simple grid scanner.  Evaluation points along a user-defined grid.

YAML options:
    grid_pts[req'd]: The number of points along each dimension on the grid.  A vector is given with each element corresponding to each dimension.
    like:            Use the functors thats corresponds to the specified purpose.
    parameters:      Specifies the order of parameters that corresponds to the grid points specified by the tag "grid_pts".
"""

    __version__="1.0.0"
    
    def __init__(self, **options):
        
        if with_mpi:
            comm = MPI.COMM_WORLD
            self.numtasks = comm.Get_size()
            self.rank = comm.Get_rank()
        else:
            self.numtasks = 1
            self.rank = 0
        
        # gets dimension of hyper cube.
        self.dim = splug.get_dimension()
        
        # gets inifile value corresponding to "like" key
        purpose = options["like"]
        
        # or alternatively you can access the inifile options directly by:
        # purpose = splug.get_inifile_value("like", dtype=str)
        
        # gets likelihood corresponding to the purpose "purpose"
        self.like = splug.get_purpose(purpose)
        
        # get grids pt number from inifile. Return a list of ints
        N = options["grid_pts"]
        
        # or alternatively you can access the inifile options directly by:
        # N = splug.get_inifile_value("grid_pts", dtype=list, etype=int)
        
        if len(N) != self.dim:
            raise Exception("Grid scanner: The dimension of gambit ({0}) does not match the dimension of the inputed grid_pts ({1}).".format(self.dim, len(N)))
        
        self.size = np.prod(np.asarray(N))
        
        # gets "parameters" infile value with a default of []. Returns a list of strings
        user_params = splug.get_inifile_value("parameters", dtype=list, etype=str, default=list());
        
        self.vecs=[]

        # prepare the grid of points
        if len(user_params) > 0:
            
            # get the parameters names from the prior
            param_names = splug.get_prior().getShownParameters()
            
            if len(param_names) != len(user_params):
                raise Exception("Grid scanner: The dimension of gambit ({0}) does not match the dimension of the inputed parameters ({1}).".format(len(param_names), len(user_params)))
            
            for param in param_names:
                if param in user_params:
                    self.vecs.append(np.linspace(0.0, 1.0, N[user_params.index(param)]))
                else:
                    raise Exception("Grid scanner: parameter \"{0}\" is not provided.".format(param))

        else:
            for n in N:
                self.vecs.append(np.linspace(0.0, 1.0, n))
            
    def plugin_main(self):

        # scan the grid of points, divided evenly across the numper of MPI processes
        for pt in np.vstack(np.meshgrid(*self.vecs)).reshape(self.dim, -1).T[self.rank:self.size:self.numtasks]:

            # run likelihood
            self.like(pt)
            
            # get pt id
            id = self.like.getPtID()
            
            # prints value
            splug.get_printer().get_stream().print(1.0, "mult", self.rank, id)
        
        return 0
    
    
