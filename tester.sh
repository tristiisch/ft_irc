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

# VALGRIND_FLAGS="--leak-check=full --error-exitcode=1 --show-leak-kinds=definite --track-origins=yes"
VALGRIND_FLAGS="--error-exitcode=1"

function compile {
	make "$@"
	if [ $? != 0 ]; then
		echo -e "\033[0;31mKO : compile error\033[0m"
		exit 1
	fi
}

function launch {
	./$EXEC $GOOD_ARGS
	# PIPE=${PIPESTATUS[0]}
	PIPE=$?
	if [ $PIPE == 139 ]; then
		echo -e "\033[0;31mKO : Segmentation fault\033[0m"
		return $PIPE
	elif [ $PIPE != 0 ]; then
		echo -e "\033[0;31mKO : prog return $PIPE\033[0m"
		return $PIPE
	fi
}

function launchClients {
	# if [ ! -z $(pidof ircserv) ]; then
	# 	exit 1
	# fi
	make client
	for (( i = 1; 5 >= i; i++ ))
	do
		printf '\r[⬜] Test client n°%s ...\n' "$i"
		./client &> /dev/null
		printf '\r[🟩] Test client n°%s ... done\n' "$i"
		sleep 1
	done
	# PID=$(pidof ircserv)
	PID=$(pidof valgrind.bin)
	if [ ! -z $PID ]; then
		kill -2 $PID
	fi
}

function memory_check {
	if command -v valgrind &>/dev/null ; then
		if ! valgrind $VALGRIND_FLAGS ./$EXEC $GOOD_ARGS ; then
			echo -e "\033[0;31mKO : leaks --------------------------- valgrind result\033[0m"
			return 1
		fi
	elif command -v leaks &>/dev/null ; then
		if ! leaks -atExit --q -- ./$EXEC $GOOD_ARGS; then
			echo -e "\033[0;31mKO : leaks --------------------------- leaks\033[0m"
			return 1
		fi
	else
		echo -e "\033[0;33mWARN: can't do memory check, unable to find valgrind or leaks\033[0m"
	fi
}

compile kill debug -i CXXFLAGS='-Wall -Wextra -std=c++98 -g3 -fsanitize=address'

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

compile clean debug CXXFLAGS='-Wall -Wextra -std=c++98 -g3'


printf '[⬜] Test %s (max 120 sec) ...\n' "$GOOD_ARGS"
# (timeout 120 ./$EXEC $GOOD_ARGS) & launchClients
# launchClients & launch
launchClients & memory_check
if [ $? != 0 ]; then
	OK=0
	printf '[🟥] Test %s return code %d\n' "$GOOD_ARGS" $?
elif [ $? == 139 ]; then
	OK=0
	printf '[🟥] Test %s make a SegFault\n' "$GOOD_ARGS"
else
	printf '[🟩] Test %s ... done\n' "$GOOD_ARGS"
fi

# if [ $OK == 1 ]; then
# 	printf '\r[🟩] Testing good arguments ... done\n'
# else
# 	printf '\r[🟧] Testing good arguments ... done\n'
# fi

exit $?
