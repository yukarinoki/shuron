import copy
import numpy as np
import time

import monte_carlo

class PathIntegralQMC(monte_carlo.MonteCarloSolver):
    """Search for global energy minimum with PI-QMC
    
    Begins with a wide search across several duplicate slices.
    Strong transverse field (Gamma) encourages moves despite fixed temperature.
    As tranverse field weakens, this quantum superposition begins to collapse
    represented by a strengthening interslice coupling J_perp.
    Moves in single slices, and global moves across all slices are attempted
    and a basic Metropolis-Hastings approach is used to evaluate them.
    """
    
    def __init__(self, problem, params):
        
        super(PathIntegralQMC, self).__init__(problem, params)

        self.solver_name = "Path-Integral Quantum Monte Carlo"
        self.P = params['P']
        # effective temperature
        self.PT = self.P*params['T']
        
        self.steps = params['steps']
        #self.s = params['s']
        self.g0 = params['G0']
        self.gf = params['Gf']
        self.step = params['step']
        self.s = np.power(self.g0/self.gf, 1.0/self.step) - 1
        #print "{}\t{}".format(t,self.s)
        #sys.exit()
        
        # copy problem objects into every slice
        # there is some replication of storage here (J matrix, etc.)
        # but it allows using the class methods without passing extra data
        # back and forth
        self.slices = []
        for x in range(self.P):
            self.slices.append(copy.deepcopy(problem))


    def solve(self):
        """Solve the problem and return the solution energy and configuration"""

        if self.outfile:
            fp = open(self.outfile, 'w')
            config = self.p.spins_to_hex() + ",-1"
            fp.write("{}\n{}\n{}\n".format(self.solver_name, self.params, repr(self.p)))
            self._state_dump(fp, self.params["G0"], self.p.E, config, 0, 0)

        #a = 1.0/self.g0
        a = self.g0
        t = a/float(self.step)
        print "{}, {}, {}".format(a,t, self.step)
        
        fixed = float(self.slices[0].description.split(" ")[2])
        #beta_f = 1.0/self.gf
        beta_f= self.gf
        print "{}".format(beta_f)
        counter = 0
        update = 0
        obj = 0
        b_flag = 0
        best = np.inf

        spin_log = int(self.step/15000)
        
        start = time.time()
        while a > beta_f:
        # try local moves in all slices, in random order
            counter += 1
            #a *= (1 + self.s)
            #G = 1/a
            G = a
            Jp = -0.5*self.PT*np.log(np.tanh(1.0/(a*self.PT)))
            slices = np.random.permutation(self.P)
            for k in slices:
                # choose a random spin
                i = np.random.randint(0, self.p.size)
                
                # get local energy cost
                dE = self.slices[k].calculate_dE(i)
                
                # get interslice energy cost
                dE_i = self.calculate_dE_interslice(k, i, Jp)
                if self.step_accepted(dE + dE_i, self.PT):
                    self.slices[k].flip_spin(i)
                    self.slices[k].E += dE

            dE_global = np.zeros(self.P)
            # try a global move on a random spin
            # Jp doesn't impact global moves
            i = np.random.randint(0, self.p.size)
            for k in range(self.P):
                dE_global[k] = self.slices[k].calculate_dE(i)
            if self.step_accepted(np.sum(dE_global), self.PT):
                for k in range(self.P):
                    self.slices[k].flip_spin(i)
                    self.slices[k].E += dE_global[k]

            # minimum energy inserts into obj and output count, obj and update
            # compare best with obj and do update++
            # updated annealed heat outputs into other file
            energies = [x.E for x in self.slices]
            obj = np.min(energies)
            x = np.argmin(energies)
            if obj < best:
                b_flag = 1
                best = obj
                update += 1
                if self.outfile:
                    config = self.slices[x].spins_to_hex() + ",{}".format(x)
                    self._state_dump(fp, update, counter, best - fixed, G, config)
            if b_flag == 1 or counter % spin_log == 0:
                config = self.slices[x].spins_to_hex() + ",{}".format(x)
                print "{}\t{}\t{}\t{}\t{}".format(counter, obj - fixed, update, config, a)
                #print "{}\t{}\t{}".format(counter, obj - fixed, update)
            b_flag = 0
            a -= t
        end = time.time()
        elapsed_time = end - start
        print "Annealed obj updated"
        print "Elapsed_time: {}".format(elapsed_time)
        print "stride is {}".format(self.s)

        if self.outfile:
            fp.close()
        
        # Get the best from all slices and store it in the original problem object
        energies = [x.E for x in self.slices]
        best = np.argmin(energies)
        self.p.E = energies[best] - fixed
        self.p.spins = self.slices[best].spins.copy()
        self.count = counter
        print "steps: {}".format(counter)
        print "Linear Schedule\n"
        
        return self.p.E - fixed, self.p.spins_to_hex(), self.count
        
    
    def calculate_dE_interslice(self, k, i, Jp):
        """Calculate the interslice energy of a spin flip and apply periodic
        boundary conditions between replicas
        
        k: the slice index
        i: the spin index
        Jp: the current interslice coupling strength
        """
        
        # dE = -2*spin*Jp(spin_left + spin_right)
        dE = -2*Jp*self.slices[k].get_spin(i)
        
        # find neighbours and apply boundary conditions
        left = k - 1
        right = k + 1 if k + 1 < self.P else 0
        
        return dE*(self.slices[left].get_spin(i) + self.slices[right].get_spin(i))
