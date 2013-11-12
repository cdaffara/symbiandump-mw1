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

    if [ ! -d c:\\findtest ]
    then
		mkdir c:\\findtest
    fi
	cd c:\\findtest
	cat "fddsfsduf  dsofisdf ndsljfs d" > test1.txt
	cat "fddsfsduf  dsofdsfsdf  dfds
	          dfsd isdf ndsljfs d" > test2.txt
	cat "fddsfsduf " > test3.txt
	if [ ! -d .\\test4 ]
	then
		mkdir .\\test4
	fi
	cd .\\test4
	cat "fddsfsduf  dsofisdf ndsljfs d" > test41.txt
	cat "fddsfsduf  dsofdsfsdf  dfds
	          dfsd isdf ndsljfs d" > test42.txt
	cat "fddsfsduf " > test43.txt
	cd 
	
}
report()
{
    echo "Changing the directory back";
    echo "--------------------Find test results------------------------"
    echo " TOTAL TESTS : $total"
    echo " TOTAL PASS  : $pass"
    echo " TOTAL FAIL  : $fail"
    echo "------------------------------------------------------------"
}
test_find33()
{
	let total+=1;
	echo "test_find33 :find with primaries ; find  ./test3.txt -type b"
	find c:\\findtest\\test3.txt -type b
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find34()
{
	let total+=1;
	echo "test_find34 :find with primaries ; find  ./test3.txt -type p"
	find c:\\findtest\\test3.txt -type p
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find35()
{
	let total+=1;
	echo "test_find35 :find with primaries ; find  ./test3.txt -type s"
	find c:\\findtest\\test3.txt -type s
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find36()
{
	let total+=1;
	echo "test_find36 :find with primaries ; find  ./test3.txt -type c"
	find c:\\findtest\\test3.txt -type c
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find37()
{
	let total+=1;
	echo "test_find37 :find with primaries ; find  ./test4 -type d"
	find c:\\findtest\\test4 -type d
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find38()
{
	let total+=1;
	echo "test_find38 :find with primaries ; find  ./test3.txt -type f"
	find c:\\findtest\\test3.txt -type f
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find39()
{
	let total+=1;
	echo "test_find39 :find with primaries ; find  ./test3.txt ! -type f"
	find c:\\findtest\\test3.txt ! -type f  
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find40()
{
	let total+=1;
	echo "test_find40 :find with primaries ; find  ./test3.txt  -type f -and ! -type d"
	find c:\\findtest\\test3.txt  -type f -and ! -type d 
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find41()
{
	let total+=1;
	echo "test_find41 :find with primaries ; find  ./test3.txt  -type f -or  -size 2"
	find c:\\findtest\\test3.txt  -type f -or  -size 2 
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}

#begin making calls

init
test_find33
test_find34
test_find35
test_find36
test_find37
test_find38
test_find39
test_find40
test_find41
report
