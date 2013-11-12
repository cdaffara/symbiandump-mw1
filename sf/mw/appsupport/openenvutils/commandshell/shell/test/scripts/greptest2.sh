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
test_grep17()
{
	echo " test_grep17 : grep with valid args; grep -m 0 \"hi\" test1.txt ";
	let total+=1;
	grep -m 0 "hi" c:\\greptest\\test1.txt 
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
test_grep18()
{
	echo " test_grep18 : grep with valid args; grep --mmap \"hi\" test1.txt ";
	let total+=1;
	grep --mmap "hi" c:\\greptest\\test1.txt 
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
test_grep19()
{
	echo " test_grep19 : grep with valid args; grep -n \"hi\" test1.txt ";
	let total+=1;
	grep -n "hi" c:\\greptest\\test1.txt 
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
test_grep20()
{
	echo " test_grep20 : grep with valid args; grep -o \"hi\" test1.txt ";
	let total+=1;
	grep -o "hi" c:\\greptest\\test1.txt 
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
test_grep21()
{
	echo " test_grep21 : grep with valid args; grep -q \"hi\" test1.txt ";
	let total+=1;
	grep -q "hi" c:\\greptest\\test1.txt 
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
test_grep22()
{
	echo " test_grep22 : grep with valid args; grep -s \"hi\" test1.txt ";
	let total+=1;
	grep -s "hi" c:\\greptest\\test1.txt 
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
test_grep23()
{
	echo " test_grep23 : grep with valid args; grep -r \"hi\" ./test4 ";
	let total+=1;
	grep -r "hi" c:\\greptest\\test4 
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
test_grep24()
{
	echo " test_grep24 : grep with valid args; grep -w \"h\" test1.txt ";
	let total+=1;
	grep -w "h" c:\\greptest\\test1.txt 
	ret=$?
	if [ $ret -eq 1 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep25()
{
	echo " test_grep25 : grep with valid args; grep -cv "hi" test1.txt ";
	let total+=1;
	grep -cv "hi" c:\\greptest\\test1.txt 
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
test_grep26()
{
	echo " test_grep26 : grep with valid args; grep -ov "hi" test1.txt ";
	let total+=1;
	grep -ov "hi" c:\\greptest\\test1.txt 
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
test_grep27()
{
	echo " test_grep27 : grep with valid args; grep -x \"hi\" test1.txt ";
	let total+=1;
	grep -x "hi" c:\\greptest\\test1.txt 
	ret=$?
	if [ $ret -eq 1 ]
	then
		echo "PASS";
		let pass+=1;
	else
		echo "FAIL"
		echo "Expected 1 Returned $ret"
		let fail+=1;
	fi
}
test_grep28()
{
	echo " test_grep28 : grep with valid args; grep -a "hi" test1.txt ";
	let total+=1;
	grep -a "hi" c:\\greptest\\test1.txt 
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
test_grep17
test_grep18
test_grep19
test_grep20
test_grep21
test_grep22
test_grep23
test_grep24
test_grep25
#test_grep26
test_grep27
test_grep28
report
