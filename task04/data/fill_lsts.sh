
for i in {1..40}; do
  echo "$(pwd)/"$i"_pred.txt" >> predictions.lst
  echo "$(pwd)/"$i"_truth.txt" >> truths.lst
done
