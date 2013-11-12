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
	cd test4
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
test_find17()
{
	let total+=1;
	echo "test_find17 :find with primaries ; find  ./test4/test41.txt -links 2 ";
	find c:\\findtest\\test4\\test41.txt -links 2
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
test_find18()
{
	let total+=1;
	echo "test_find18 :find with primaries ; find  ./test4 -ls  ";
	find c:\\findtest\\test4 -ls
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
test_find19()
{
	let total+=1;
	echo "test_find19 :find with primaries ; find  ./test4 -ls  ";
	find c:\\findtest\\test4 -ls
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
test_find20()
{
	let total+=1;
	echo "test_find20 :find with primaries ; find  . -maxdepth 1  ";
	find . -maxdepth 1
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

test_find21()
{
	let total+=1;
	echo "test_find21 :find with primaries ; find  . -mindepth 1  ";
	find . -mindepth 1
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
test_find22()
{
	let total+=1;
	echo "test_find22 :find with primaries ; find  ./test3.txt  -mtime 1  ";
	find c:\\findtest\\test3.txt -mtime 1
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
test_find23()
{
	let total+=1;
	echo "test_find23 :find with primaries ; find  ./test1.txt -atime 1";
	find c:\\findtest\\test1.txt -atime 1
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
test_find24()
{
	let total+=1;
	echo "test_find24 :find with primaries ; find  ./test4/  -name \"test4*\"  ";
	find c:\\findtest\\test4\\ -name "test4*"
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
test_find25()
{
	let total+=1;
	echo "test_find25 :find with primaries ; find  ./test3.txt  -newer test1.txt*  ";
	find c:\\findtest\\test3.txt -newer c:\\findtest\\test1.txt
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
test_find26()
{
	let total+=1;
	echo "test_find26 :find with primaries ; find  ./test4/  -path \"*\"  ";
	find c:\\findtest\\test4\\ -path  "*"
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
test_find27()
{
	let total+=1;
	echo "test_find27 :find with primaries ; find  ./test4/  -print  ";
	#print is also a keyword in shell. what to do???
	find c:\\findtest\\test4\\ "-print"
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
test_find28()
{
	let total+=1;
	echo "test_find28 :find with primaries ; find  ./test4/  -print0  ";
	find c:\\findtest\\test4\\ -print0
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
# not supported
test_find29()
{
	let total+=1;
	echo "test_find29 :find with primaries ; find  ./test4/  -printx  ";
	find c:\\findtest\\test4\\ -printx
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
test_find30()
{
	let total+=1;
	echo "test_find30 :find with primaries ; find  ./test4/  -prune  ";
	find c:\\findtest\\test4\\ -prune
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
test_find31()
{
	let total+=1;
	echo "test_find31 :find with primaries ; find  ./test3.txt  -size 4  ";
	find c:\\findtest\\test3.txt -size 4
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
test_find32()
{
	let total+=1;
	echo "test_find32 :find with primaries ; find  ./test3.txt -size 10c  ";
	find c:\\findtest\\test3.txt -size 10c
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
test_find17
test_find18
test_find19
test_find20
test_find21
test_find22
test_find23
test_find24
test_find25
test_find26
test_find27
test_find28
test_find29
test_find30
test_find31
test_find32
report
