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

# Each named parameter is unset using no option to unset an existing option
test_unset2()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset with no option to unset patterned name which is existing"
cdpath=/dirstest
unset cdpath
ret=$?
if [ $ret = 0 ]
then
	if [ -z $cdpath ]
	then
  		echo "PASS"
  		let passno=passno+1
	else
		echo "FAIL: Expected zero-length value, returned $cdpath"
		let failno=failno+1
	fi	
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}


# Each named parameter is unset using -m option to unset an existing option
test_unset3()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset with -m option to unset patterned name which is existing"
cdpath=/dirstest
unset -m cdpath
ret=$?
if [ $ret = 0 ]
then
    if [ -z $cdpath ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected zero-length value, returned $cdpath"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}

# Each named parameter is unset using -m option to unset an existing pattern in RESTRICTED mode
test_unset4()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset with -m option to unset patterned name which is existing in RESTRICTED mode"
cdpath=/dirstest
setopt restricted
unset -m cdpath
ret=$?
if [ $ret = 0 ]
then
    if [ -z $cdpath ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected zero-length value, returned $cdpath"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}

# Each named parameter is unset using -m option to unset an nonexisting pattern 
test_unset5()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset with -m option to unset patterned name which is nonexisting"
unset -m sdfgsdfg
ret=$?
if [ $ret = 1 ]
then
   	echo "PASS"
   	let passno=passno+1
else
    echo "FAIL: Expected return value 1, returned $ret"
    let failno=failno+1
fi
}

# Each named parameter is unset using no option to unset an invalid parameter name
test_unset6()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset with no option to unset an invalid parameter name"
unset [
ret=$?
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, returned $ret"
    let failno=failno+1
fi
}

# A env variable name parameter is unset
test_unset7()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset env variable parameter name"
unset PATH
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, returned $ret"
    let failno=failno+1
fi
}

# A env variable name parameter is unset in restricted mode
test_unset8()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset env variable parameter name in restricted mode"
setopt restricted
unset PATH
ret=$?
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, returned $ret"
    let failno=failno+1
fi
}

# Export a parameter and then unset it
test_unset9()
{
let totalno=totalno+1
echo "Test $totalno: export a parameter and then unset it"
export a=1234
unset a
ret=$?

if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, returned $ret"
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
test_unset2
test_unset3
# test_unset4
test_unset5
# test_unset6
test_unset7
# test_unset8
test_unset9
report
