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
# Inititalise counters for Total tests, passes, fails.
#

preamble()
	{
	if [ ! -d TestDir ]
	then
		mkdir TestDir
	fi
	}

init()
	{
	let totalno=0;
	let passno=0;
	let failno=0;
	}


#Test: Tests dirs with -v option
test_dirs1()
{
	let totalno=totalno+1
	echo "Test $totalno: dirs with -v option"
	if [ -d TestDir ]
	then
		pushd TestDir
		ret=$?
		if [ $ret = 0 ]
		then
			dirs -v
			ret=$?
			if [ $ret = 0 ]
			then
				echo "PASS"
				let passno=passno+1
			else
				echo "dirs failed with -v option"
				echo "FAIL"
				let failno=failno+1
			fi 
		else
			echo "pushd failed with error $ret"
			let failno=failno+1
		fi
	else
		echo "TestDir is not created"
		echo "FAIL"
		let failno=failno+1 
	fi
	
	popd
}

#Test: Delete the directory stack with -c option
test_dirs2()
{
	let totalno=totalno+1
    echo "Test $totalno: Delete the directory stack with -c option"
	
	dirs -c
	ret=$?
	
	if [ $ret = 0 ]
	then
                echo "PASS"
                echo "directory stack deleted"
                let passno=passno+1
            else
                echo "dirs failed with -c option"
                echo "FAIL"
                let failno=failno+1
            fi		

}


#Test: Test dirs with -p and +n option
test_dirs3()
{
    let totalno=totalno+1
    echo "Test $totalno: Test dirs with -p and +n option"

    dirs -p 2
    ret=$?

    if [ $ret = 0 ]
    		then
                echo "PASS"
                let passno=passno+1
            else
                echo "dirs failed with -p option"
                echo "FAIL"
                let failno=failno+1
            fi

}

#Test: Test dirs with -v and +n option
test_dirs4()
{
    let totalno=totalno+1
    echo "Test $totalno: Test dirs with -v and +n option"

    dirs -v 1
    ret=$?

    if [ $ret = 0 ]
    then
                echo "PASS"
                let passno=passno+1
            else
                echo "dirs failed with -v and  option"
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


postamble()
	{
	rmdir TestDir 
	}


preamble
init

 test_dirs1
 test_dirs2
 test_dirs3
 test_dirs4

report
postamble
