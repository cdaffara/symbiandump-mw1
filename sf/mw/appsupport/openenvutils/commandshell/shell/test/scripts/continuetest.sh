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

# break 1 level and try to continue.
test_continue2()
{
let totalno=totalno+1
echo "Test $totalno: continue within while loop"
let var=1
while [ $var = 1 ]
do
    if [ $var = 1 ]
    then
        let var=2
        continue 2 
    fi
    let var=3
done
if [ $var = 2 ]
then
   echo "PASS"
   let passno=passno+1
else
   echo "FAIL: Expected value 2, returned $var"
   let failno=failno+1
fi
}

# @internal test function
util_continue3()
{
res=`continue`
}

# continue not inside any loop
test_continue3()
{
let totalno=totalno+1
echo "Test $totalno: continue not within loop"
util_continue3
ret=$?
if [ $ret = 1 ]
then
   echo "PASS"
   let passno=passno+1
else
   echo "FAIL: Expected value 1, returned $var"
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
test_continue2
test_continue3
report

