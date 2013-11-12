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


test_unsetopt2()
{
let totalno=totalno+1
echo "Test $totalno: unsetopt interactive"
unsetopt ineractive
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected 0, returned $ret"
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
test_unsetopt2
report
