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

test_print2()
{
let totalno=totalno+1
echo "Test $totalno: print with a \"-aC <no_of_columns>\" option to print strings as <no_of_columns> columns per row"
print -aC 3 ab cd ef gh ij kl mn
ret=$?
if [ $ret = 0 ]
then
  echo "PASS"
  let passno=passno+1
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print3()
{
let totalno=totalno+1
echo "Test $totalno: print with a -ac option to print strings as columns"
print -ac 3 ab cd ef gh ij kl mn
ret=$?
if [ $ret = 0 ]
then
  echo "PASS"
  let passno=passno+1
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print4()
{
let totalno=totalno+1
echo "Test $totalno: print with a -o option to print strings in ascending order"
sortedarray=(`print -o z y`)
ret=$?
if [ $ret = 0 ]
then
  if [ $sortedarray[0] = y ] && [ $sortedarray[2] = z ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected y z, but  returned $sortedarray"
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print5()
{
let totalno=totalno+1
echo "Test $totalno: print with a -O option to print strings in descending order"
sortedarray=(`print -O y z`)
ret=$?
if [ $ret = 0 ]
then
  if [ $sortedarray[0] = z ] && [ $sortedarray[2] = y ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected z y, but  returned $sortedarray"
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print6()
{
let totalno=totalno+1
echo "Test $totalno: print with a -io option to print strings in ascending order case independently"
sortedarray=(`print -io Yy yY`)
ret=$?
if [ $ret = 0 ]
then
  if [ $sortedarray[0] = Yy ] && [ $sortedarray[2] = yY ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected Yy yY, but  returned $sortedarray"
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print7()
{
let totalno=totalno+1
echo "Test $totalno: print with a -iO option to print strings in descending order case independently"
sortedarray=(`print -iO yY Yy`)
ret=$?
if [ $ret = 0 ]
then
  if [ $sortedarray[0] = yY ] && [ $sortedarray[2] = Yy ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected yY Yy, but  returned $sortedarray"
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print8()
{
let totalno=totalno+1
echo "Test $totalno: print with a -l option to print strings in different lines"
print -l p q
ret=$?
if [ $ret = 0 ]
then
  echo "PASS"
  let passno=passno+1
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print9()
{
let totalno=totalno+1
echo "Test $totalno: print with a -m option to print strings not containing a pattern"
array=(`print -m'a' j a j j`)
ret=$?
if [ $ret = 0 ]
then
  if [ $array[0] = j ] && [ $array[2] = j ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected j j, but  returned $array"
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print10()
{
let totalno=totalno+1
echo "Test $totalno: print with a -n option to print strings without inserting a line feed"
array=(`print -n a b`)
ret=$?
if [ $ret = 0 ]
then
  if [ $array[0] = a ] && [ $array[2] = b ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected a b, but  returned $array"
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print11()
{
let totalno=totalno+1
echo "Test $totalno: print with a -N option to print strings separated and terminated by nulls"
res=(`print -N s y m b i a n`)
ret=$?
if [ $ret = 0 ]
then
  if [ $res[0] = s ] && [ $res[2] = y ] && [ $res[3] = m ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected symbian, but  returned \"$res\""
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}


test_print12()
{
let totalno=totalno+1
echo "Test $totalno: print with a -P option to expand prompt"
res=`print -P %c`
ret=$?
if [ $ret = 0 ]
then
  if [ $res = `pwd` ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"$cwd\", but  returned \"$res\""
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print13()
{
let totalno=totalno+1
echo "Test $totalno: print with a -r option to ignore escape conventions of echo"
res=`print -r hello`
ret=$?
if [ $ret = 0 ]
then
  if [ $res = hello ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected hello, but  returned \"$res\""
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print14()
{
let totalno=totalno+1
echo "Test $totalno: print with a -R option to emulate BSD print which does not process escape sequences unless -e option is given"
res=(`print -R -P %c`)
ret=$?
if [ $ret = 0 ]
then
  if [ $res[0] = -P ] && [ $res[2] = %c ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected -P %c, but  returned \"$res\""
        let failno=failno+1
    fi
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

test_print15()
{
let totalno=totalno+1
echo "Test $totalno: print with a -u option without file descriptor value"
print -u hello
ret=$?
if [ $ret = 1 ]
then
  echo "PASS"
  let passno=passno+1
 else
  echo "FAIL: Expected return value 1, but  returned $ret"
  let failno=failno+1
fi
}

test_print16()
{
let totalno=totalno+1
echo "Test $totalno: print with a -u option to write the argument string to a file descriptor"
print -u 2 hello
ret=$?
if [ $ret = 0 ]
then
   echo "PASS"
   let passno=passno+1
 else
  echo "FAIL: Expected return value 0, but  returned $ret"
  let failno=failno+1
fi
}

#Test: Test print with -s option, to place the results in history 
test_print17()
{
	let totalno=totalno+1
	echo "Test $totalno: print with -s option, to place the results in history"
	print -s Im history now
	ret=$?
	
	if [ $ret = 0 ]
	then
   		echo "PASS"
   		let passno=passno+1
 	else
  		echo "FAIL: Expected return value 0, but  returned $ret"
  		let failno=failno+1
	fi
}

#Test: Test print with -z option, to place the results in editing buffer stack
test_print18()
{
    let totalno=totalno+1
    echo "Test $totalno: print with -z option, to place the results in editing buffer stack"
    print -z editing buffer stack
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %d 
test_print19()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %d "
    printf '%d\n' 628
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %g
test_print20()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %g "
    printf '%g\n' 172.82
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}


#Test: Test printf with format specifier %<num>c
test_print21()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %<num>c "
    printf '%5c\n' x
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %.<num>s to chop the string
test_print22()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %.<num>s to chop the string"
    printf '%.5s\n' Trim_the_end
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}


#Test: Test printf with format specifier %*.*f
test_print23()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %*.*f"
    printf '%*.*f\n' 4 1 1283.526
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with invalid format specifier 
test_print24()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with invalid format specifier"
    printf '%y' fgsdf
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with bad math expression
test_print25()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with bad math expression"
    printf '%d\n' 9u
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with no arguments and %d as format specifier
test_print26()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with no arguments and %d as format specifier"
    printf '%d\n'
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with initial -- before format specifier printf ignores --
test_print27()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with initial -- before format specifier. printf ignores --"
    printf -- '%s\n' Hello
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print with -D option
test_print28()
{
    let totalno=totalno+1
    echo "Test $totalno: print with -D option"
    print -D "${HOME}"
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print -m option with bad pattern specified
test_print29()
{
    let totalno=totalno+1
    echo "Test $totalno: print -m option with bad pattern specified"
    print -m '[]' 8 9njk jkdd
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}


#Test: Test printf with %b format specifier
test_print30()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with %b format specifier"
    printf '%b' Hello'\n\t'World
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with %q format specifier
test_print31()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with %q format specifier"
    printf '%q' '\tHello\'
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with %x format specifier
test_print32()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with %x format specifier"
    printf '%x' 10
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with %X format specifier
test_print33()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with %X format specifier"
    printf '%X' 10
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with %E format specifier
test_print34()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with %E format specifier"
    printf '%.1E' 1021.2823
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with out of range %n$ format specifier
test_print35()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with out of range %n$ format specifier"
    printf '%20$d' 9 28
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 1, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print with -R -en option to emulate BSD echo command
test_print36()
{
    let totalno=totalno+1
    echo "Test $totalno: print with -R -en option to emulate BSD echo command"
    print -R -en Hello\\nWorld
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print with -R -En option to emulate BSD echo command
test_print37()
{
    let totalno=totalno+1
    echo "Test $totalno: print with -R -En option to emulate BSD echo command"
    print -R -E Hello\\nWorld
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print -aC with invalid number of columns
test_print38()
{
    let totalno=totalno+1
    echo "Test $totalno: print -aC with invalid number of columns"
    print -aC -9 jf 9 dk9 fk8 99dm
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 1, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print -aC with non numerical argument for columns
test_print39()
{
    let totalno=totalno+1
    echo "Test $totalno: print -aC with non numerical argument for columns"
    print -aC a b c d e
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 1, but returned $ret"
        let failno=failno+1
    fi
}


#Test: Test print with -p option, for 'bad file name' condition 
test_print40()
{
    let totalno=totalno+1
    echo "Test $totalno: print with -p option, for 'bad file name' condition"
    print -p coprocess
    ret=$?

    if [ $ret = 1 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 1, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print with -C option, not along with -a option
test_print41()
{
    let totalno=totalno+1
    echo "Test $totalno: print with -C option, not along with -a option"
    print -C 4 a b c d e f
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test print with -c option, not along with -a option
test_print42()
{
    let totalno=totalno+1
    echo "Test $totalno: print with -c option, not along with -a option"
    print -c a b c d e f
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %i
test_print43()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %i"
    printf '%i\n' 65
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %o
test_print44()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %o"
    printf '%o\n' 15
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %u
test_print45()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %u"
    printf '%u\n' 15128121
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %G
test_print46()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %G"
    printf '%G\n' 15128121
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
        let failno=failno+1
    fi
}

#Test: Test printf with format specifier %e
test_print47()
{
    let totalno=totalno+1
    echo "Test $totalno: printf with format specifier %e"
    printf '%e\n' 123.3456
    ret=$?

    if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret"
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
test_print2
test_print3
test_print4
test_print5
test_print6
test_print7
test_print8
test_print9
# test_print10
# test_print11
test_print12
test_print13
test_print14
test_print15
test_print16
test_print17
#test_print18
test_print19
test_print20
test_print21
test_print22
test_print23
test_print24
test_print25
test_print26
test_print27
test_print28
test_print29
test_print30
test_print31
test_print32
test_print33
test_print34
test_print35
test_print36 # Defect No: 55
test_print37 # Defect No: 56
test_print38
test_print39
test_print40
test_print41
test_print42
test_print43
test_print44
test_print45
test_print46
test_print47
report
