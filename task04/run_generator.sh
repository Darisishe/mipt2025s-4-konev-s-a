#!/bin/sh

for i in {1..20}; do
  ../bin/task04_01 "./data/default.json" "./data/$i.png" ./data/"$i"_truth.txt $i
done

for i in {21..30}; do
  ../bin/task04_01 "./data/low_contrast_config.json" "./data/$i.png" ./data/"$i"_truth.txt $i
done

for i in {31..40}; do
  ../bin/task04_01 "./data/noised_config.json" "./data/$i.png" ./data/"$i"_truth.txt $i
done
