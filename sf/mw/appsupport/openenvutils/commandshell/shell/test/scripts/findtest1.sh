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

    if [ ! -d findtest ]
    then
		mkdir c:/findtest
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
set +x
test_find1()
{
	let total+=1;
	echo "test_find1() :find without options; find";
	find
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
test_find15()
{
	let total+=1;
	echo "test_find15() :find with invalid options; find -a";
	find -a
	ret=$?
	if [ $ret -ne 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find16()
{
	let total+=1;
	echo "test_find16() :find with invalid arg; find ./test123.txt -type f";
	find c:\\test123.txt -type f
	ret=$?
	if [ $ret -ne 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
#deprecated
test_find2()
{
	let total+=1;
	echo "test_find2 :find with valid option ; find  -d . ";
	find -d .
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
test_find3()
{
	let total+=1;
	echo "test_find3 :find with valid option ; find  -s . ";
	find -s .
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
test_find4()
{
	let total+=1;
	echo "test_find4 :find with primaries ; find  ./test1.txt -amin 1 ";
	find c:\\findtest\\test1.txt -amin 1 
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
test_find5()
{
	let total+=1;
	echo "test_find5 :find with primaries ; find  ./test1.txt -anewer test2.txt";
	find c:\\findtest\\test1.txt  -anewer c:\\findtest\\test2.txt
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

test_find6()
{
	let total+=1;
	echo "test_find6 :find with primaries ; find  ./test2.txt -cmin 1";
	find c:\\findtest\\test2.txt -cmin 1
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
test_find7()
{
	let total+=1;
	echo "test_find7 :find with primaries ; find  ./test1.txt -cnewer test2.txt";
	find c:\\findtest\\test1.txt -cnewer c:\\findtest\\test2.txt
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
test_find8()
{
	let total+=1;
	echo "test_find8 :find with primaries ; find  ./test2.txt -ctime 1";
	find c:\\findtest\\test2.txt -ctime 1
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
test_find9()
{
	let total+=1;
	echo "test_find9 :find with primaries ; find  ./test2.txt -delete ";
	find c:\\findtest\\test2.txt  -delete
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
test_find10()
{
	let total+=1;
	echo "test_find10 :find with primaries ; find  ./test4 -depth ";
	find c:\\findtest\\test4 -depth
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
#not supported
test_find11()
{
	let total+=1;
	echo "test_find11 :find with primaries ; find  ./test3.txt -depth 2";
	find c:\\findtest\\test3.txt -depth 2
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
test_find12()
{
	let total+=1;
	echo "test_find12 :find with primaries ; find  ./test4 -empty";
	find c:\\findtest\\test4 -empty
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
test_find13()
{
	let total+=1;
	echo "test_find13 :find with primaries ; find  . -execute ";
	find . -execute
	ret=$?
	if [ $ret -ne 0 ]
	then
		echo " PASS ";
		let pass+=1;
	else
		echo "Fail"
		echo "Expected 0 Returned $ret"
		let fail+=1;
	fi
}
test_find14()
{
	let total+=1;
	echo "test_find14 :find with primaries ; find  ./test4/test41 -iname \"test4*\" ";
	find c:\\findtest\\test4\test41.txt -iname "test4*"
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
init
#test_find1
#test_find2
test_find3
test_find4
test_find5
test_find6
test_find7
test_find8
test_find9
test_find10
#test_find11
test_find12
test_find13
test_find14
test_find15
test_find16
report
