#!/bin/bash

OK='\033[32m'
FAIL='\033[31m'
ENDC='\033[0m'
BOLD='\033[1m'

PASS_COUNTER=0
FAIL_COUNTER=0

TEST_DIR=$(dirname "$0")

for TEST_IN in "${TEST_DIR}"/test*.in; do
	echo -e "${BOLD}Testing file ${TEST_IN}${ENDC}"
	RES=$($1 < "$TEST_IN" 2>&1)

	TEST_OUT="${TEST_IN%.in}.out"

	if diff "$TEST_OUT" <(printf "%s\n" "$RES"); then
		echo -e "${OK}TEST PASS${ENDC}"
		PASS_COUNTER=$(( PASS_COUNTER + 1 ))
	else
		echo -e "${FAIL}TEST FAIL${ENDC}"
		echo "Program output"
		printf "%s\n" "${RES}"
		echo "Expected output"
		cat "$TEST_OUT"
		FAIL_COUNTER=$(( FAIL_COUNTER + 1 ))
	fi

done

echo -e "\n${OK}TESTS PASSED ${PASS_COUNTER}${ENDC}"
echo -e "${FAIL}TESTS FAILED ${FAIL_COUNTER}${ENDC}"

