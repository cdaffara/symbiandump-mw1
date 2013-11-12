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

preamble()
{
mkdir cdtest
mkdir dirstest
}

test_cd1()
{
let totalno=totalno+1
echo "Test $totalno: cd with a "nonexistingdir" as argument, cdpath being set to ."
cdpath=.
cd popfdtest
ret=$?
if [ $ret -eq 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
fi
cdpath=
}

test_cd2()
{
let totalno=totalno+1
echo "Test $totalno: cd with a "nonexistingdir" as argument, cdpath being set to ./dirstest "
cdpath=./dirstest
cd popfdtest
ret=$?
if [ $ret -eq 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $ret"
    let failno=failno+1
fi
cdpath=
}

test_cd3()
{
let totalno=totalno+1
echo "Test $totalno: cd with a unix style directory as an argument"
cd cdtest
if [ $PWD = "C:\zsh\cdtest" ] || [ $PWD = "E:\zsh\cdtest" ] || [ $PWD = "Z:\zsh\cdtest" ]; then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected return value 1, but  returned $PWD"
    let failno=failno+1
fi
cdpath=
}

postamble()
{
cd ..
rmdir dirstest
rmdir cdtest
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
preamble
test_cd1
test_cd2
test_cd3
postamble
report

