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

# unhash command to try removing a nonexisting command from hash table
test_unhash2()
{
let totalno=totalno+1
echo "Test $totalno: unhash with no options and with a nonexisting hashtable entry"
unhash dispdirs 
ret=$?
if [ $ret = 1 ]
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

test_unhash3()
{
let totalno=totalno+1
echo "Test $totalno: unhash with -d option and with a variable which is cdable"
setopt cdablevars
foo=/dirstest
cd foo
unhash -d foo
ret=$?
if [ $ret = 0 ]
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
unsetopt cdablevars
}

# unhash command to try removing a nonexisting command from hash table
test_unhash4()
{
let totalno=totalno+1
echo "Test $totalno: unhash with -m option and with a nonexisting hashtable entry pattern"
unhash -m xdghsd
ret=$?
if [ $ret = 1 ]
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

func1()
{
}

# unhash command to try removing an existing patterned entries from hash table without using correct option -f alongside
test_unhash5()
{
let totalno=totalno+1
echo "Test $totalno: unhash with -m option and with an existing hashtable entry pattern for function"
unhash -m func1
ret=$?
if [ $ret = 1 ]
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

# unhash command to try removing an existing patterned entries from hash table with right option -f
test_unhash6()
{
let totalno=totalno+1
echo "Test $totalno: unhash with -mf option and with an existing hashtable entry pattern for function"
unhash -mf func1
ret=$?
if [ $ret = 0 ]
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

report()
    {
    echo "#############################################################################"
    echo "Total tests : $totalno"
    echo "Passed      : $passno"
    echo "Failed      : $failno"
    echo "#############################################################################"
    }

init
test_unhash2
test_unhash3
test_unhash4
test_unhash5
test_unhash6
report
