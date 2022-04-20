#!/bin/bash
# Script Linux & Mac pour lancer un main de tests, et vérifie si :
# - il n'y a pas de leaks dessus
# - si le programme return bien 0 (pas de segfault ou sigabort)
# - [LINUX] Vérifie les invalid read/write et autre erreur invisible
# - Compare le resultat de la sortie standard avec la STL (le main doit gérer le #define IS_STL)

EXEC=ircserv
ERROR_CMD=("" "9426" "-1" "not_a_port" "0 password" "1 password" "2147483647 password" "-2147483648 password" "2147483648 password" "2147483649 password" "2147499999 password"  "-2147499999 password")

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
printf '[⬜] Testing bad arguments ...'
for failCmd in ${ERROR_CMD[@]}
do
	./$EXEC $failCmd &> /dev/null
	if [ $? == 0 ]; then
		OK=0
		printf '\r[🟥] Arguments %s return code %d\n' $failCmd $?
		printf '[⬜] Testing bad arguments ...'
	elif [ $? == 139 ]; then
		OK=0
		printf '\r[🟥] Arguments %s make a SegFault\n' $failCmd
		printf '[⬜] Testing bad arguments ...'
	fi
done
if [ $OK == 1 ]; then
	printf '\r[🟩] Testing bad arguments ... done\n'
else
	printf '\r[🟧] Testing bad arguments ... done\n'
fi

#memory_check

exit $?
