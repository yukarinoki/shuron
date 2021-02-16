for i in `seq 2 10`
    do for j in `seq 1 100`
        do 
            cat minfront.txt > minc/${i}0/${j}.sh
            cat ../hitachi/testdata/mincut/graph/mincgraph_${i}0 >> minc/${i}0/${j}.sh
            cat minmidback.txt >> minc/${i}0/${j}.sh
            echo ${j} >> minc/${i}0/${j}.sh
            cat minback.txt >> minc/${i}0/${j}.sh
        done
done

for i in `seq 2 10`
    do for j in `seq 1 100`
        do 
            cat minfront.txt > maxc/${i}0/${j}.sh
            cat ../hitachi/testdata/maxcut/graph/maxcgraph_${i}0 >> maxc/${i}0/${j}.sh
            cat minmidback.txt >> maxc/${i}0/${j}.sh
            echo ${j} >> maxc/${i}0/${j}.sh
            cat minback.txt >> maxc/${i}0/${j}.sh
        done
done