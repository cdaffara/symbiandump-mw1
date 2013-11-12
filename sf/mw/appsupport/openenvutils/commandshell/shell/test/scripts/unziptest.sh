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
	let total=0;
	let pass=0;
	let fail=0;

	if [ ! -d c:\\ziptest ]
	then
		echo "Run minizip.sh before testing"
		exit
	else
		cd c:\\ziptest
	fi
	if [ -d .\\unziptest ]
	then
		rm -r .\\unziptest	
	fi
	mkdir .\\unziptest
	cd .\\unziptest
	mkdir .\dir2
	mkdir .\dir3
	mkdir .\dir
	mkdir .\dir1
	cd 
}

test_unzip1()
{
    let total+=1;
    echo "test_unzip1:unzip -d dir2-x ../pqr3.zip "
    #echo "test_unzip1:unzip -d dir2 -x ../pqr3.zip " >>unziplog.txt
    unzip  -o -d c:\\ziptest\\unziptest\\dir2  c:\\ziptest\\pqr3.zip 
    ret=$?
	if [ $ret -ne 0 ]
	then
		echo "FAIL : Return Value $ret"
		let fail+=1;
	else
		echo "PASS";
		let pass+=1;
	fi
}

test_unzip2()
{
    let total+=1;
    echo "test_unzip2:unzip -d dir3 -e ../pqr3.zip "

    #echo "test_unzip2:unzip -d dir3 -e ../pqr3.zip " >>unziplog.txt
    unzip  -o -d c:\\ziptest\\unziptest\\dir3  c:\\ziptest\\pqr3.zip 
    ret=$?
	if [ $ret -ne 0 ]
	then
		echo "FAIL : Return Value $ret"
		let fail+=1;
	else
		echo "PASS";
		let pass+=1;
	fi
}
test_unzip3()
{
    let total+=1;
    echo "test_unzip3:unzip -l ../pqr10.zip "

    unzip -l c:\\ziptest\\pqr3.zip 
	#unzip -l c:\\ziptest\\pqr3.zip > c:\\ziptest\\unziptest\\temp.txt
    ret=$?
	if [ $ret -ne 0 ]
	then
		echo "FAIL : Return Value $ret"
		let fail+=1;
		return
	else
		echo "PASS";
		let pass+=1
	fi
	
}
test_unzip4()
{
    let total+=1;
    echo "test_unzip4:unzip -d dir ../pqr10.zip "
    #echo "test_unzip4:unzip -d dir ../pqr10.zip " >>unziplog.txt
    unzip  -o -d c:\\ziptest\\unziptest\\dir c:\\ziptest\\pqr3.zip #>> unziplog.txt 2>> unziplog.txt
    ret=$?
	if [ $ret -ne 0 ]
	then
		echo "FAIL : Return Value $ret"
		let fail+=1
	else
		echo "PASS";
		let pass+=1;
	fi
}
test_unzip5()
{
    let total+=1;
    echo "test_unzip5:unzip -x -d dir1 ../pqr3.zip "
    #echo "test_unzip5:unzip -x -d dir1 ../pqr11.zip " >>unziplog.txt
    unzip -o -x -d c:\\ziptest\\unziptest\\dir1 c:\\ziptest\\pqr3.zip #>> unziplog.txt 2>> unziplog.txt
    ret=$?
	if [ $ret -ne 0 ]
	then
		echo "FAIL : Return Value $ret"
		let fail+=1;
		return
	else
		echo "PASS";
		let pass+=1;
	fi
}
test_unzip6()
{
    let total+=1;
    echo "test_unzip6:unzip -o ../pqr3.zip "
    #echo "test_unzip6:unzip -o ../pqr10.zip " >>unziplog.txt
    unzip -o c:\\ziptest\\pqr3.zip 
    ret=$?
	if [ $ret -ne 0 ]
	then
		echo "FAIL : Return Value $ret"
		let fail+=1;
		return
	else
		echo "PASS";
		let pass+=1;
	fi
}

report()
{
	echo "Changing the directory back";
	echo "--------------------unzip test results------------------------"
	echo " TOTAL TESTS : $total"
	echo " TOTAL PASS  : $pass"
	echo " TOTAL FAIL  : $fail"
	echo "------------------------------------------------------------"
}

#begin calls
init
test_unzip1
test_unzip2
test_unzip3
test_unzip4
test_unzip5
test_unzip6
report






