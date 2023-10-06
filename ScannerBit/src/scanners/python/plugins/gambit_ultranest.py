"""
Ultranest scanners
==================
"""

import pickle
import ultranest
from mpi4py import MPI

import scanner_plugin as splug
from utils import copydoc, version

class ReactiveUltranest(splug.scanner):
    """
    Ultranest reactive sampler.
    """

    name = "reactive_ultranest"
    __version__ = version(ultranest)
    
    def ultra_transform(self, cube):
        #print("transform = ", cube, self.mpi_rank)
        
        return self.transform_to_vec(cube)
    
    def ultra_like(self, cube):
        lnew = self.loglike(cube)
        self.saves[tuple(cube)] = (self.mpi_rank, self.point_id)
        #print("like = ", cube, self.mpi_rank)
        return lnew
    
    def transfer(self):
        comm = MPI.COMM_WORLD
        
        if self.mpi_rank == 0:
            for i in range(1, self.mpi_size):
                data = comm.recv(source=i, tag=1)
                self.saves.update(data)
        else:
            comm.send(self.saves, dest=0, tag=1)
    
    @copydoc(ultranest.ReactiveNestedSampler)
    def __init__(self, log_dir="ultranest_log_dir", **kwargs):
        """
        To ensure results are saved, by default we set the argument

        :param: log_dir ('reactive_ultranest_run')
        """
        super().__init__()
        self.saves = {}
        self.sampler = ultranest.ReactiveNestedSampler(
            self.parameter_names,
            self.ultra_like,
            transform=self.ultra_transform,
            log_dir=log_dir,
            **self.init_args)

    
    def run_internal(self, pkl_name="ultranest.pkl", **kwargs):
        """
        We add the argument

        :param: pkl_name ('ultranest.pkl')

        to store the results from the sampler to a pickle. This helps inspect
        results outside gambit.
        """
        
        self.sampler.run(**kwargs)
        self.transfer()
        
        if self.mpi_rank == 0:
            result = self.sampler.results
            wts = result["weighted_samples"]["weights"]
            upts = result["weighted_samples"]["upoints"]
            
            for wt, upt in zip(wts, upts):
                if tuple(upt) in self.saves:
                    save = self.saves[tuple(upt)]
                    self.print(wt, "Posterior", save[0], save[1])
                else:
                    print("warning: point has no correponding id.")
                        
            with open(pkl_name, "wb") as f:
                pickle.dump(result, f)
    
    @copydoc(ultranest.ReactiveNestedSampler.run)
    def run(self):
        self.run_internal(**self.run_args)


__plugins__ = {ReactiveUltranest.name: ReactiveUltranest}
