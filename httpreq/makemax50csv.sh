touch result_maxcut_50.csv
for j in `seq 1 100`
do 
            echo -n "${j}," >> result_maxcut_50.csv
done
echo >> result_maxcut_50.csv
 
for i in `seq 2 10`
    do for j in `seq 1 100`
        do 
            RES=$(cat maxc/positice_50/${i}0/${j}.sh | curl -H 'Authorization: Bearer 980789b22f4a87c20ad7b9262bfdaaee ' https://annealing-cloud.com/api/v2/solve -d @- | jq '.result.energies[]')
            echo -n ${RES}, >> result_maxcut_50.csv
        done
    echo >> result_maxcut_50.csv
done