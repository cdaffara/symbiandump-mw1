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


# Read the arguments as input to the shell and execute the resulting command in the current shell process with EVALLINENO option unset
test_eval2()
{
let totalno=totalno+1
echo "Test $totalno: eval to run a command with EVALLINENO option unset"
unsetopt EVALLINENO
evalvar=pwd
outputvar=`eval $evalvar`
ret=$?
if [ $ret = 0 ]
then
    if [ $PWD = $outputvar ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $PWD, returned $outputvar"
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
setopt EVALLINENO
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
test_eval2
report
