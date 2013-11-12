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
let totalno=0
let passno=0
let failno=0
}


test_set2()
{
let totalno=totalno+1
echo "Test $totalno:"
set -s q p r
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi

}

test_set3()
{
let totalno=totalno+1
echo "Test $totalno:"
set +s x z c
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_set4()
{
let totalno=totalno+1
echo "Test $totalno:"
set -A array -x -- foo
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_set5()
{
let totalno=totalno+1
echo "Test $totalno:"
set -
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}


test_set6()
{
let totalno=totalno+1
echo "Test $totalno:"
set --
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_set7()
{
let totalno=totalno+1
echo "Test $totalno:"
set -x
ret=$?

if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_set8()
{
let totalno=totalno+1
echo "Test $totalno:"
set +x
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_set9()
{
let totalno=totalno+1
echo "Test $totalno:"
set +
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_set10()
{
let totalno=totalno+1
echo "Test $totalno:"
set +A array --
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}


test_set11()
{
let totalno=totalno+1
echo "Test $totalno:"
set -A array --
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_set12()
{
let totalno=totalno+1
echo "Test $totalno:"
setopt ksh_arrays
set -A array -x foo
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
unsetopt ksh_arrays
} 

test_set13()
{

let totalno=totalno+1
echo "Test $totalno:"
setopt ksh_arrays
set +A array -x foo
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
unsetopt ksh_arrays
}

test_set14()
{
let totalno=totalno+1
echo "Test $totalno:"

setopt ksh_arrays
set +A array -s a v c
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
unsetopt ksh_arrays
}

test_set15()
{
let totalno=totalno+1
echo "Test $totalno:"
setopt ksh_arrays
set +A array +s a v c
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
unsetopt ksh_arrays
}

test_set16()
{
let totalno=totalno+1
echo "Test $totalno:"
setopt ksh_arrays
set -A array +s a v c
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
unsetopt ksh_arrays
}

# Set the options for the shell and/or set the positional parameters, or declare and set an array
test_set17()
{
let totalno=totalno+1
echo "Test $totalno: set command to set positional parameters"
. ./util_set17.sh x y z
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
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
test_set2
test_set3
test_set4
test_set5
test_set6
test_set7
test_set8
test_set9
test_set10
test_set11
test_set12
test_set13
test_set14
test_set15
test_set16
test_set17
report
