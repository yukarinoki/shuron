import copy
#import datetime
import numpy as np
#from collections import deque
import time
import sys

import monte_carlo

class SimulatedAnnealing(monte_carlo.MonteCarloSolver):
    """Search for global energy minimum with Simulated Annealing
    
    Begins with wide search and narrows around minima by lowering temperature,
    leading to a lower probability of accepting a poor move, using a basic
    Metropolis-Hastings approach.
    """
    
    def __init__(self, problem, params):
        
        super(SimulatedAnnealing, self).__init__(problem, params)
        #self.schedule = np.linspace(params['T0'], params['Tf'], params['steps'])
        # store last thousand trials in queues
        #self.queue_len = 1000
        #self.energies = deque(maxlen=self.queue_len)
        #self.configurations = deque(maxlen=self.queue_len)

        self.solver_name = "Simulated Annealing"
        self.T0 = params['T0']
        self.Tf = params['Tf']
        #self.s = params['s']
        self.P = params['P']
        self.step = params['step']
        self.s = np.power(self.T0/self.Tf, 1.0/self.step) - 1
        #print "{}".format(self.s)
        #sys.exit()

        self.slices = []
        for x in range(self.P):
            self.slices.append(copy.deepcopy(problem))
        
        # write a header
        #if self.outfile:
        #    with open(self.outfile, 'w') as fp:
        #        fp.write("{}, {}".format(self, datetime.datetime.now()))
        #        fp.write("T,E,config\n")

    def solve(self):
        """Solve the problem and return the solution energy and configuration"""
        
        #storage_thresh = self.steps - self.queue_len
        #self.energies.clear()
        #self.configurations.clear()

        if self.outfile:
            fp = open(self.outfile, 'w')
            config = self.p.spins_to_hex() + ",-1"
            fp.write("{}\n{}\n{}\n".format(self.solver_name, self.params, repr(self.p)))
            self._state_dump(fp, self.params["T0"], self.p.E, config, 0, 0)

        #a = 1.0/self.T0
        a = self.T0
        t = a/float(self.step)
        
        fixed = float(self.slices[0].description.split(" ")[2])
        #beta_f = 1.0/self.Tf
        beta_f = self.Tf
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
            #T = a/1
            T = a
            slices = np.random.permutation(self.P)
            for k in slices:
                #choose a random spin
                i = np.random.randint(0, self.p.size)

                # get local energy cost
                dE = self.slices[k].calculate_dE(i)

                if self.step_accepted(dE, T):
                    self.slices[k].flip_spin(i)
                    self.slices[k].E += dE
            dE_global = np.zeros(self.P)
            i = np.random.randint(0, self.p.size)
            for k in range(self.P):
                dE_global[k] = self.slices[k].calculate_dE(i)
            if self.step_accepted(np.sum(dE_global), T):
                for k in range(self.P):
                    self.slices[k].flip_spin(i)
                    self.slices[k].E += dE_global[k]
            energies = [x.E for x in self.slices]
            obj = np.min(energies)
            x = np.argmin(energies)
            if obj < best:
                b_flag = 1
                best = obj
                update += 1
                if self.outfile:
                    config = self.slices[x].spins_to_hex() + ",{}".format(x)
                    self._state_dump(fp, update, counter, best - fixed, T, config)
                    print "{}\t{}\t{}\t{}".format(counter, obj - fixed, update, config)
            if b_flag == 1 or counter % spin_log == 0:
                config = self.slices[x].spins_to_hex() + ",{}".format(x)
                print "{}\t{}\t{}\t{}\t{}".format(counter, obj - fixed, update, config, a)
                #print "{}\t{}\t{}".format(counter, obj - fixed, update)
            b_flag = 0
            a -= t
        end = time.time()
        elapsed_time = end -start
        print "Annealed obj updated"
        print "Elapsed_time: {}".format(elapsed_time)
        print "stride is {}".format(self.s)
        """    
        for step, T in enumerate(self.schedule):
        # choose a random spin
        i = np.random.randint(0, self.p.size)
        dE = self.p.calculate_dE(i)
        if self.step_accepted(dE, T):
        self.p.flip_spin(i)
        self.p.E += dE
        
        # this will slow things down quite a bit, but is cool to see
        if self.outfile:
        self._state_dump(fp, T, self.p.E, self.p.spins_to_hex())
        """            
            
        # store final self.queue_len steps
        #if step >= storage_thresh:
        #    config = self.p.spins_to_hex(self.p.spins)
        #    self.energies.append(self.p.E)
        #    self.configurations.append(config)
        
        if self.outfile:
            fp.close()
        
        # Get the best from the queue
        #best = np.argmin(self.energies)
        #self.p.E = self.energies[best]
        #self.p.spins = self.p.hex_to_spins(self.configurations[best])

        energies = [x.E for x in self.slices]
        best = np.argmin(energies)
        self.p.E = energies[best] - fixed
        self.p.spins = self.slices[best].spins.copy()
        self.count = counter
        print "steps: {}".format(counter)
        print "Linear schedule\n"

        return self.p.E - fixed, self.p.spins_to_hex(), self.count
