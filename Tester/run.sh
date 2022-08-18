#!/bin/bash

NC="\033[0m"
BOLD="\033[1m"
ULINE="\033[4m"
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
MAGENTA="\033[35m"
fatal_error()
{
	if [ -z "$1" ]
	then
		message="fatal error"
	else
		message="$1"
	fi
	if [ -z "$2" ]
	then
		exit_status=1
	else
		exit_status=$2
	fi
	printf "${RED}$message${NC}\n"
	exit $exit_status
}

function should_execute()
{
	ref=$1
	shift
	tests=("$@")
	if [ ${#tests} -eq 0 ]
	then
		return 0
	else
		for test_number in "${tests[@]}"
		do
			if [ "$ref" == "$test_number" ]; then return 0; fi
		done
	fi
	return 1
}

commands_needed=("awk" "sleep" "date" "awk" "dirname" "touch" "chmod" "ping" "git" "mkdir" "make" "nm" "grep" "wc" "cat" "hostname" "head")
for command_needed in "${commands_needed[@]}"
do
	command -v $command_needed > /dev/null 2>&1 || fatal_error "'$command_needed' command not installed... Aborting."
done

cd "$(dirname "$0")"

CONFIG=0
LEAKS=1
READ_CONFIG=1
UPDATE=0
VERBOSE=0

source scripts/config.sh

DISABLE_TIMEOUT=0
TESTS_OK=0
TESTS_KO=0
TESTS_LK=0
TESTS_TO=0
exec 3>&1

source scripts/test_utils.sh

source scripts/update.sh

cat assets/banner.txt

# Parse arguments
while getopts ":cltuv" opt
do
	case $opt in
		c)
			CONFIG=1;;
		l)
			LEAKS=0;;
		t)
			DISABLE_TIMEOUT=1;;
		u)
			UPDATE=1;;
		v)
			VERBOSE=1;;
		*)
			break;;
	esac
done
shift $((OPTIND-1))

MEMLEAKS=""
LEAK_RETURN=240
if ! command -v valgrind > /dev/null 2>&1
then
	printf "${BLUE}valgrind is not installed. Memory leaks detection is not enabled...${NC}\n"
	LEAKS=0
elif [[ "$OSTYPE" == "darwin"* ]]
then
	printf "${BLUE}Memory leaks detection has been disabled on Darwin plateforms...${NC}\n"
	LEAKS=0
else
	if [ $LEAKS -gt 0 ]
	then
		MEMLEAKS="valgrind --leak-check=full --show-leak-kinds=all --undef-value-errors=no --error-exitcode=$LEAK_RETURN --errors-for-leak-kinds=all"
	fi
fi

# Config
if ! [ -f config.vars ] || [ $CONFIG -gt 0 ]
then
	prompt_configuration
fi

if [ $READ_CONFIG -gt 0 ]
then
	if ! [ -x "config.vars" ]
	then
		if ! chmod u+x config.vars > /dev/null 2>&1
		then
			fatal_error "The config.vars file is not executable...\nTry \`chmod +x config.vars\`"
		fi
	fi
	. config.vars
fi

if [ -z "$PROJECT_DIRECTORY" ] || [ -z "$CHECK_UPDATE" ]
then
	prompt_configuration
fi

# Update
if [ $UPDATE -gt 0 ] || ([ $CHECK_UPDATE -gt 0 ] && should_update)
then
	echo "LAST_UPDATE=$(date +%s)" > .last-update
	if ping -c 1 example.org > /dev/null 2>&1
	then
		update_tester
	else
		printf "${BLUE}Cannot check remote update...${NC}\n"
	fi
fi

if ! mkdir -p outs > /dev/null 2>&1
then
	fatal_error "Unable to create the out logs folder..."
fi
if ! [ -w outs ]
then
	fatal_error "Unable to write to the 'outs' folder as your user...${NC}"
fi

printf "\n"
printf "${BOLD}Testing your project....\n${NC}" 
sleep 1
printf "${BOLD}We will see ...\n\n ${NC}"
sleep 2
printf "${BOLD}\rHere is the result:${NC}\n\n"
trap pipex_summary SIGINT
num="00"
test_suites=("$@")

pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program doesn't use forbidden functions"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	nm -u $PROJECT_DIRECTORY/pipex > /dev/null 2>&1 | grep -Ev '(___error|___stack_chk_fail|___stack_chk_guard|access|close|dup|dup2|__errno_location|execve|exit|fork|free|__gmon_start__|__libc_start_main|malloc|open|perror|pipe|printf|read|strerror|unlink|wait|waitpid|write|dyld_stub_binder)(@|$)' > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: 1" > outs/test-$num-exit.txt
	if [ $status_code -eq 1 ]
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		result_color=$GREEN
	else
		TESTS_KO=$(($TESTS_KO + 1))
		result="KO"
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program does not crash with no parameters"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -ne 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program does not crash with one parameter"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -ne 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
if [ $VERBOSE -gt 0 ] && ([ "$result" != "OK" ] || [ "$result_color" != "$GREEN" ])
then
	[ -f outs/test-$num.txt ] && cat outs/test-$num.txt 
	[ -f outs/test-$num-original.txt ] && cat outs/test-$num-original.txt 
	[ -f outs/test-$num-tty.txt ] && cat outs/test-$num-tty.txt 
	[ -f outs/test-$num-exit.txt ] && cat outs/test-$num-exit.txt 
fi

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program does not crash with two parameters"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "grep Now" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -ne 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program does not crash with three parameters"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "grep Now" "wc -w" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -ne 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program exits with the last command's status code"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	PATH=$PWD/assets:$PATH pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "grep Now" "exit 5" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -eq 5 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program handles infile's open error"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "not-existing/deepthought.txt" "grep Now" "wc -w" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -eq 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The output when infile's open error occur is correct"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "not-existing/deepthought.txt" "grep Now" "wc -w" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	< /dev/null grep Now | wc -w > outs/test-$num-original.txt 2>&1
	if [ $status_code -lt 128 ]
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if diff outs/test-$num-original.txt outs/test-$num.txt > /dev/null 2>&1
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program handles outfile's open error"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "grep Now" "wc -w" "not-existing/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -ne 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program handles execve errors"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	chmod 644 assets/deepthought.txt
	PATH=$PWD/assets:$PATH pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "cat" "not-executable" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -ne 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program handles path that doesn't exist"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	PATH=/not/existing:$PATH pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "grep Now" "wc -w" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -eq 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# **************************************************************************** #

printf "\n${BOLD}The next tests will use the following command:${NC}\n"
printf "$PROJECT_DIRECTORY/pipex \"assets/deepthought.txt\" \"notexisting\" \"wc\" \"outs/test-xx.txt\"\n"
printf "(notexisting is a command that is not supposed to exist)${NC}\n\n"

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program handles the command"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "notexisting" "wc" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -eq 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The output of the command contains 'command not found'"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "notexisting" "wc" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if grep -i "command not found" outs/test-$num-tty.txt > /dev/null 2>&1 && [ $status_code -lt 128 ]
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		result_color=$GREEN
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
			result_color=$RED
		else
			TESTS_OK=$(($TESTS_OK + 1))
			result="OK"
			result_color=$GREEN
		fi
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose


# **************************************************************************** #

printf "\n${BOLD}The next tests will use the following command:${NC}\n"
printf "$PROJECT_DIRECTORY/pipex \"assets/deepthought.txt\" \"cat\" \"notexisting\" \"outs/test-xx.txt\"\n"
printf "(notexisting is a command that is not supposed to exist)${NC}\n\n"

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program exits with the right status code"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "cat" "notexisting" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -eq 127 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The output of the command contains 'command not found'"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "cat" "notexisting" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if grep -i "command not found" outs/test-$num-tty.txt > /dev/null 2>&1 && [ $status_code -lt 128 ]
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		result_color=$GREEN
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
			result_color=$RED
		else
			TESTS_OK=$(($TESTS_OK + 1))
			result="OK"
			result_color=$GREEN
		fi
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# **************************************************************************** #

printf "\n${BOLD}The next tests will use the following command:${NC}\n"
printf "$PROJECT_DIRECTORY/pipex \"assets/deepthought.txt\" \"grep Now\" \"$(which cat)\" \"outs/test-xx.txt\"\n"

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The program exits with the right status code"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "grep Now" "$(which cat)" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	if [ $status_code -lt 128 ] # 128 is the last code that bash uses before signals
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		if [ $status_code -eq 0 ]
		then
			result_color=$GREEN
		else
			result_color=$GREEN
		fi
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_KO=$(($TESTS_KO + 1))
			result="KO"
		fi
		result_color=$RED
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# TEST
num=$(echo "$num 1" | awk '{printf "%02d", $1 + $2}')
description="The output of the command is correct"
printf "${BLUE}# $num: %-69s  []${NC}" "$description"
if should_execute ${num##0} ${test_suites[@]}
then
	pipex_test $PROJECT_DIRECTORY/pipex "assets/deepthought.txt" "grep Now" "$(which cat)" "outs/test-$num.txt" > outs/test-$num-tty.txt 2>&1
	status_code=$?
	echo -e "Exit status: $status_code`[ $status_code -eq $LEAK_RETURN ] && printf " (Leak special exit code)"`\nExpected: <128" > outs/test-$num-exit.txt
	< assets/deepthought.txt grep Now | $(which cat) > outs/test-$num-original.txt 2>&1
	if diff outs/test-$num-original.txt outs/test-$num.txt > /dev/null 2>&1 && [ $status_code -lt 128 ]
	then
		TESTS_OK=$(($TESTS_OK + 1))
		result="OK"
		result_color=$GREEN
	else
		if [ $status_code -eq 143 ]
		then
			TESTS_TO=$(($TESTS_TO + 1))
			result="TO"
			result_color=$RED
		elif [ $status_code -eq $LEAK_RETURN ]
		then
			TESTS_LK=$(($TESTS_LK + 1))
			result="LK"
		else
			TESTS_OK=$(($TESTS_OK + 1))
			result="OK"
			result_color=$GREEN
		fi
	fi
	printf "\r${result_color}# $num: %-69s [%s]\n${NC}" "$description" "$result"
else
	printf "\n"
fi
pipex_verbose

# **************************************************************************** #

pipex_verbose

pipex_summary
