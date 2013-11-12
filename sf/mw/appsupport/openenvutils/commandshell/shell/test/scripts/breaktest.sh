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
# break not inside any loop
#

init()
    {
    let totalno=0;
    let passno=0;
    let failno=0;
    }

# @internal test function
util_break2()
{
let totalno=totalno+1
echo "Test $totalno: break not inside any loop"
res=`break`
}



test_break2()
{
util_break2
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


# break 2 levels out
test_break3()
{
let totalno=totalno+1
echo "Test $totalno: break with 2 as argument inside repeat inside for loop, to exit from both"
for i in 1 2 3
do	
	repeat 10 break 2
done
if [ $i = 1 ]
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
# test_break2
test_break3
report
