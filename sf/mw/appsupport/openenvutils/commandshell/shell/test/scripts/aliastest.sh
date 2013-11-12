// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

init()
    {
    let totalno=0;
    let passno=0;
    let failno=0;
    }

test_alias2()
{
let totalno=totalno+1
echo "Test $totalno: alias with no argument to display set aliases"
alias var='somecmd'
res=`alias`
ret=$?
if [ $ret = 0 ]
then
	if [ -z $res ]
	then
		echo "FAIL: Expected alias list of nonzero length, but returned zero-length"
        let failno=failno+1
	else
		echo "PASS"
        let passno=passno+1		
	fi
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}

test_alias3()
{
let totalno=totalno+1
echo "Test $totalno: alias with more than alias settings"
alias a=z b=y
ret=$?
if [ $ret = 0 ]
then
  	echo "PASS"
  	let passno=passno+1
  	unalias a b
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}

test_alias4()
{
let totalno=totalno+1
echo "Test $totalno: alias with nonexistant alias"
alias notanalias
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

test_alias5()
{
let totalno=totalno+1
echo "Test $totalno: alias with -g option to test a global alias"
alias -g M='P.I.P.S enables Symbian'
ret=$?
if [ $ret = 0 ]
then
	print M > a.txt
	ret=$?
	rm a.txt
	if [ $ret = 0 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected return value 0, but  returned $ret ls failed"
        let failno=failno+1
    fi
else
	echo "FAIL: Expected return value 0, but  returned $ret alias failed"
 	let failno=failno+1
fi
}

test_alias6()
{
let totalno=totalno+1
echo "Test $totalno: alias with -m and a pattern as argument to get all alias listed with that pattern inside"
alias ab=xy
res=`alias -m "ab"`
ret=$?
if [ $ret = 0 ]
then
  	if [ $res = "ab=xy" ]
  	then
	    echo "PASS"
        let passno=passno+1
  	else
        echo "FAIL: Expected ab=xy, but  returned $res"
        let failno=failno+1
  	fi
  	unalias ab
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}



test_alias7()
{
let totalno=totalno+1
echo "Test $totalno: alias with -s option and a file suffix as an alias and a command as its value, so that something.suffix will be replaced as command something.suffix"
alias -s txt=echo
ret=$?
res=`result.txt`
if [ $ret = 0 ]
then
  	if [ $res = "result.txt" ]
  	then
        echo "PASS"
        let passno=passno+1
  	else
        echo "FAIL: Expected result.txt, but  returned $res"
        let failno=failno+1
  	fi
  	unalias -s txt 
else
	echo "FAIL: Expected return value 0, but  returned $ret"
	let failno=failno+1
fi
}

test_alias8()
{
let totalno=totalno+1
echo "Test $totalno: alias with -L option to list all aliases except suffix aliases"
alias -L
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

test_alias9()
{
let totalno=totalno+1
echo "Test $totalno: alias with -L option to display a nonalias"
alias -L noaliasbythisname
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

test_alias10()
{
let totalno=totalno+1
echo "Test $totalno: alias with -g option to print global aliases"
alias -g globalalias=pwd
aliaslist=(`alias -g`)
ret=$?
if [ $ret = 0 ]
then
	len=${#aliaslist[*]}
    let i=0
    while [ $i -le $len  ]
    do
        if [ $aliaslist[$i] = "globalalias=pwd" ]
        then
            echo "PASS"
            let passno=passno+1
            return
        fi
        let i=i+1
    done
    echo "FAIL: Expected globalalias=pwd, but not found"
    let failno=failno+1
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}

test_alias11()
{
let totalno=totalno+1
echo "Test $totalno: alias with +g option to print global aliases name only - no value"
alias -g globalalias=pwd
aliaslist=(`alias +g`)
ret=$?
if [ $ret = 0 ]
then
	len=${#aliaslist[*]}
    let i=0
    while [ $i -le $len  ]
    do
        if [ $aliaslist[$i] = "globalalias" ]
        then
            echo "PASS"
            let passno=passno+1
            return
        fi
        let i=i+1
    done
    echo "FAIL: globalalias not found"
    let failno=failno+1	
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}


test_alias12()
{
let totalno=totalno+1
echo "Test $totalno: alias with -r option to create a regular alias"
alias -r regularalias=pwd
ret=$?
cwd=`regularalias`
if [ $ret = 0 ]
then
    if [ $PWD = $cwd ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $PWD, but  returned $cwd"
        let failno=failno+1
    fi
	unalias regularalias 
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
	let failno=failno+1
fi
}

test_alias13()
{
let totalno=totalno+1
echo "Test $totalno: alias with -r option to list regular aliases"
alias -r regularalias=pwd
aliaslist=(`alias -r`)
ret=$?
if [ $ret = 0 ]
then
	len=${#aliaslist[*]} 
	let i=0
    while [ $i -le $len  ]
	do
		if [ $aliaslist[$i] = "regularalias=pwd" ]
    	then
        	echo "PASS"
        	let passno=passno+1
			unalias regularalias
			return
    	fi
		let i=i+1
	done
    echo "FAIL: Expected regularalias=pwd, but not found"
    let failno=failno+1	
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}

test_alias14()
{
let totalno=totalno+1
echo "Test $totalno: alias with +r option to list regular aliases(names only)"
alias -r regularalias=pwd
aliaslist=(`alias +r`)
ret=$?
if [ $ret = 0 ]
then
    len=${#aliaslist[*]}
    let i=0
    while [ $i -le $len  ]
    do
        if [ $aliaslist[$i] = "regularalias" ]
        then
            echo "PASS"
            let passno=passno+1
            unalias regularalias
            return
        fi
        let i=i+1
    done
    echo "FAIL: Expected regularalias, but not found"
    let failno=failno+1
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}

test_alias15()
{
let totalno=totalno+1
echo "Test $totalno: alias with +m and a pattern as argument to get the alias name listed with that pattern inside"
alias ab=xy
res=`alias +m "ab"`
ret=$?
if [ $ret = 0 ]
then
  	if [ $res = "ab" ]
  	then
        echo "PASS"
        let passno=passno+1
  	else
        echo "FAIL: Expected ab, but  returned $res"
        let failno=failno+1
  	fi
  	unalias ab
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}


test_alias16()
{
let totalno=totalno+1
echo "Test $totalno: alias with -s option to display the suffix aliases"
alias -s txt=echo
ret=$?
aliaslist=(`alias -s`)
if [ $ret = 0 ]
then
	len=${#aliaslist[*]}
    let i=0
    while [ $i -le $len  ]
    do
        if [ $aliaslist[$i] = "txt=echo" ]
        then
            echo "PASS"
            let passno=passno+1
            unalias -s txt 
            return
        fi
        let i=i+1
    done
    echo "FAIL: Expected txt=echo, but not found"
    let failno=failno+1
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}

test_alias17()
{
let totalno=totalno+1
echo "Test $totalno: alias with +s option to display the suffix alias names"
alias -s txt=echo
ret=$?
aliaslist=(`alias +s`)
if [ $ret = 0 ]
then
    len=${#aliaslist[*]}
    let i=0
    while [ $i -le $len  ]
    do
        if [ $aliaslist[$i] = "txt" ]
        then
            echo "PASS"
            let passno=passno+1
            unalias -s txt
            return
        fi
        let i=i+1
    done
    echo "FAIL: Expected txt, but not found"
    let failno=failno+1
else
 	echo "FAIL: Expected return value 0, but  returned $ret"
 	let failno=failno+1
fi
}


test_alias18()
{
let totalno=totalno+1
echo "Test $totalno: alias with +L option to list all aliase names except suffix aliases"
alias +L
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

# define an alias to another alias with that value.
test_alias19()
{
let totalno=totalno+1
echo "Test $totalno: alias with a variable name and a command as argument"
alias workdir1="pwd"
alias workdir2="workdir1"
alias workdir3="workdir2"
ret=$?
if [ $ret = 0 ]
then
     if [ `pwd` = `workdir3` ]
     then
          echo "PASS"
          let passno=passno+1
     else
          echo "FAIL: Expected \"`pwd`\", returned \"`workdir`\""
          let failno=failno+1
     fi
else
     echo "FAIL: Expected return value 0, but  returned $ret"
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
test_alias2
test_alias3
test_alias4
test_alias5
test_alias6
test_alias7
test_alias8
test_alias9
test_alias10
test_alias11
test_alias12
test_alias13
test_alias14
test_alias15
test_alias16
test_alias17
test_alias18
test_alias19
report
