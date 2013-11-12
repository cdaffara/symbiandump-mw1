#
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
#


#!/home/guest/vinodp/local/bin/zsh
###################################
# Test cases for internal commands#
###################################


#Inititalise counters for Total tests, passes, fails.
init()
{
let totalno=0;
let passno=0;
let failno=0;
return
}


#Start test case for defect DEF115281 
test_ls_long()
{

let totalno=totalno+1

ls -l
ret=$?

if [ $ret = 0 ]
then
 echo "Is ls -l command given here showing the contents properly\(y\/n\):"
 read yesorno
 if [ $yesorno = "y" ]
 then
   echo "PASS"
   let passno=passno+1
 else
   echo "FAIL: Expected \"y\", returned $yesorno"
   let failno=failno+1
 fi
else
 echo "FAIL: Expected return value 0, but  returned $ret"
 let failno=failno+1
fi
return
}



report()
{
echo "#######################################################################################"
echo "Total tests : $totalno"
echo "Passed      : $passno"
echo "Failed      : $failno"
echo "#######################################################################################"
}



init
test_ls_long
report
