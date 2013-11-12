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
# All tests exits the script/shell on success
# So manually execute them
#

# Exits the shell script on success. Test whether a var is readonly or not after setting it as readonly.
test_readonly1()
{
let totalno=totalno+1
echo "Test $totalno: readonly function to make a variable not writeable"
readonly r
r=1
res=$?
if [ $res != 1 ]
then
    echo "FAIL: Expected 1, returned $res"
    let failno=failno+1
fi
}


test_exit1()
{
let totalno=totalno+1
echo "Test $totalno: exit from shell"
echo "This test makes you exit from the shel. Watch to check whether the shell exits or not."
echo "The exit test result is not reported"
echo "Do you want to test it?(y/n)"
read var
if [ $var = y ]
then
    exit
else
   echo "Try now"
fi
}

util_exit2()
{
exit
}

# Exit from function
test_exit2()
{
let totalno=totalno+1
echo "Test $totalno: exit from function"
util_exit2
}


# Exit when RCS is unset
test_exit3()
{
let totalno=totalno+1
echo "Test $totalno: exit from function"
unsetopt RCS
exit
}

# Exit when MONITOR is set
test_exit4()
{
let totalno=totalno+1
echo "Test $totalno: exit from function"
setopt MONITOR
exit
}

# Exit when MONITOR is unset
test_exit4()
{
let totalno=totalno+1
echo "Test $totalno: exit from function"
unsetopt MONITOR
exit
}


test_bye1()
{
let totalno=totalno+1
echo "Test $totalno: bye from shell"
echo "This test makes you exit from the shel. Watch to check whether the shell exits or not."
echo "The bye test result is not reported"
echo "Do you want to test it?(y/n)"
read var
if [ $var = y ]
then
    exit
else
   echo "Try now"
fi
}

test_logout1()
{
let totalno=totalno+1
echo "Test $totalno: logout from shell"
echo "This test makes you exit from the shel. Watch to check whether the shell exits or not."
echo "The logout test result is not reported"
echo "Do you want to test it?(y/n)"
read var
if [ $var = y ]
then
    exit
else
   echo "Try now"
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

test_exit1
test_exit2
test_bye1
test_logout1
test_readonly
report

