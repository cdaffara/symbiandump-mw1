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
    let totalno=0
    let passno=0
    let failno=0
}


#Test: Tests whence with -w option
test_whence1()
{
    let totalno=totalno+1
    echo "Test $totalno: Tests whence with -w option"
    whence -w echo
    ret=$?

    if [ $ret = 0 ]
    then
        {
        echo "PASS"
        let passno=passno+1
        }
    else
        {
        echo "whence fails with option '-w'"
        echo "FAIL"
        let failno=failno+1
        }
    fi
}

#Test: Tests whence with -v option
test_whence2()
{
    let totalno=totalno+1
    echo "Test $totalno: Tests whence with -w option"
    whence -v echo
    ret=$?

    if [ $ret = 0 ]
    then
        {
        echo "PASS"
        let passno=passno+1
        }
    else
        {
        echo "whence fails with option '-v'"
        echo "FAIL"
        let failno=failno+1
        }
    fi
}

#Test: Tests whence with -f option
test_whence3()
{
    let totalno=totalno+1
    echo "Test $totalno: Tests whence with -w option"
    whence -f echo
    ret=$?

    if [ $ret = 0 ]
    then
        {
        echo "PASS"
        let passno=passno+1
        }
    else
        {
        echo "whence fails with option '-f'"
        echo "FAIL"
        let failno=failno+1
        }
    fi
}

#Test: Tests whence with -m option
test_whence4()
{
    let totalno=totalno+1
    echo "Test $totalno: Tests whence with -m option"
    whence -m 'whence'
    ret=$?

    if [ $ret = 0 ]
    then
        {
        echo "PASS"
        let passno=passno+1
        }
    else
        {
        echo "whence fails with option '-m'"
        echo "FAIL"
        let failno=failno+1
        }
    fi
}

#Test: Tests whence with -p option
test_whence5()
{
    let totalno=totalno+1
    echo "Test $totalno: Tests whence with -p option"
    res=`whence -p dklfsg`
    ret=$?
    echo $res
	if [ $ret = 1 ] && [ -z $res ]
    then
        {
        echo "PASS"
        let passno=passno+1
        }
    else
        {
        echo "whence fails with option '-p'"
        echo "FAIL"
        let failno=failno+1
        }
    fi
}

#Test: Tests whence with -a option and bad argument
test_whence6()
{
    let totalno=totalno+1
    echo "Test $totalno: Tests whence with -p option"
    whence -a World
    ret=$?

    if [ $ret = 0 ]
    then
        {
        echo "PASS"
        let passno=passno+1
        }
    else
        {
        echo "whence fails with option '-a'"
        echo "FAIL"
        let failno=failno+1
        }
    fi
}


report()
    {
    echo "---------------------------------------------------------"
    echo "Total tests   : $totalno"
    echo "Passed tests  : $passno"
    echo "Failed tests  : $failno"
    echo "---------------------------------------------------------"
    }


init
 test_whence1
 test_whence2
 test_whence3
 test_whence4
 test_whence5
 test_whence6
report
