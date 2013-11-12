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

#Test: Tests the -q option of read
test_read1()
{
	let totalno=totalno+1
	echo "Test $totalno: Tests the -q option of read" > result_read.txt
	echo "Type 'y' or 'n':"
	read -q buf
	ret=$?

	if [ $ret = 0 ]
	then
		let passno=passno+1
	elif [ $ret = 1 ]
	then
		echo "read sets 'buf' to '$buf'" >> result_read.txt
		echo "PASS" >> result_read.txt
        let passno=passno+1
	else
		echo "read fails with option '-q'" >> result_read.txt
		echo "FAIL" >> result_read.txt
		let failno=failno+1
	fi
}


#Test: Tests the -s option of read. This option does not echo back the user input
test_read2()
{	
	let totalno=totalno+1
	echo "Test $totalno:  Tests the -s option of read. This option does not echo back the user input" >> result_read.txt
	echo "Type \"data\":" 
	read -s buf
	ret=$?

	if [ $ret = 0 ]
	then
		echo "Were the characters echoed back? Press Y or N:"
		TestCmd=`read -q choice`
		ret=$?
		if [ $ret = 0 ]
		then
			echo "Characters are echoed back to the console" >> result_read.txt
    	    echo "FAIL" >> result_read.txt
        	let failno=failno+1
		else
			if [ ! -z $buf ] && [ $buf = "data" ]
			then
				echo "User entered \"$buf\"" >> result_read.txt
				echo "PASS" >> result_read.txt
            	let passno=passno+1
			else
				echo "User entry does not match \"data\"" >> result_read.txt
				echo "FAIL" >> result_read.txt
				let failno=failno+1
			fi
		fi
	else
		let failno=failno+1
		echo "read failed with s option" >> result_read.txt
		echo "FAIL" >> result_read.txt
	fi
}

#Test: Tests the read command with -e option
test_read3()
{
	let totalno=totalno+1
	buf=					# To initialise buf to NULL
	echo "Test $totalno: Tests the read command with -e option" >> result_read.txt
	echo "data" | read -e buf
	ret=$?
	
	if [ $ret = 0 ]
	then
		echo "Was the data echoed back?: Input Y or N"
		read -q choice
		ret=$?
		if [ $ret = 0 ]
		then
			if [ -z $buf ]
			then
				echo "PASS" >> result_read.txt
				let passno=passno+1
			else
				echo "read with '-e' option stores \"data\" in buf" >> result_read.txt
				echo "FAIL" >> result_read.txt
				let failno=failno+1
			fi
		else
			echo "\"data\" was not echoed back to screen. read command failed with -e option" >> result_read.txt
			echo "FAIL" >> result_read.txt
			let failno=failno+1
		fi
	else
		echo "read failed with '-e' option" >> result_read.txt
		echo "FAIL" >> result_read.txt
		let failno=failno+1
	fi	
}

#Test: Tests the read command with '-E' option
test_read4()
{
	let totalno=totalno+1
	buf=				# To initialise buf to NULL
	echo "Test $totalno: Tests the read command with '-E' option" >> result_read.txt
	echo "data" | read -E buf
	ret=$?
	if [ $ret = 0 ]
	then
		echo "Was \"data\" echoed back to screen? Type 'Y' or 'N':" >> result_read.txt
		read -q choice
		ret=$?
		if [ $ret = 0 ]
		then
			if [ ! -z $buf ] && [ $buf = "data" ]
			then
				echo "PASS" >> result_read.txt
				let passno=passno+1 
			else
				echo "'buf' does not hold the value \"data\"" >> result_read.txt
				echo "FAIL" >> result_read.txt
				let failno=failno+1
			fi
		else
			echo "\"data\" was not echoed back to screen. read command failed with -E option" >> result_read.txt
            echo "FAIL" >> result_read.txt
            let failno=failno+1
		fi	
	else
		echo "read failed with '-E' option" >> result_read.txt
		echo "FAIL" >> result_read.txt
		let failno=failno+1
	fi	
}

#Test: Tests the -A option of the read command
test_read5()
{
    let totalno=totalno+1
    buf=
    echo "Test $totalno: Tests the -A option of the read command" >> result_read.txt
    echo "data1 data2 data3 data4" | read -A buf
    ret=$?
    if [ $ret = 0 ]
    then
		if [ ! -z $buf ] &&  [ ${buf[1]} = "data1" ] && [ ${buf[2]} = "data2" ] && [ ${buf[3]} = "data3" ]
		then
        	echo "PASS" >> result_read.txt
        	let passno=passno+1
		else
			echo "FAIL" >> result_read.txt
			let failno=failno+1
		fi
    else
        echo "read fails with '-A' option" >> result_read.txt
        echo "FAIL" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests the -A option with bad argument
test_read5A()
{
    let totalno=totalno+1
    unset buf
	unset buf1
    echo "Test $totalno: Tests the -A option with bad argument" >> result_read.txt
    echo "data1 data2 data3 data4" | read -A buf buf1
    ret=$?
    if [ $ret = 1 ]
    then
		echo "PASS" >> result_read.txt
		let passno=passno+1
    else
        echo "Expected return value was 1, but read returned $ret" >> result_read.txt
        echo "FAIL" >> result_read.txt
        let failno=failno+1
    fi
}


#Test: Tests read without mentioning the buffer to store the user input scalars
test_read6()
{
	let totalno=totalno+1
	REPLY=
	echo "Test $totalno: Tests read without mentioning the buffer to store the user input scalars" >> result_read.txt
	echo "data" | read 
	ret=$?
	
	if [ $ret = 0 ]
	then
		if [ ! -z $REPLY ] && [ $REPLY = "data" ]
		then
			echo "PASS" >> result_read.txt
			let passno=passno+1
		else
			echo "REPLY does not store \"data\"" >> result_read.txt
			echo "FAIL" >> result_read.txt
			let failno=failno+1
		fi	
	else
		echo "read fails when buffer not mentioned" >> result_read.txt
		echo "FAIL" >> result_read.txt
		let failno=failno+1
	fi
}

#Test: Tests read without mentioning the buffer to store the user input arrays
test_read6A()
{
    let totalno=totalno+1
    unset reply
    echo "Test $totalno: Tests read without mentioning the buffer to store the user input arrays" >> result_read.txt
    echo "data1 data2 data3" | read -A 
    ret=$?

    if [ $ret = 0 ]
    then
		if [ ! -z $reply ] &&  [ ${reply[1]} = "data1" ] && [ ${reply[2]} = "data2" ] && [ ${reply[3]} = "data3" ]
        then
            echo "PASS" >> result_read.txt
		    let passno=passno+1
        else
            echo "reply does not store \"data1 data2 data3\"" >> result_read.txt
			echo "$reply" >> result_read.txt
            echo "FAIL" >> result_read.txt
            let failno=failno+1
        fi
    else
        echo "read fails when buffer not mentioned for arrays" >> result_read.txt
        echo "FAIL" >> result_read.txt
        let failno=failno+1 
    fi
}




#Test: Tests read with multiple buffers
test_read7()
{
	let totalno=totalno+1
	echo "Test $totalno: Tests read wih multiple buffers" >> result_read.txt
	buf1=
	buf2=
	buf3=
	buf4=
	echo "data1 data2 data3 data4" | read buf1 buf2 buf3 buf4
	ret=$?
	
	if [ $ret = 0 ]
	then
		if [ ! -z $buf1 ] && [ ! -z $buf2 ] && [ ! -z $buf3 ] && [ ! -z $buf4 ] && [ $buf1 = "data1" ] && [ $buf2 = "data2" ] && [ $buf3 = "data3" ] && [ $buf4 = "data4" ]
		then
			echo "PASS" >> result_read.txt
			let passno=passno+1
		else
			echo "read does not store user input in corresponding buffers" >> result_read.txt
			echo "FAIL" >> result_read.txt
			let failno=failno+1
		fi
	else
		echo "read failed with multiple buffers for user input" >> result_read.txt
		echo "FAIL" >> result_read.txt
		let failno=failno+1
	fi

}

#Test: Tests read with -k option
test_read8()
{
	let totalno=totalno+1
	buf=
	echo "Test $totalno: Tests read with -k option" >> result_read.txt
	echo "Enter only 5 letters"
	read -k5 buf
	ret=$?	

	if [ $ret = 0 ]
	then
		echo "PASS" >> result_read.txt
		let passno=passno+1
	else
		echo "FAIL" >> result_read.txt
		echo "read failed with '-k' option" >> result_read.txt
	fi 
}

#Test: Tests read with combination of options
test_read10()
{
	let totalno=totalno+1
    buf=
    echo "Test $totalno: Tests read with combination of options" >> result_read.txt
	echo "Hello Hi World" | read -AE buf    
	ret=$?

	if [ $ret = 0 ]
    then
		echo "Was \"Hello Hi World\" echoed to the console? Type [y/n]:"
		read -q choice
		ret=$?
	
		if [ $ret = 0 ]
		then
			echo "PASS"	 >> result_read.txt
			let passno=passno+1
		else
			echo "\"Hello Hi World\" was not echoed to the console" >> result_read.txt
			echo "FAIL" >> result_read.txt
			let failno=failno+1
		fi
    else
        echo "FAIL" >> result_read.txt
        echo "read failed with '-AE' option" >> result_read.txt
		let failno=failno+1
    fi
}


#Test: Tests read with -t option
test_read11()
{
    let totalno=totalno+1
    buf=
    echo "Test $totalno: Tests read with -t option" >> result_read.txt
	echo "Enter \"data\" within next 5 seconds. Failing to which, console is returned"
    read -t5 buf
    ret=$?

    if [ $ret = 0 ]
    then
		echo "PASS" >> result_read.txt
		echo "You entered \"data\" in time" >> result_read.txt
        let passno=passno+1
    elif [ $ret = 1 ]
	then
        echo "PASS" >> result_read.txt
		echo "You failed to enter \"data\" in time" >> result_read.txt
		let passno=passno+1
	else
		echo "FAIL" >> result_read.txt
		echo "read failed with -t option" >> result_read.txt
		let failno=failno+1
    fi
}

#Test: Tests read with -u option
test_read12()
{
    let totalno=totalno+1
    buf=
    echo "Test $totalno: Tests read with -u option" >> result_read.txt
	echo "Enter \"data\":"
    read -u 0 buf		# file descriptor 0 is used for std input
    ret=$?

    if [ $ret = 0 ]
    then
		if [ ! -z $buf ] && [ $buf = "data" ]
		then
			echo "PASS" >> result_read.txt
        	let passno=passno+1
		else
			echo "FAIL" >> result_read.txt
			echo "buf does not contain \"data\"" >> result_read.txt
			let failno=failno+1
		fi
    else
        echo "FAIL" >> result_read.txt
		echo "read failed with -u option" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests read with -d option
test_read13()
{
    let totalno=totalno+1
    buf=
    echo "Test $totalno: Tests read with -d option" >> result_read.txt
    echo "data @" | read -d @ buf      
    ret=$?

    if [ $ret = 0 ]
    then
        if [ ! -z $buf ] && [ $buf = "data" ]
        then
            echo "PASS" >> result_read.txt
            let passno=passno+1
        else
            echo "FAIL" >> result_read.txt
            echo "buf does not contain \"data\"" >> result_read.txt
            let failno=failno+1
        fi
    else
        echo "FAIL" >> result_read.txt
        echo "read failed with -d option" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests read with -z option
test_read14()
{
	let totalno=totalno+1
	buf=
	echo "Test $totalno: Tests read with -z option" >> result_read.txt
	print -z data; read -z buf
	ret=$?

	if [ $ret = 0 ]
	then
		if [ ! -z $buf ] && [ $buf = "data" ]
		then
			echo "PASS" >> result_read.txt
			let passno=passno+1
		else
			echo "buf does not contain \"Testing\"" >> result_read.txt
			echo "FAIL" >> result_read.txt
			let failno=failno+1
		fi
	else
		echo "FAIL" >> result_read.txt
		echo "read failed with -z option" >> result_read.txt
		let failno=failno+1
	fi
}

#Test: Tests read with invalid arguments for -c option
test_read15()
{
    let totalno=totalno+1
    buf=
    echo "Test $totalno: Tests read with invalid arguments for -c option" >> result_read.txt
    read -c buf
    ret=$?

    if [ $ret = 1 ]
    then
		echo "PASS" >> result_read.txt
		let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 1" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests read with invalid arguments for -lc option
test_read15A()
{
    let totalno=totalno+1
    buf=
    echo "Test $totalno: Tests read with invalid arguments for -lc option" >> result_read.txt
    read -lc buf
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS" >> result_read.txt
        let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 1" >> result_read.txt
        let failno=failno+1
    fi
}


#Test: Tests read with combination of -k and -t option
test_read16()
{
    let totalno=totalno+1
    unset buf
    echo "Test $totalno: Tests read with combination of -k and -t option" >> result_read.txt
	echo "Enter \"data\":"
    read -t5 -k4 buf
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS" >> result_read.txt
        let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 0" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests read with combination of -k and -u options
test_read17()
{
    let totalno=totalno+1
    unset buf
    echo "Test $totalno: Tests read with combination of -k and -u options" >> result_read.txt
    echo "Enter a key:"
    read -k1 -u 0 buf
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS" >> result_read.txt
        let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 0" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests read with combination of -k and -p
test_read18()
{
    let totalno=totalno+1
    unset buf
    echo "Test $totalno: Tests read with combination of -k and -p" >> result_read.txt
    read -k1 -p buf
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS" >> result_read.txt
        let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 1" >> result_read.txt
        let failno=failno+1
    fi
}


#Test: Tests read with combination of -k, -u  and -p
test_read19()
{
    let totalno=totalno+1
    unset buf
    echo "Test $totalno: Tests read with combination of -k, -u and -p" >> result_read.txt
    read -k1 -u 0 -p buf
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS" >> result_read.txt
        let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 1" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests read with combination of -p and -u without mentioning the file desrciptor
test_read20()
{
    let totalno=totalno+1
    unset buf
    echo "Test $totalno: Tests read with combination of -p and -u without mentioning the file desrciptor" >> result_read.txt
    read -u -p buf
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS" >> result_read.txt
        let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 1" >> result_read.txt
        let failno=failno+1
    fi
}


#Test: Tests read with invalid file descripto for -u option
test_read21()
{
    let totalno=totalno+1
    unset buf
    echo "Test $totalno: Tests read with invalid file descripto for -u option" >> result_read.txt
    read -u -10 buf
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS" >> result_read.txt
        let passno=passno+1
    else
        echo "FAIL" >> result_read.txt
        echo "read returned $ret, where expected return value was 1" >> result_read.txt
        let failno=failno+1
    fi
}

#Test: Tests the read command with -e and -A option
test_read22()
{
    let totalno=totalno+1
    buf=                    # To initialise buf to NULL
    echo "Test $totalno: Tests the read command with -e option" >> result_read.txt
    echo "data" | read -e -A buf
    ret=$?

    if [ $ret = 0 ]
    then
        echo "Was the data echoed back?: Input Y or N" >> result_read.txt
        read -q choice
        ret=$?
        if [ $ret = 0 ]
        then
            if [ -z $buf ]
            then
                echo "PASS" >> result_read.txt
                let passno=passno+1
            else
                echo "read with '-e' option stores \"data\" in buf" >> result_read.txt
                echo "FAIL" >> result_read.txt
                let failno=failno+1
            fi
        else
            echo "\"data\" was not echoed back to screen. read command failed with -e option" >> result_read.txt
            echo "FAIL" >> result_read.txt
            let failno=failno+1
        fi
    else
		echo "read failed with '-e' option" >> result_read.txt
        echo "FAIL" >> result_read.txt
        let failno=failno+1
    fi
}

report()
    {
    echo "---------------------------------------------------------"
    echo "Total tests   : $totalno"
    echo "Passed tests  : $passno"
    echo "Failed tests  : $failno"
    echo "---------------------------------------------------------"
    }


init
 test_read1
 test_read2  # Defect No:47
 test_read3
 test_read4
 test_read5
 test_read5A
 test_read6
 test_read6A
 test_read7
 test_read8
# test_read9 # Defect No 51. Add the body of the function once defect is fixed
 test_read10 # Defect No: 49
 test_read11
 test_read12
 test_read13
 test_read14
 test_read15
 test_read15A
 test_read16
 test_read17
 test_read18
 test_read19
 test_read20
 test_read21
 test_read22
report
