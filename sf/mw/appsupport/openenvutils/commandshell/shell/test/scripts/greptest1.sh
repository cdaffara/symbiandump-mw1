# Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#

init()
{
    let total=0;
    let pass=0;
    let fail=0;

    if [ ! -d c:\\greptest ]
    then
		mkdir c:\\greptest	
    fi
	cd c:\\greptest
	cat "Hello.
		hi his
		hi
		Testing for the grep command." > test1.txt
	cat "fddsfsduf  dsofdsfsdf  dfds
	          dfsd isdf ndsljfs d" > test2.txt
	cat "fddsfsduf " > test3.txt
	if [ ! -d .\\test4 ] 
	then 
		mkdir .\\test4
	fi
	cd .\\test4
	cat "Hello.
		hi his
		hi
		Testing for the grep command." > test41.txt
	cat "fddsfsduf  dsofdsfsdf  dfds
	          dfsd isdf ndsljfs d" > test42.txt
	cat "fddsfsduf " > test43.txt
	cd 
	
}
report()
{
    echo "Changing the directory back";
    echo "--------------------Grep test results------------------------"
    echo " TOTAL TESTS : $total"
    echo " TOTAL PASS  : $pass"
    echo " TOTAL FAIL  : $fail"
    echo "------------------------------------------------------------"
}

test_grep1()
{
	echo " test_grep1 : grep with no options; grep";
	let total+=1;
	grep
	ret=$?
	if [ $ret -ne 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep2()
{
	echo " test_grep2: grep with invalid options; grep -q";
	let total+=1;
	grep -q
	ret=$?
	if [ $ret -ne 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep3()
{
	echo " test_grep3 : grep with valid args; grep \"hi\" test1.txt ";
	let total+=1;
	grep "hi" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep4()
{
	echo " test_grep4 : grep with valid args; grep -c \"hi\" test1.txt ";
	let total+=1;
	grep -c "hi" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep5()
{
	echo " test_grep5 : grep with valid args; grep -A 2 \"hi\" test1.txt ";
	let total+=1;
	grep -A 2 "hi" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep6()
{
	echo " test_grep6 : grep with valid args; grep -B 2 \"hi\" test1.txt ";
	let total+=1;
	grep -B 2 "hi" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep7()
{
	echo " test_grep7 : grep with valid args; grep -C 2 \"hi\" test1.txt ";
	let total+=1;
	grep -C 2 "hi" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep17()
{
	echo " test_grep17 : grep with valid args; grep -d recurse \"hi\"  ./test4 ";
	let total+=1;
	grep -d recurse "hi" c:\\greptest\\test4
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep8()
{
	echo " test_grep8 : grep with valid args; grep -E \"h*\" test1.txt ";
	let total+=1;
	grep -E "h*" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep9()
{
	echo " test_grep9 : grep with valid args; grep -e \"hi\" -e  \"grep\" test1.txt ";
	let total+=1;
	grep -e "hi" -e "grep" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep10()
{
	echo " test_grep10 : grep with valid args; grep -F \"hi\" test1.txt ";
	let total+=1;
	grep -F "hi" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep11()
{
	echo " test_grep11 : grep with valid args; grep -d recurse -H \"hi\" ./test4 ";
	let total+=1;
	grep -d recurse -H "hi" c:\\greptest\\test4
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep12()
{
	echo " test_grep12 : grep with valid args; grep -d recurse -h \"hi\" ./test4 ";
	let total+=1;
	grep -d recurse -h "hi" c:\\greptest\\test4
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep13()
{
	echo " test_grep13 : grep with valid args; grep -i \"Hi\" test1.txt ";
	let total+=1;
	grep -i "Hi" c:\\greptest\\test1.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep14()
{
	echo " test_grep14 : grep with valid args; grep -L \"hi\" test1.txt test2.txt ";
	let total+=1;
	grep -L "hi" c:\\greptest\\test1.txt c:\\greptest\\test2.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep15()
{
	echo " test_grep15 : grep with valid args; grep -l \"hi\" test1.txt test2.txt ";
	let total+=1;
	grep -l "hi" c:\\greptest\\test1.txt c:\\greptest\\test2.txt
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep16()
{
	echo " test_grep16 : grep with valid args; grep -m 2 \"hi\" test1.txt ";
	let total+=1;
	grep -m 2 "hi" c:\\greptest\\test1.txt 
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}

#begin making calls

init
test_grep1
test_grep2
test_grep3
test_grep4
test_grep5
test_grep6
test_grep7
test_grep8
test_grep9
test_grep10
test_grep11
test_grep12
test_grep13
test_grep14
test_grep15
test_grep16
test_grep17
report
