#!/bin/sh

for i in {1..40}; do
  ../bin/task06 "./data/$i.png" ./data/"$i"_pred.txt ./data/"$i"_pred.png
done
