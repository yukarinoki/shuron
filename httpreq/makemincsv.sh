touch result_mincut.csv
for j in `seq 1 100`
do 
            echo -n "${j}," >> result_mincut.csv
done
echo >> result_mincut.csv
 
for i in `seq 2 10`
    do for j in `seq 1 100`
        do 
            RES=$(cat minc/${i}0/${j}.sh | curl -H 'Authorization: Bearer 980789b22f4a87c20ad7b9262bfdaaee ' https://annealing-cloud.com/api/v2/solve -d @- | jq '.result.energies[]')
            echo -n ${RES}, >> result_mincut.csv
        done
    echo >> result_mincut.csv
done