curl -H 'Authorization: Bearer 980789b22f4a87c20ad7b9262bfdaaee ' -d '{"type" : 5, "model": 
    [ [0,0,0,0,1], 
[0,1,0,0,-1]
  ],
  "parameters": {
    "temperature_num_steps": 10
  },
  "outputs": {
    "spins": false,
    "averaged_spins": false,
    "averaged_energy": false
  }
}' https://annealing-cloud.com/api/v2/solve