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

#pwd command with -r option print the absolute pathname of the current working directory with out symbolic links
test_pwd2()
{
let totalno=totalno+1
echo "Test $totalno: pwd with -r argument to print path of current working dir without symbolic links"
cwd=`pwd -r`
ret=$?
if [ $ret = 0 ]
    then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $PWD, returned $cwd"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

#pwd command with -P option print the absolute pathname of the current working directory with out symbolic links
test_pwd3()
{
let totalno=totalno+1
echo "Test $totalno: pwd with -P argument to print path of current working dir without symbolic links"
cwd=`pwd -P`
ret=$?
if [ $ret = 0 ]
    then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $PWD, returned $cwd"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

# pwd  with -L option print the absolute pathname of the current working directory with out symbolic links
test_pwd4()
{
let totalno=totalno+1
echo "Test $totalno: pwd with -P argument to print path of current working dir without symbolic links"
cwd=`pwd -L`
ret=$?
if [ $ret = 0 ]
    then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $PWD, returned $cwd"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
}

# pwd  with no option, with CHASE_LINKS option set, print the absolute pathname of the current working directory with out symbolic links
test_pwd5()
{
let totalno=totalno+1
echo "Test $totalno: pwd with no option, with CHASE_LINKS option set, to print path of current working dir without symbolic links"
setopt CHASE_LINKS
cwd=`pwd`
ret=$?
if [ $ret = 0 ]
    then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $PWD, returned $cwd"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, but  returned $ret"
    let failno=failno+1
fi
unsetopt CHASE_LINKS
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
test_pwd2
test_pwd3
test_pwd4
test_pwd5
report
