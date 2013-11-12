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
    let totalno=0;
    let passno=0;
    let failno=0;
    }

preamble()
{
mkdir cdtest
mkdir dirstest
}

test_cd2()
{
let totalno=totalno+1
echo "Test $totalno: cd with a directory name in restricted mode"
setopt restricted
cd cdtest
if [ $ret -eq 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
fi
}

test_cd3()
{
let totalno=totalno+1
echo "Test $totalno: cd without any argument to change to home dir"
cwd=`pwd`
cd
ret=$?
if [ $ret -eq 0 ]
then
	if [ $PWD = "C:" ] || [ $PWD = "Z:" ]
	then
    	echo "PASS"
    	let passno=passno+1
	else
		echo "FAIL: Expected C: or Z:, but  returned $PWD"
    	let failno=failno+1
	fi
	cd $cwd
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

test_cd4()
{
let totalno=totalno+1
echo "Test $totalno: cd with - argument to change to previous dir"
cwd=`pwd`
cd cdtest
cd -
ret=$?
if [ $ret -eq 0 ]
then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $cwd, but  returned $PWD"
        let failno=failno+1
    fi
    cd $cwd
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

test_cd5()
{
let totalno=totalno+1
echo "Test $totalno: cd with arg1 arg2 to replace the strings arg1 with arg2 in the current path and tries to change to that  directory"
cd dirstest
cd test dest
ret=$?
if [ $ret -eq 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
fi
cd ..
}


test_cd6()
{
let totalno=totalno+1
echo "Test $totalno: cd with +0 to change to inedxed entry in dirs stack"
pushd dirstest
mkdir new
pushd new
mkdir old
pushd old
cwd=`pwd`
cd +0
ret=$?
if [ $ret -eq 0 ]
then
	if [ $PWD = $cwd ]
	then 
    	echo "PASS"
	    let passno=passno+1	
	else
		echo "FAIL: Expected $cwd, but  returned $PWD"
		let failno=failno+1
	fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
popd
rmdir old
popd
rmdir new
popd
}

test_cd7()
{
let totalno=totalno+1
echo "Test $totalno: cd with +1 to change to inedxed entry in dirs stack"
pushd dirstest
mkdir new
pushd new
cwd=`pwd`
mkdir old
pushd old
cd +1
ret=$?
if [ $ret -eq 0 ]
then
    if [ $PWD = $cwd ]
    then
        let passno=passno+1
    else
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
rmdir old
popd
rmdir new
popd
}


test_cd8()
{
let totalno=totalno+1
echo "Test $totalno: cd with -0 to change to inedxed entry in dirs stack"
cwd=`pwd`
pushd dirstest
mkdir new
pushd new
mkdir old
pushd old
cd -0
ret=$?
if [ $ret -eq 0 ]
then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $cwd, but  returned $PWD"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
popd
rmdir old
popd
rmdir new
popd
cd ..
}

test_cd9()
{
let totalno=totalno+1
echo "Test $totalno: cd with -1 to change to inedxed entry in dirs stack"
pushd dirstest
cwd=`pwd`
mkdir new
pushd new
mkdir old
pushd old
cd -1
ret=$?
if [ $ret -eq 0 ]
then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $cwd, but  returned $PWD"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
popd
rmdir old
cd ..
rmdir new
popd
}

test_cd10()
{
let totalno=totalno+1
echo "Test $totalno: cd with a -s directory name, not symbolic link"
cd -s dirstest
ret=$?
if [ $ret -eq 0 ]
then
    echo "PASS"
    let passno=passno+1
	cd ..
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

test_cd11()
{
let totalno=totalno+1
echo "Test $totalno: cd with a -P directory name, not symbolic link"
cd -P dirstest
ret=$?
if [ $ret -eq 0 ]
then
    let passno=passno+1
	cd ..
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

test_cd12()
{
let totalno=totalno+1
echo "Test $totalno: cd with a -L directory name, CHASE_LINKS not set and dir name is not symbolic link"
cd -L dirstest
ret=$?
if [ $ret -eq 0 ]
then
    echo "PASS"
    let passno=passno+1
	cd ..
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

test_cd13()
{
let totalno=totalno+1
echo "Test $totalno: cd with a -L directory name, CHASE_LINKS set and dir name is not symbolic link"
setopt CHASE_LINKS
cd -L dirstest
ret=$?
if [ $ret -eq 0 ]
then
    echo "PASS"
    let passno=passno+1
    cd ..
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
unsetopt CHASE_LINKS
}

test_cd14()
{
let totalno=totalno+1
echo "Test $totalno: cd with a "." as argument, cdpath set"
cdpath=dirstest
cd .
if [ $ret -eq 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
cdpath=
}

test_cd15()
{
let totalno=totalno+1
echo "Test $totalno: cd with a ".," as argument, cdpath set"
cdpath=dirstest
cd .,
ret=$?
if [ $ret -eq 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
fi
cdpath=
}

test_cd16()
{
let totalno=totalno+1
echo "Test $totalno: cd with a "../dirstest" as argument, cdpath set"
cdpath=dirstest
pwd
cd dirstest
cwd=`pwd`
cd ../dirstest
ret=$?
if [ $ret -eq 0 ]
then
	if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $cwd, but  returned $PWD"
        let failno=failno+1
	fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
cdpath=
cd ..
}

test_cd17()
{
let totalno=totalno+1
echo "Test $totalno: cd with a "/nonexistingdir" as argument"
cd /popfdtest
ret=$?
if [ $ret -eq 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
fi
}

test_cd18()
{
let totalno=totalno+1
echo "Test $totalno: cd with a "nonexistingdir" as argument, cdpath being set"
cdpath=dirstest
cd popfdtest
ret=$?
if [ $ret -eq 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
fi
cdpath=
}

postamble()
{
rmdir dirstest
rmdir cdtest
}
report()
    {
    echo "#############################################################################"
    echo "Total tests : $totalno"
    echo "Passed      : $passno"
    echo "Failed      : $failno"
    echo "#############################################################################"
    }

init
preamble
#test_cd2
test_cd3
test_cd4
test_cd5
test_cd6
test_cd7
test_cd8
test_cd9
test_cd10
test_cd11
test_cd12
test_cd13
test_cd14
test_cd15
test_cd16
test_cd17
test_cd18
postamble
report

