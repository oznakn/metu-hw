#!/usr/bin/bash
cp the2.c the2.h test_cases/
cd test_cases

items=('common_connections_1_0' 'connect_users_1_0' 'connect_users_1_1' 'get_connection_1_0' 'get_user_1_0' 'get_user_1_1' 'remove_connection_1_0')

i=0

for item in "${items[@]}"; do
	out="$(gcc -ansi -pedantic-errors -Wall -lm -o the2 $item.c the2.c the2.h 2> >(while read line; do echo -e "\e[2m$line\e[0m" >&2; done) && ./the2)"
	if [ "$out" == "$(cat $item.out)" ]; then
		echo -e "$item: \e[92mSUCCESS\e[0m"
		echo -e ""
	else
		echo -e "$item: \e[91mFAIL\e[0m"
		echo -e "Given output:\e[37m"
		echo "$out" | awk '{print "     " $0}'
		echo ""
		echo -e "\e[0mDifference:"
		echo "$out" | diff --unchanged-line-format="" --old-line-format="Expected:      %L" --new-line-format="Given   :      %L" "$item.out" -  
		echo ""
		break
	fi
	((i++))
done

echo "Tests passed ($i/${#items[@]})"

rm the2 the2.c the2.h
cd ../
