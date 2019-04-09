#!/bin/bash

mkdir -p TestCases/output
make

TEST_SIZES=(1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536)

pushd TestCases/output > /dev/null

for f in ../*.lis
do
	OUTPUT_FILE="output_`basename ${f}`.csv"
	echo "Testing $f..."
	echo "n, time" >> $OUTPUT_FILE
	for s in "${TEST_SIZES[@]}"
	do
		echo -e "\tTesting size: $s"
		echo -n $s >> $OUTPUT_FILE
		echo -n "," >> $OUTPUT_FILE
		../../lru $s $f >> $OUTPUT_FILE
	done
done

popd
