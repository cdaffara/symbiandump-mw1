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

util_shift2()
{
argc=$#
shift 1
ret=$?
if [ 1 -le $argc ] && [ $ret -eq  0 ] || [ $ret -eq 1 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Returned $ret"
    let failno=failno+1
    }
fi
}

test_shift2()
{
let totalno=totalno+1
echo "Test $totalno: shift with no argument to shift the argument list by 1 left, where there is no argument"
util_shift2
}

test_shift3()
{
let totalno=totalno+1
echo "Test $totalno: shift with 1 argument to shift the argument list by 1 left from 1 argument"
util_shift2 1
}

test_shift4()
{
let totalno=totalno+1
echo "Test $totalno: shift with 1 argument to shift the argument list by 1 left from 2 arguments"
util_shift2 1 2
}


util_shift5()
{
a=(1 2 3)
shift a
}

test_shift5()
{
let totalno=totalno+1
echo "Test $totalno: shift with array name as argument to shift the argument list by left"
argc=$#
util_shift5
ret=$?
if [ $ret -eq 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
    }
fi
}

test_shift6()
{
let totalno=totalno+1
echo "Test $totalno: shift with array name as argument to shift the argument list by left"
argc=$#
util_shift5 symbian
ret=$?
if [ $ret -eq 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
    }
fi
}


test_shift7()
{
let totalno=totalno+1
echo "Test $totalno: shift with array name as argument to shift the argument list by left"
argc=$#
util_shift5 symbian os
ret=$?
if [ $ret -eq 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
    }
fi
}

test_shift8()
{
let totalno=totalno+1
echo "Test $totalno: shift with array name as argument to shift the argument list by left"
argc=$#
util_shift5 symbian os ver9.1
ret=$?
if [ $ret -eq 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
    }
fi
}

test_shift9()
{
let totalno=totalno+1
echo "Test $totalno: shift with array name as argument to shift the argument list by left"
argc=$#
util_shift5 symbian os version 9.1
ret=$?
if [ $ret -eq 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
    }
fi
}

util_shift10()
{
shift -1
}

test_shift10()
{
let totalno=totalno+1
echo "Test $totalno: shift with -1 as argument to negative-test"
argc=$#
util_shift10
ret=$?
if [ $ret -eq 1 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
    }
fi

}


test_shift11()
{
let totalno=totalno+1
echo "Test $totalno: shift with -1 as argument to negative-test"
argc=$#
util_shift10 s y m b i a n
ret=$?
if [ $ret -eq 1 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
    }
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
test_shift2
test_shift3
test_shift4
test_shift5
test_shift6
test_shift7
test_shift8
test_shift9
test_shift10
test_shift11
report
