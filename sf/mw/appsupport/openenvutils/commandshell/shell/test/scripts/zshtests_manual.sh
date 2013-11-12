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

#Start test cases
test_read()
{
let totalno=totalno+1
echo "Test $totalno: read with one variable argument"
echo "Type \"data\" and press enter: "
read readstr
ret=$?
if [ $ret = 0 ]
then
 echo "---------- "$readstr
 if [ $readstr = "data" ]
 then
   echo "PASS"
   let passno=passno+1
 else
   echo "FAIL: Expected \"data\", returned $readstr"
   let failno=failno+1
 fi
else
 echo "FAIL: Expected return value 0, but  returned $ret"
 let failno=failno+1
fi
return
}

test_history()
{
let totalno=totalno+1
echo "Test $totalno: history to get the last command given"
history
ret=$?
if [ $ret = 0 ]
then
    echo "check whether the previous commands given before zshtets_auto.sh are the same as that dispalyed above(y/n):"
	read yorn
	if [ $yorn = "y" ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"y\", returned $yorn"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
return
}

test_vared1()
{
let totalno=totalno+1
echo "Test $totalno: vared to edit env variable PATH"
vared PATH
echo "Were you able to edit the PATH?(y/n)"
read var
if [ $var = y ]
then
   echo "PASS"
   let passno=passno+1
else
   echo "FAIL: Expected \"y\", returned $var"
   let failno=failno+1
fi
}

test_fc2()
{
let totalno=totalno+1
echo "Test $totalno: fc with -ln -1 argument"
fc -ln -1
echo "Is the previous command printed?(y/n)"
read var
if [ $var = y ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected \"y\", returned $var"
    let failno=failno+1
fi
}

test_r1()
{
let totalno=totalno+1
echo "Test $totalno: r to get the previous command given"
r
ret=$?
if [ $ret = 0 ]
then
    echo "check whether the previous command given before zshtets_auto.sh are the same as that dispalyed above(y/n):"
    read yorn
    if [ $yorn = "y" ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"y\", returned $yorn"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
return
}


test_print15()
{
let totalno=totalno+1
echo "Test $totalno: print with a -u option to write the argument string to a file descriptor"
print -u 2 hello
ret=$?
if [ $ret = 0 ]
then
	echo "check whether the print command given printed hello on console?(y/n):"
    read yorn
    if [ $yorn = "y" ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"y\", returned $yorn"
        let failno=failno+1
    fi
else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print16()
{
let totalno=totalno+1
echo "Test $totalno: print with a -z option to write the argument string to editing buffer stack"
print -z hello
ret=$?
if [ $ret = 0 ]
then
	echo "check whether the print command given printed hello on console?(y/n):"
    read yorn
    if [ $yorn = "y" ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"y\", returned $yorn"
        let failno=failno+1
    fi
else
   echo "FAIL: Expected 0, returned $ret"
   let failno=failno+1
fi
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
test_read
test_history
test_vared1
test_r1
test_print15
test_print16
report

