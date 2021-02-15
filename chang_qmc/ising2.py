import bitarray
import numpy as np
import prettytable

class SpinGlass(object):
    """An Ising Spin Glass Configuration"""
    
    def __init__(self,
                 data_file="test.txt",
                 spin_configuration=None):
        """Process a data file and optionally a starting spin configuration
        
        data_file has K+1 columns and some string in the first line
        - The first K columns are positive integer spin IDs
        - The last column is the coupling between the spins
        - Solver is at present designed to work with K = 2 only
        
        spin_configuration is a value in hex that represents the spins using
        binary 1 (spin up) and 0 (spin down), packed into hex for shorthand
        readability
        """
        
        # store argument values
        self.data_file = data_file
        self.spin_configuration = spin_configuration
        
        # declare problem data
        self.description = None
        self.size = None
        self.scaling_factor = 1
        
        # declare soon-to-be numpy arrays
        # these will be made non-writable
        #self.J = None
        #self.h = None
        #self.adjacency = None
        self.spins_initial = None
        
        # populate problem data
        data = self._process_data_file()
        self._process_data(data)

        # process an input hex string configuration, or randomize
        if spin_configuration:
            self.spins_initial = self.hex_to_spins(spin_configuration)
        else:
            self.randomize()
        
        # create working copies
        # these values are intended to be accessed and modified by a solver
        self.spins = self.spins_initial.copy()
        
        # calculate initial energy
        self.E_initial_biased = self.calculate_E()
        self.E_initial = self.E_initial_biased - self.sum
        self.E = self.E_initial_biased
    
    
    def _process_data_file(self):
        """Read data_file, set description and return array"""

        with open(self.data_file, 'r') as f:
            self.description = f.readline().strip()
            list = self.description.split(' ')
            self.n = int(list[0])
            self.size = self.n
            self.m = int(list[1])
            self.sum = float(list[2])
        data = np.loadtxt(self.data_file, skiprows=1)
        return data
        
        
    def _process_data(self, data):
        """data array into usable data structures
        
        Create empty spin array
        Create structures for couplings between spins
        Create adjacency list for spins and neighbours
        """
        # save graph's information
        self.edges = []
        for i in xrange(self.n):
            self.edges.append([])

        self.data = data            
        for i in xrange(self.m):
            e = data[i]
            self.edges[int(e[0]) - 1].append((int(e[1]) - 1, e[2]))
            self.edges[int(e[1]) - 1].append((int(e[0]) - 1, e[2]))


    def calculate_E(self):
        """By self.edges, Calculates the energy of the classical spin configuration

        This isn't done often, so optimizing for speed isn't important
        """

        E = 0
        for i in xrange(self.n):
            Ei = 0
            Ei += 0.5*sum((1 if self.spins[j] else -1)* w for (j,w) in self.edges[i])
            if not self.spins[i]:
                Ei *= -1
            E += Ei

        return E

    def calculate_dE(self, i):
        """By self.edges, Calculate the difference in energy from flipping a single spin i"""

        # h is interactions between spins and magnetic field. We do not use this value, so we ommit this factor calculating dE.
        #dE = self.h[i]
        dE = 0
        dE += sum((1 if self.spins[j] else -1)*w for (j,w) in self.edges[i])
        if not self.spins[i]:
            dE *= -1

        return -2*dE
        
    
    def hex_to_spins(self, hex_spins):
        """Convert a hex string to a big endian binary array
        
        This function takes user input, so we'll be careful
        
        hex_spins: hex representation of spin configuration
        - allows, but doesn't require, a leading "0x"
        """
        
        # purely alphanumeric strings only
        assert(hex_spins.isalnum())
        
        binary = '{:0{}b}'.format(int(hex_spins,16), self.size)
        spins = bitarray.bitarray(binary)

        return spins
        

    def spins_to_hex(self, spins=None):
        """Return a hex string representation of the spin configuration bitarray"""
        
        if spins:
            spins = bitarray.bitarray(spins)
        else:
            spins = self.spins
        hex_spins = "0x{:x}".format(int(spins.to01(),2))
        
        return hex_spins
    
    
    def randomize(self):
        """Set self.spins_initial to a random configuration"""
        
        spins = [np.random.random() > 0.5 for x in range(self.size)]
        self.spins_initial = bitarray.bitarray(spins)
    
    def get_spin(self, i):
        """Return spin as +/- 1"""
        
        return 1 if self.spins[i] else -1
    
    
    def flip_spin(self, i):
        """Flip a spin between up and down states"""
        
        self.spins[i] ^= True
    
    
    def __repr__(self):
        
        return '{}(data_file="{}", spin_configuration="{}")'.format(self.__class__, self.data_file, self.spins_to_hex(self.spins))
        
        
    def __str__(self):
        
        ret = "\nIsing Spin Glass\n"
        x = prettytable.PrettyTable(["Field", "Value"])
        x.align["Field"] = "l"
        x.align["Value"] = "l"
        x.add_row(["data file", self.data_file])
        x.add_row(["description", self.description])
        x.add_row(["initial config", self.spins_to_hex(self.spins_initial)])
        x.add_row(["current config", self.spins_to_hex(self.spins)])
        x.add_row(["initial energy", self.E_initial*self.scaling_factor])
        x.add_row(["current energy", self.E*self.scaling_factor])
        
        return ret + x.get_string()


if __name__ == "__main__":
    test = SpinGlass()
    print test
