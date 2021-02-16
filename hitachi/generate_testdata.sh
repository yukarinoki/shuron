g++ -std=c++14 gen2.cpp -I .
for i in `seq 2 10`
do 
    echo ${i}0 | ./a.out
done 