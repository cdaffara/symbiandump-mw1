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

# Test: to check the range of history mentioning option 'n'
test_fc2()
{
let totalno=totalno+1
echo "Test $totalno: To check the range of history mentioning option n"

TestCmd=`fc -ln -1 -5`
ret=$?
echo "Return value is:$ret"

if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}


#Test: To check the range of command history with strings
test_fc3()
{
let totalno=totalno+1
echo "Test $totalno: To check the command history with strings as options"

#pwd
#fc -l -1 -3
TestCmd=`fc -l "pwd" "fc"`
ret=$?

echo "Return value is: $ret"

if [ $ret = 1 ]
then
	echo "PASS"
	let passno=passno+1
else
	echo "FAIL"
	let failno=failno+1
fi
}

#Test: fc prints command history in reverse order
test_fc4()
{
let totalno=totalno+1
echo "Test $totalno: fc prints the command history in reverse order"

TestCmd=`fc -lr -1 -3`
ret=$?

if [ $ret = 0 ]
then
	echo "PASS"
	let passno=passno+1
else
	echo "FAIL"
	let failno=failno+1
fi


}


#Test: fc prints command history matching the pattern entered
test_fc5()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history matching the patteen entered"

TestCmd=`fc -lm "cd"`
ret=$?

if [ $ret = 0 ]
then
	echo "PASS"
	let passno=passno+1
else
	echo "FAIL"
	let failno=failno+1
fi


}

#Test: fc prints command history with date time stamp in dd.mm.yyyy format
test_fc6()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history with date time stamp in dd.mm.yyyy format"

TestCmd=`fc -ldfE -1 -7`
ret=$?

if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi


}


#Test: fc prints command history with date time stamp in yyyy-mm-dd format
test_fc7()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history with date time stamp in yyyy-mm-dd format"

TestCmd=`fc -ldfi -1 -7`
ret=$?

if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi


}

#Test: fc prints command history with date time stamp in yyyy-mm-dd format along with elapsed time
test_fc8()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history with date time stamp in yyyy-mm-dd format along with elapsed time"

TestCmd=`fc -ldfiD -1 -7`
ret=$?

if [ $ret = 0 ]
then 
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi


}


#Test: fc prints command history into a file
test_fc9()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history into a file"

# Setting the command history to 5
SAVEHIST=5			
TestCmd=`fc -WI CmdHist.txt`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
	echo "PASS"
	let passno=passno+1
else
	echo "FAIL"
	let failno=failno+1
fi

}

#Test: fc prints command history into a file with repetition
test_fc10()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history into a file with repetition"

# Setting the command history to 5
SAVEHIST=5
TestCmd=`fc -W CmdHist.txt`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}


#Test: fc prints command history into a file in append mode
test_fc11()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history into a file in append mode"

# Setting the command history to 5
SAVEHIST=5
TestCmd=`fc -AI CmdHist.txt`
ret=$?

echo "TC returns $ret"
if [ $ret = 0  ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}

#Test: fc prints command history into a file in append mode with repetition
test_fc12()
{
let totalno=totalno+1
echo "Test $totalno: fc prints command history into a file in append mode with repetition"

# Setting the command history to 5
SAVEHIST=5
TestCmd=`fc -A CmdHist.txt`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}

#Test: fc reads command history from a file with repetition
test_fc13()
{
let totalno=totalno+1
echo "Test $totalno: fc reads command history from a file with repetition"

# Setting the command history to 5
SAVEHIST=5
TestCmd=`fc -R CmdHist.txt`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}

#Test: fc reads command history from a file without repetition 
test_fc14()
{
let totalno=totalno+1
echo "Test $totalno: fc reads command history from a file without repetition"

# Setting the command history to 5
SAVEHIST=5
TestCmd=`fc -RI CmdHist.txt`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}

#Test: fc returns 'Too many arguments' when P is given as option
test_fc15()
{
let totalno=totalno+1
echo "Test $totalno: fc returns 'Too many arguments' when P is given as option"

TestCmd=`fc -lP -1 -5`
ret=$?

echo "TC returns $ret"
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}

#Test: fc returns 'bad option' on supplying invalid option
test_fc16()
{
let totalno=totalno+1
echo "Test $totalno: fc returns 'bad option' on supplying invalid option"

TestCmd=`fc -lX -1 -5`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
 	echo "FAIL"	
    let failno=failno+1
else
	echo "PASS"
    let passno=passno+1
fi

}

#Test: fc with -p option. This TC is for coverage improvement
test_fc17()
{
let totalno=totalno+1
echo "Test $totalno: fc with -p option. This TC is for coverage improvement"

TestCmd=`fc -lp -1 -5`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}

#Test: Try reading command history from a non existing file 
test_fc18()
{
let totalno=totalno+1
echo "Test $totalno: Try reading command history from a non existing file"

TestCmd=`fc -R NoFile.txt`
ret=$?

echo "TC returns $ret"
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL"
    let failno=failno+1
fi

}

postamble()
	{
	rm CmdHist.txt
	}


report()
	{
	echo "---------------------------------------------------------"
	echo "Total tests	: $totalno"
	echo "Passed tests	: $passno"
	echo "Failed tests	: $failno"
	echo "---------------------------------------------------------"
	}

init
test_fc2
# test_fc3
 test_fc4   
 test_fc5
test_fc6
test_fc7
test_fc8
 test_fc9
 test_fc10
 test_fc11
 test_fc12
 test_fc13
 test_fc14
 test_fc15  #Defect No 44 raised  
 test_fc16  #Defect No 44 raised
 test_fc17  #Defect No 43 raised
#  test_fc18  #Defect No 44 raised
report
postamble
