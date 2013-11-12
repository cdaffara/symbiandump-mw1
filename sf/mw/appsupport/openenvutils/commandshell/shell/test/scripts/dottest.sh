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

#Test: Using dot, try running a script file
test_dot1()
{
    let totalno=totalno+1
    echo "Test $totalno: Test dirs with -v and +n option"

    . ./util_dot1.sh
    ret=$?

    if [ $ret = 0 ]
    then
                echo "PASS"
                let passno=passno+1
            else
                #echo "dirs failed with -v and  option"
                echo "FAIL"
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
 test_dot1
report


