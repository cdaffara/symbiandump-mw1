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
	let total=0
	let pass=0
	let fail=0

	if [ -d c:\\ziptest ]
	then
		rm -r c:\\ziptest
	fi
	mkdir c:\\ziptest
	cd c:\\ziptest
	echo "Change directory to ziptest"	
	echo "kjhdlaskdjsd dsakjdas
	sajdkaksjdasd
	sdklsjdasds" > abc.txt
	echo "dsdfdsfds
	dfsdfd " > def.txt
	echo " " > ziplog.txt
	if [ -d .\\test1 ]
	then
		rm -r c:\\ziptest
	fi

	mkdir c:\\test1
	cd c:\\test1
	echo "kjhdlaskdjsd dsakjdas
	sajdkaksjdasd
	sdklsjdasds" > abc1.txt
	echo "dsdfdsfds
	dfsdfd " > def.txt
	cd 
}		

test_zip2()
{
	let total+=1;
	echo "test_zip2:Zip with no options and zip file argument"

	#echo "test_zip2:Zip with no options and zip file argument" >> ziplog.txt
	zip c:\\ziptest\\abc.zip #>> ziplog.txt 2>> ziplog.txt
	#returns 0 instead of 1
	ret=$?
	if [ $ret -eq 0 ]
	then
		echo "PASS"
		let pass+=1;
	else
		echo "FAIL\nReturn Value $ret"
		let fail+=1;
	fi
}

test_zip1()
{
	let total+=1;
	echo "test_zip1:Zip with no options and no arguments"

	#echo "test_zip1:Zip with no options and no arguments" > ziplog.txt
	zip 
	#redirection hangs
	#returns 0 instead of 1
	ret=$?
	if [ $ret -ne 0 ]
	then
		echo "FAIL\nExpected Value : 1 Return Value:$ret"
		let fail+=1;
	else
		echo "PASS"
		let pass+=1;
	fi
}

test_zip3()
{
	let total+=1;
	echo "test_zip3:Zip pqr3.zip abc.txt def.txt "
	
	#echo "test_zip3:Zip pqr3.zip abc.txt def.txt " >> ziplog.txt
	zip c:\\ziptest\\pqr3.zip c:\\ziptest\\abc.txt c:\\ziptest\\def.txt #>> ziplog.txt 2>> ziplog.txt
	ret=$?
	if [ -e c:\\ziptest\\pqr3.zip -a  $ret -eq 0 ]
 	then
		echo "Pass";
		let pass+=1;
	else
		echo "Fail"
		let fail+=1;
	fi
}
test_zip4()
{
	let total+=1;
	echo " test_zip4 : zip -0 pqr4.zip abc.txt def.txt"
	
	#echo " test_zip4 : zip -0 pqr4.zip abc.txt def.txt" >> ziplog.txt
	 zip -0 c:\\ziptest\\pqr4.zip c:\\ziptest\\abc.txt c:\\ziptest\\def.txt #>> ziplog.txt 2>> ziplog.txt
	 ret=$?

	if [ -e c:\\ziptest\\pqr4.zip ]
	then
		echo "PASS : Zip file created"
		let pass+=1;
	else
		echo "FAIL\nExpected Value:1  Return Value:$ret"
		let fail+=1;
	fi
}

test_zip10()
{
	let total+=1;
	echo " test_zip10 : zip -5 pqr10.zip abc.txt def.txt"
	
	#echo " test_zip10 : zip -5 pqr10.zip abc.txt def.txt" >> ziplog.txt
	 zip -5 c:\\ziptest\\pqr10.zip c:\\ziptest\\abc.txt c:\\ziptest\\def.txt #>> ziplog.txt 2>> ziplog.txt
	 ret=$?

	if [ -e c:\\ziptest\\pqr10.zip ]
	then
		echo "PASS : Zip file created"
		let pass+=1;
	else
		echo "FAIL\nExpected Value:1  Return Value:$ret"
		let fail+=1;
	fi
}

test_zip5()
{
	let total+=1;
	echo " test_zip5 : zip -20 pqr5.zip abc.txt def.txt"
	#echo " test_zip5 : zip -20 pqr5.zip abc.txt def.txt" >> ziplog.txt
	# on linux compresses 0% but doesnt fail	
	 zip -20 c:\\ziptest\\pqr5.zip c:\\ziptest\\abc.txt c:\\ziptest\\def.txt 
	 ret=$?

	if [ -e c:\\ziptest\\pqr5.zip -a  $ret -eq 0 ]
	then
		echo "PASS : Zip file created"
		let pass+=1;
	else
		echo "FAIL\n Return Value:$ret"
		let fail+=1;
	fi
}

test_zip6()
{
	let total+=1;
	echo " test_zip6 : zip -o pqr5.zip abc.txt "
	#echo " test_zip6 : zip -o pqr5.zip abc.txt " >> ziplog.txt
	zip -o c:\\ziptest\\pqr5.zip c:\\ziptest\\abc.txt #>> ziplog.txt 2>> ziplog.txt
	 ret=$?

	if [ -e c:\\ziptest\\pqr5.zip -a $ret -eq 0 ]
	then
		echo "PASS : Zip file created"
		let pass+=1;
	else
		echo "FAIL\n Return Value:$ret"
		let fail+=1;
	fi
}


test_zip7()
{
	let total+=1;
	echo " test_zip7 : zip -a pqr5.zip def.txt "
	#echo " test_zip7 : zip -a pqr5.zip def.txt " >> ziplog.txt
	zip -a c:\\ziptest\\pqr5.zip c:\\ziptest\\def.txt #>> ziplog.txt 2>> ziplog.txt
	 ret=$?

	if [ -e c:\\ziptest\\pqr5.zip -a  $ret -eq 0 ]
	then
		echo "PASS : Zip file created"
		let pass+=1;
	else
		echo "FAIL\n Return Value:$ret"
		let fail+=1;
	fi
}

test_zip11()
{
	let total+=1;
	echo "test_zip11:Zip pqr11.zip abc.txt def.txt test\*";
	zip -o c:\\ziptest\\pqr3.zip c:\\ziptest\\abc.txt c:\\ziptest\\def.txt 
	ret=$?
	if [ -e c:\\ziptest\\pqr3.zip -a  $ret -eq 0 ]
 	then
		echo "Pass";
		let pass+=1;
	else
		echo "Fail"
		let fail+=1;
	fi
}

report()
{
	echo "Changing the directory back";
	echo "--------------------zip test results------------------------"
	echo " TOTAL TESTS : $total"
	echo " TOTAL PASS  : $pass"
	echo " TOTAL FAIL  : $fail"
	echo "------------------------------------------------------------"
}

#begin calls

init
test_zip1
test_zip2
test_zip3
test_zip4
test_zip5
test_zip6
test_zip7
test_zip10
test_zip11
report
