#!/bin/bash
i=1
j=$(($RANDOM%10))
k=$RANDOM
for run in {1..10}
do
    ./DiscreteEvent $i $j $k > test_data_arrival.txt
    j=$(($RANDOM%10))
    k=$(($RANDOM))
    i=$(($i+1))
done
for run in {1..5}
do
    ./DiscreteEvent $i $j $k > test_data_.txt
    j=$(($RANDOM%10))
    k=$(($RANDOM))
    i=$(($i+1))
done
