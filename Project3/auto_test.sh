#!/bin/bash

mkdir -p TestCases/output
make

TEST_SIZES=(1 2 4 8 16 32 64 128 256 512 1024 1536 2048 3072 4096 6192 8192 12288 16384 24576 32768 49152 65536)

pushd TestCases/output > /dev/null

#for f in ../*.lis
#do
#	OUTPUT_FILE="output_`basename ${f}`.csv"
#	echo "Testing $f..."
#	echo "n, time" >> $OUTPUT_FILE
#	for s in "${TEST_SIZES[@]}"
#	do
#		echo -e "\tTesting size: $s"
#		echo -n $s >> $OUTPUT_FILE
#		echo -n "," >> $OUTPUT_FILE
#		../../lru $s $f >> $OUTPUT_FILE
#	done
#done

OUTPUT_FILE="output.csv"

echo -n "ratio," > $OUTPUT_FILE

for f in ../*.lis
do
	echo -n "`basename $f`," >> $OUTPUT_FILE
done

truncate -s-1 $OUTPUT_FILE
echo -ne "\n" >> $OUTPUT_FILE

for size in "${TEST_SIZES[@]}"
do

	# The number
	echo -n "${size}," >> $OUTPUT_FILE
	echo "Testing at size ${size}"

	for file in ../*.lis
	do
		echo -e "\tFile: ${file}"
		../../lru $size $file >> $OUTPUT_FILE
		truncate -s-1 $OUTPUT_FILE	# Remove the \n
		echo -ne "," >> $OUTPUT_FILE
	done
	truncate -s-1 $OUTPUT_FILE		# Remove the trailing ,
	echo -ne "\n" >> $OUTPUT_FILE

done

popd
