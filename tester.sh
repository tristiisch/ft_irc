#!/bin/bash

EXEC=ircserv
ERROR_ARGS=(''
	'9426'
	'-1'
	'not_a_port'
	'0 password'
	'1 password'
	'2147483647 password'
	'-2147483648 password'
	'2147483648 password'
	'2147483649 password'
	'2147499999 password'
	'-2147499999 password'
)

GOOD_ARGS="6667 password"

VALGRIND_FLAGS="--leak-check=full --error-exitcode=1 --show-leak-kinds=definite --track-origins=yes"
DIFF_VERSION=`diff --version | head -n 1 | sed 's/|/ /' | awk '{print $4}' | sed -e 's/\./000/g' -e 's/000/\./1'`
if [ $(echo "$DIFF_VERSION >= 3.4" | bc -l) == 1 ]; then
	DIFF_FLAGS="--color"
else
	DIFF_FLAGS=""
fi

function compile {
	make $1
	if [ $? != 0 ]; then
		echo -e "\033[0;31mKO : compile error\033[0m"
		exit 1
	fi
}

function launch {
	./$EXEC $@
	PIPE=${PIPESTATUS[0]}
	if [ $PIPE == 139 ]; then
		echo -e "\033[0;31mKO : Segmentation fault\033[0m"
		return $PIPE
	elif [ $PIPE != 0 ]; then
		echo -e "\033[0;31mKO : prog return $PIPE\033[0m"
		return $PIPE
	fi
}

function memory_check {
	if command -v valgrind &>/dev/null ; then
		if ! valgrind $VALGRIND_FLAGS ./$EXEC &> output ; then
			echo -e "\033[0;31mKO : leaks --------------------------- valgrind result :\033[0m"
			cat output
			echo -e "\033[0;31mKO : leaks or bad usage\033[0m"
			return 1
		fi
	elif command -v leaks &>/dev/null ; then
		if ! leaks -atExit --q -- ./$EXEC &> output ; then
			echo -e "\033[0;31mKO : leaks --------------------------- leaks result :\033[0m"
			cat output
			echo -e "\033[0;31mKO : leaks\033[0m"
			return 1
		fi
	else
		echo -e "\033[0;33mWARN: can't do memory check, unable to find valgrind or leaks\033[0m"
	fi
}

compile $1

OK=1
# printf '[⬜] Testing bad arguments ...'
for failArg in ${ERROR_ARGS[@]}
do
	printf '[⬜] Test %s ...' "$failArg"
	./$EXEC $failArg &> /dev/null
	if [ $? == 0 ]; then
		OK=0
		printf '\r[🟥] Test %s return code %d\n' "$failArg" $?
	elif [ $? == 139 ]; then
		OK=0
		printf '\r[🟥] Test %s make a SegFault\n' "$failArg"
	else
		printf '\r[🟩] Test %s ... done\n' "$failArg"
	fi
done
if [ $OK == 1 ]; then
	printf '\r[🟩] Testing bad arguments ... done\n'
else
	printf '\r[🟧] Testing bad arguments ... done\n'
fi

# for goodArg in $GOOD_ARGS
# do
	printf '[⬜] Test %s (max 120 sec) ...' "$GOOD_ARGS"
	timeout 120 ./$EXEC $GOOD_ARGS &> log.txt
	if [ $? != 0 ]; then
		OK=0
		printf '\r[🟥] Test %s return code %d\n' "$GOOD_ARGS" $?
	elif [ $? == 139 ]; then
		OK=0
		printf '\r[🟥] Test %s make a SegFault\n' "$GOOD_ARGS"
	else
		printf '\r[🟩] Test %s ... done                                 \n' "$GOOD_ARGS"
	fi
# done
if [ $OK == 1 ]; then
	printf '\r[🟩] Testing good arguments ... done\n'
else
	printf '\r[🟧] Testing good arguments ... done\n'
fi

#memory_check

exit $?
