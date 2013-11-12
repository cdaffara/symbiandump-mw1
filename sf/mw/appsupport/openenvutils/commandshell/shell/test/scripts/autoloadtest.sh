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
# Autoload -X loads the function to replace existing one, and gets executed
#

init()
    {
    let totalno=0;
    let passno=0;
    let failno=0;
    }

test_autoload2()
{
let totalno=totalno+1
echo "Test $totalno: autoload with one -X argument"
autoload -X
ret=$?
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 1, returned $ret"
    let failno=failno+1
fi
}


# Autoload +z loads the function in zsh mode
test_autoload3()
{
let totalno=totalno+1
echo "Test $totalno: autoload with one +z argument, to load the function in zsh mode"
autoload +z
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



#autoload +k loads the function in ksh mode
test_autoload4()
{
let totalno=totalno+1
echo "Test $totalno: autoload with one +k argument, to load the function in ksh mode"
autoload +z
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

# autoload -kz as invalid option
test_autoload5()
{
let totalno=totalno+1
echo "Test $totalno: autoload with one -kz argument, to display invalid option"
autoload -kz
ret=$?
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 1, returned $ret"
    let failno=failno+1
fi
}

# "autoload +" to print names only of autoloaded functions
test_autoload6()
{
let totalno=totalno+1
echo "Test $totalno: autoload with one + argument, to display names of autoloaded functions"
autoload +
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


# "autoload -Uu" to print bad option -u
test_autoload7()
{
let totalno=totalno+1
echo "Test $totalno: autoload withe -Uu as argument, to display bad option -u"
autoload -Uu
ret=$?
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 1, returned $ret"
    let failno=failno+1
fi
}

# "autoload -w somearg" tries to load zwc file to autoload
test_autoload8()
{
let totalno=totalno+1
echo "Test $totalno: autoload with "-w somearg" as argument, to display error: Cannot open zwc file: somearg.zwc"
autoload -w somearg
ret=$?
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 1, returned $ret"
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
test_autoload2
test_autoload3
test_autoload4
test_autoload5
test_autoload6
test_autoload7
test_autoload8
report
