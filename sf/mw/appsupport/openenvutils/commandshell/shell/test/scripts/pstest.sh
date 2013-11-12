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


test_ps1()
{

	let totalno+=1;
	echo "--- test_ps1:ps without options---"
	#echo "--- test_ps1:ps without options---" > pslog.txt
	ps #>> pslog.txt 2>> pslog.txt 
	ret=$?
	
	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1		
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}

test_ps2()
{
	
	let totalno+=1;
	echo "---test_ps2:ps with invalid options---"
	#echo "---test_ps2:ps with invalid options---" >> pslog.txt
	ps -q  #>> pslog.txt 2>> pslog.txt
	ret=$?

	if [ $ret -ne 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL, Return Value : $ret"
		let failno+=1
	fi
}

test_ps3()
{
	let totalno+=1
	echo "test_ps3:ps -l (with valid option)"
	#echo "test_ps3:ps -l(with valid option)" >> pslog.txt
	ps -l #>> pslog.txt 2>> pslog.txt
	ret=$?
			
	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}


test_ps4()
{
	let totalno+=1
	echo "ps_test4:ps -o with valid column"
	#echo "ps_test4:ps -o with valid column" >> pslog.txt
	ps -o time,args,comm #>> pslog.txt 2>> pslog.txt
	ret=$?
	
	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL, Return Value : $ret"
		let failno+=1
	fi
}

test_ps5()
{
	let totalno+=1
	echo "test_ps5:ps -o with invalid column"
	#echo "test_ps5:ps -o with invalid column" >>pslog.txt
	ps -o abc,def #>> pslog.txt 2>> pslog.txt
	ret=$?

	if [ $ret -ne  0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}

test_ps6()
{
	let totalno+=1;
	echo "test_ps6:ps -o with both valid and invalid column"
	#echo "test_ps6:ps -o with both valid and invalid column" >>pslog.txt
	ps -o time
	ret=$?

	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}

test_ps7()
{
	let totalno+=1
	echo "test_ps7:ps -f (with valid option)"
	#echo "test_ps7:ps -f(with valid option)" >> pslog.txt
	ps -f #>> pslog.txt 2>> pslog.txt
	ret=$?
			
	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}
test_ps8()
{
	let totalno+=1
	echo "test_ps8:ps -s (with valid option)"
	#echo "test_ps8:ps -s(with valid option)" >> pslog.txt
	ps -s #>> pslog.txt 2>> pslog.txt
	ret=$?
			
	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}
test_ps9()
{
	let totalno+=1
	echo "test_ps9:ps -h (with valid option)"
	#echo "test_ps9:ps -h(with valid option)" >> pslog.txt
	ps -h #>> pslog.txt 2>> pslog.txt
	ret=$?
			
	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}
test_ps10()
{
	let totalno+=1
	echo "test_ps10:ps -lf (with valid option)"
	#echo "test_ps10:ps -lf(with valid option)" >> pslog.txt
	ps -lf #>> pslog.txt 2>> pslog.txt
	ret=$?
			
	if [ $ret -eq 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}
test_ps11()
{
	let totalno+=1
	echo "test_ps11:ps -lf -o time(with invalid option)"
	#echo "test_ps10:ps -lf -o time(with invalid option)" >> pslog.txt
	ps -lf -o time #>> pslog.txt 2>> pslog.txt
	ret=$?
			
	if [ $ret -ne 0 ]
	then 
			echo "PASS"
			let passno+=1
	else
		echo "FAIL,Return Value : $ret"
		let failno+=1
	fi
}

report()
{
	echo "--------------------ps test results------------------------"
	echo " TOTAL TESTS : $totalno"
	echo " TOTAL PASS  : $passno"
	echo " TOTAL FAIL  : $failno"
	echo "------------------------------------------------------------"
}

#begin making the calls
{
init
test_ps1
test_ps2
test_ps3
test_ps4
test_ps5
test_ps6
test_ps7
test_ps8
test_ps9
test_ps10
test_ps11
report
}
