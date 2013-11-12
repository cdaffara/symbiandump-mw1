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

test_redirect2()
{
    let totalno=totalno+1
    echo "Test $totalno: Test redirection of text file to stdin  and stdout of a exe using < and >"

    echo "456789" > C:\\in.txt
    tredirect < C:\\in.txt > C:\\out.txt
    
    read temp < c:\\out.txt
    
    if [ $temp = 456789 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL"
        let failno=failno+1
    fi
                 

}

test_redirect1()
{
    let totalno=totalno+1
    echo "Test $totalno: Test redirection of text file to stdin  of a exe using < "

    echo "456789" >  C:\\in.txt
    
    tredirect < C:\\in.txt 
    ret=$?

    if [ $ret = 0 ]
    then
         echo
         echo "Is previous output on the screen 456789\(y\/n\):"
         read yesorno 
         if [ $yesorno = "y" ]
         then
              echo "PASS"
              let passno=passno+1
         else
              echo "FAIL"
              let failno=failno+1
         fi
    else
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
test_redirect1
test_redirect2
report
rm -f  C:\\in.txt  C:\\out.txt


