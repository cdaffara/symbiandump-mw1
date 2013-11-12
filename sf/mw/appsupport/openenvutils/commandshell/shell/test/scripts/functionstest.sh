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

# "functions -m somepattern" to autoload patterned functions
test_functions2()
{
let totalno=totalno+1
echo "Test $totalno: autoload with -m somepattern as arguments, to autoload functions with somepattem"
functions -m test
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

# "functions -mz nonexisingpattern" to autoload patterned functions in zsh mode
test_functions3()
{
let totalno=totalno+1
echo "Test $totalno: autoload with -mz nonexistingpattern as arguments, to autoload functions with nonexisting pattem in zsh mode"
functions -mz nonexistingpatterm
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

util_functions4()
{
echo hello
}

# "functions -mz exisingpattern" to autoload patterned functions in zsh mode
test_functions4()
{
let totalno=totalno+1
echo "Test $totalno: autoload with -mz existingpattern as arguments, to autoload functions with pattem in zsh mode"
util_functions4
functions -mz util_functions4
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

# "functions -mk exisingpattern" to autoload patterned functions in ksh mode
test_functions5()
{
let totalno=totalno+1
echo "Test $totalno: autoload with -mk existingpattern as arguments, to autoload functions with pattem in ksh mode"
util_functions4
functions -mk util_functions4
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
test_functions2
test_functions3
test_functions4
test_functions5
report
