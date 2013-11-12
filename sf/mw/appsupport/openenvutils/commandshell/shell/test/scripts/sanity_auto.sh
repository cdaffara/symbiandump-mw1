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

#!/home/guest/vinodp/local/bin/zsh
###################################
# Test cases for internal commands#
###################################

#Inititalise counters for Total tests, passes, fails.
preamble()
{
if [ ! -d chdirtest ] 
then
	mkdir chdirtest
fi
if [ ! -d cdtest ]
then
	mkdir cdtest
fi
if [ ! -d pushdtest ]
then
    mkdir pushdtest
fi
if [ ! -d popdtest ]
then
    mkdir popdtest
fi
if [ ! -d dirstest ]
then
    mkdir dirstest
fi
}

init()
{
let totalno=0;
let passno=0;
let failno=0;
}

#Start test cases
#pwd command print the absolute pathname of the current working directory
test_pwd1()
{
let totalno=totalno+1
echo "Test $totalno: pwd with no argument"
cwd=`pwd`
ret=$?
if [ $ret = 0 ]
	then
	if [ $PWD = $cwd ]
	then
		echo "PASS"
		let passno=passno+1
	else
		echo "FAIL: Expected $PWD, returned $cwd"
		let failno=failno+1
	fi
else
	echo "FAIL: Expected return value 0, but  returned $ret"
	let failno=failno+1
fi
}

# echo command Write each arg on the standard output, with a space separating each one
test_echo1()
{
let totalno=totalno+1
echo "Test $totalno: echo with a string as argument"
echostr=`echo "This is a test string"`
ret=$?
if [ $ret = 0 ]
then
	if [ $echostr = "This is a test string" ]
	then
		echo "PASS"
		let passno=passno+1 
	else
		echo "FAIL: Expected \"This is a test string\", returned \"$echostr\""
		let failno=failno+1
	fi
else
	echo "FAIL: Expected return value 0, but  returned $ret"
	let failno=failno+1
fi
}

# With no flags the arguments are printed on the standard output as described by echo
test_print1()
{
let totalno=totalno+1
echo "Test $totalno: print with a string and a variable as argument"
let var=1+2
printstr=`print "This is a value of a variable: $var"`
ret=$?
if [ $ret = 0 ]
then
	 if [ $printstr = "This is a value of a variable: 3" ]
	 then
		  echo "PASS"
		  let passno=passno+1
	 else
		  echo "FAIL: Expected \"This is a value of a variable: 3\", returned \"$printstr\""
		  let failno=failno+1
	 fi
else
	 echo "FAIL: Expected return value 0, but  returned $ret"
	 let failno=failno+1
fi
}


# Print the arguments according to the format specification. Formatting rules are the same as used in C
test_printf1()
{
let totalno=totalno+1
echo "Test $totalno: printf with a string and a variable as argument"
let var=1+2
printfstr=`printf "This is a value of a variable: $var"`
ret=$?
if [ $ret = 0 ]
then
	 if [ $printfstr = "This is a value of a variable: 3" ]
	 then
		  echo "PASS"
		  let passno=passno+1
	 else
		  echo "FAIL: Expected \"This is a value of a variable: 3\", returned \"$printfstr\""
		  let failno=failno+1
	 fi
else
	 echo "FAIL: Expected return value 0, but  returned $ret"
	 let failno=failno+1
fi
}

# Read one line and break it into fields using the characters 
test_read1()
{
let totalno=totalno+1
echo "Test $totalno: read a value into the argument"
echo Hello > readtest.txt
read readvar < readtest.txt
ret=$?
if [ $ret = 0 ]
then
	 if [ $readvar = "Hello" ]
	 then
		  echo "PASS"
		  let passno=passno+1
	 else
		  echo "FAIL: Expected \"Hello\", read \"$readvar\""
		  let failno=failno+1
	 fi
else
	 echo "FAIL: Expected return value 0, but  returned $ret"
	 let failno=failno+1
fi
}


# change the current directory to arg
test_chdir1()
{
let totalno=totalno+1
echo "Test $totalno: chdir with one argument"
if [ -d chdirtest ] 
then
	 oldcwd=`pwd`
	 chdir chdirtest
	 ret=$?
	 if [ $ret = 0 ]
	 then
		  if [ $PWD = $oldcwd\chdirtest ]
		  then
			   echo "PASS"
			   let passno=passno+1
		  else
			   echo "FAIL: Expected \"$PWD\", returned \"$oldcwd\chdirtest\""
			   let failno=failno+1
			   cd ..
			   return
		  fi
	 else
		  echo "FAIL: Expected return value 0, but  returned $ret"
		  let failno=failno+1
		  cd..
	      return
	 fi
	 chdir ..
else
	 echo "FAIL:chdirtest dir does not exist"
	 let failno=failno+1
fi
}

# change the current directory to arg
test_cd1()
{
let totalno=totalno+1
echo "Test $totalno: cd with one argument"
if [ -d cdtest ]
then
	 oldcwd=`pwd`
	 cd cdtest
	 ret=$?
	 if [ $ret = 0 ]
	 then
		  if [ $PWD = $oldcwd\cdtest ]
		  then
			   echo "PASS"
			   let passno=passno+1
		  else
			   echo "FAIL: Expected \"$PWD\", returned \"$oldcwd\\cdtest\""
			   let failno=failno+1
		  fi
	 else
		  echo "FAIL: Expected return value 0, but  returned $ret"
		  let failno=failno+1
	 fi
	 cd ..
else
	 echo "FAIL:cdtest dir missing"
	 let failno=failno+1
fi
}


# Change the current directory, and push the old current directory onto the directory stack
test_pushd1()
{
let totalno=totalno+1
echo "Test $totalno: pushd with directory name as argument"
if [ -d pushdtest ]
then
	 oldcwd=`pwd`
	 pushd pushdtest
	 ret=$?
	 if [ $ret = 0 ]
	 then
		  if [ $PWD = ${oldcwd}\pushdtest ]
		  then
			   echo "PASS"
			   let passno=passno+1
		  else
			   echo "FAIL: Expected \"$PWD\", returned \"${oldcwd}\pushdtest\""
			   let failno=failno+1
		  fi
	 else
		  echo "FAIL: Expected return value 0, but  returned $ret"
		  let failno=failno+1
 	fi
	popd
else
	 echo  "FAIL: Dir pushdtest does not exist."
	 let failno=failno+1
fi
}



# Remove an entry from the directory stack, and perform a cd to the new top directory
test_popd1()
{
let totalno=totalno+1
echo "Test $totalno: popd with no argument"
if [ -d popdtest ]
then
 	oldcwd=`pwd`
	pushd popdtest
 	popd
	if [ $ret = 0 ]
	then 
		  if [ $PWD = $oldcwd ]
		  then
			   echo "PASS"
			   let passno=passno+1  
		  else
			   echo "FAIL: popd failed: Expected \"$oldcwd\", returned  \"$PWD\""
               let failno=failno+1
  		  fi
	else
  		echo "FAIL: Popd failed: Expected return value 0, but  returned $ret"
	  	let failno=failno+1
	fi
else
 	echo "FAIL: dir popdtest doenot exist."
 	let failno=failno+1
fi
}


# With no arguments, print the contents of the directory stack
test_dirs1()
{
let totalno=totalno+1
echo "Test $totalno: dirs with -l option set"
if [ -d dirstest ]
then
	oldcwd=`pwd`
	pushd dirstest
	ret=$?
	if [ $ret = 0 ]
	then
		dirsarray=(`dirs -l`)
		ret=$?
        if [ $ret != 0 ]
		then
			echo "FAIL: dirs gave nonzero return value: $ret"
			let failno=failno+1
			popd
			return		
		else
			len=${#dirsarray[*]} #Number of elements in stack
			if [ $len != 2 ]
			then 
				echo "FAIL: dirs failed: Expected 2, but is ${len}"
				let failno=failno+1
				popd
				return
			else
				if [ ${dirsarray[1]} != ${oldcwd}\dirstest ] || [ ${dirsarray[2]} != ${oldcwd} ]   
				then
					echo "FAIL: dirs failed: Expected \"${oldcwd}\dirstest\" as the first string, but is \"${dirsarray[1]}\" or expected \"${oldcwd}\" as the second string, but is \"${dirsarray[2]}\""
					let failno=failno+1
					popd
					return
  				fi
			fi
		fi
	else
		echo "FAIL: Pushd failed: Expected return value 0, but  returned $ret"
		let failno=failno+1
	fi
 	popd
	dirsstr=`dirs -l`
	dirsarray=($dirsstr)
	if [ $ret = 0 ]
	then
		if [ ${dirsarray[1]} = $oldcwd ]
		then
 			echo "PASS"
			let passno=passno+1
		else
			echo "FAIL: dirs failed: Expected \"$oldcwd\", returned  \"${dirsarray[1]}\""
			let failno=failno+1
		fi
	else
		echo "FAIL: Popd failed: Expected return value 0, but  returned $ret"
		let failno=failno+1
	fi
	dirstr=`dirs`
else
	echo "FAIL: Dir dirstest does not exist"
	let failno=failno+1
fi
}


# For each name with a corresponding value, define an alias with that value.
test_alias1()
{
let totalno=totalno+1
echo "Test $totalno: alias with a variable name and a command as argument"
alias workdir="pwd"
ret=$?
if [ $ret = 0 ]
then
	 if [ `pwd` = `workdir` ]
	 then
		  echo "PASS"
		  let passno=passno+1
	 else
		  echo "FAIL: Expected \"`pwd`\", returned \"`workdir`\""
		  let failno=failno+1
	 fi
	 unalias workdir
else
	 echo "FAIL: Expected return value 0, but  returned $ret"
	 let failno=failno+1
fi
}


# causes unhash to remove regular or global aliases
test_unalias1()
{
let totalno=totalno+1
echo "Test $totalno: unalias with an existing alias name as argument"
alias workdir="pwd"
unalias workdir
ret=$?
if [ $ret = 0 ]
then
	workdir
    ret=$?
	if [ $ret != 127 ]
	then
		echo "FAIL: Expected cmd not found(127), returned $ret"
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


# set positional parameters
# @internal test function
util_set1()
{
arg1=$1;arg2=$2;arg3=$3
old=(`echo $@`)
set a b c # Values directly to positional parameters
if [ $1 = a ] && [ $2 = b ] && [ $3 = c ]
then
	 ret=0
else
	 ret=1
	 return $ret
fi
echo "First set returned $ret"
set $old # Value from array to possitional parameters
if [ $1 = $arg1 ] && [ $2 = $arg2 ] && [ $3 = $arg3 ]
then
	 ret=0
else
	 ret=1
fi
echo "second set returned $ret"
return $ret
}

# Set the options for the shell and/or set the positional parameters, or declare and set an array
test_set1()
{
let totalno=totalno+1
echo "Test $totalno: set command to set positional parameters"
util_set1 x y z
ret=$?
if [ $ret = 0 ]
then
	echo "PASS"
	let passno=passno+1
else
	echo "FAIL: Expected 0, returned $ret"
	let failno=failno+1
fi
}


# Each named parameter is unset
test_unset1()
{
let totalno=totalno+1
echo "Test $totalno: unset command to unset array name"
unsetarray=(1 2 3)
unset unsetarray
ret=$?
if [ $ret = 0 ]
then
	if [ ${unsetarray[1]} ] || [ ${unsetarray[2]} ] || [ ${unsetarray[3]} ]
	then
	   	echo "FAIL: Expected empty strings as array elements, but were $unsetarray[1], $unsetarray[2], $unsetarray[3]"
       	let failno=failno+1
	else
		echo "PASS"
		let passno=passno+1
	fi
else
	echo "FAIL: Expected return value 0, returned $ret"
	let failno=failno+1
fi
}

# For each name, indicate how it would be interpreted if used as a command name.
test_which1()
{
let totalno=totalno+1
echo "Test $totalno: which with internal command name as argument"
which pwd
ret=$?
if [ $ret = 0 ]
then
	echo "PASS"
	let passno=passno+1
else
	echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}


# Read the arguments as input to the shell and execute the resulting command in the current shell process
test_eval1()
{
let totalno=totalno+1
echo "Test $totalno: eval to run a command"
evalvar=pwd
outputvar=`eval $evalvar`
ret=$?
if [ $ret = 0 ]
then
	if [ $PWD = $outputvar ]
    then
		echo "PASS"
    	let passno=passno+1
	else
		echo "FAIL: Expected $PWD, returned $outputvar"
    	let failno=failno+1
	fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}


# Redirect the output of the first command to the file
test_redirectionop1()
{
let totalno=totalno+1
echo "Test $totalno: > operator to redirect the output of a command to a  file"
pwd > redirectedfile
ret=$?
if [ $ret = 0 ]
then
	#Read in raw mode (-r) to allow reading '\'
	read -r redirectedvar < redirectedfile
    if [ $PWD = $redirectedvar ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"$PWD\", returned \"$redirectedvar\""
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}



# Convert the result to lower case whenever the parameter is expanded.
test_typeset1()
{
let totalno=totalno+1
echo "Test $totalno: typeset to set a variable to have always value in lower case"
typeset -l var=VAL
ret=$?
if [ $ret = 0 ]
then
    if [ $var = val ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"val\", returned \"$var\""
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}



# For each name, indicate how it would be interpreted if used as a command name. 
test_whence1()
{
let totalno=totalno+1
echo "Test $totalno: whence to display the path of a bad command"
res=`whence tweak`
ret=$?
if [ $ret = 1 ] && [ $res =  ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 1 and returned $ret or expected empty string and returned $res"
    let failno=failno+1
fi
}


# not supported, to be removed
test_hash1()
{
let totalno=totalno+1
echo "Test $totalno: hash to store the path of a command in hash table"
hash pwd=pwd
ret=$?
if [ $ret != 0 ]
then
  	echo "FAIL: Expected 0, but returned $ret"
	let failno=failno+1
	return
fi
res=`hash -v pwd`
ret=$?
if [ $ret = 0 ] && [ $res = "pwd=pwd" ]
then
    echo "PASS"
    let passno=passno+1
else
    echo "FAIL: Expected 0 and returned $ret or expected \"pwd=pwd\" string and returned $res"
    let failno=failno+1
fi
}


util_autoloaded_function()
{
echo hello
}

# autoload a function into cache so that can be loaded next time fast
test_autoload1()
{
let totalno=totalno+1
echo "Test $totalno: autoload to load a function"
autoload util_autoloaded_function 
ret=$?
if [ $ret = 0 ]
then
	res=`util_autoloaded_function`
	if [ $res = hello ]
	then
   		echo "PASS"
	   	let passno=passno+1
   		unfunction util_autoloaded_function
	else
		echo "FAIL: Expected hello, returned $res"
    	let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}




# Execute the bult-in command given as argument
test_builtin1()
{
let totalno=totalno+1
echo "Test $totalno: builtin with pwd as argument"
cwd=`builtin pwd`
ret=$?
unhash -f pwd
if [ $ret = 0 ]
then
	 if [ $PWD = $cwd ]
	 then
		   echo "PASS"
		   let passno=passno+1
	 else
		   echo "FAIL: Expected $PWD, returned $cwd"
		   let failno=failno+1
	 fi
else
	 echo "FAIL: Expected return value 0, but  returned $ret"
	 let failno=failno+1
fi
}


# Execute the simple command preference being to external command
test_command1()
{
let totalno=totalno+1
echo "Test $totalno: command with pwd as argument"
cwd=`command pwd`
ret=$?
if [ $ret = 0 ]
then
	 if [ $cwd = hello ] # hello is printed to the console by the external command.
	 then
		   echo "PASS"
		   let passno=passno+1
	 else
		   echo "FAIL: Expected $PWD, returned $cwd"
		   let failno=failno+1
	 fi
else
	 echo "FAIL: Expected return value 0, but  returned $ret"
	 let failno=failno+1
fi
}

# To be removed
test_exec1()
{
let totalno=totalno+1
echo "Test $totalno: exec with no argument"
exec
ret=$?
if [ $ret = 127 ]
then
 	  echo "PASS"
	  let passno=passno+1
else
	  echo "FAIL: Expected 127, returned $ret"
	  let failno=failno+1
fi
}

# Prevents expanding of alias
test_noglob1()
{
let totalno=totalno+1
echo "Test $totalno: noglob to prevent expanding alias"
alias foo='echo an alias for foo'
noglob foo
ret=$?
if [ $ret = 127 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 127, returned $ret"
    let failno=failno+1
fi
}


# Open square bracket on command prompt should return 1
test_open_sq_bkt()
{
let totalno=totalno+1
echo "Test $totalno: ["
[
ret=$?
if [ $ret = 1 ]
then
   	echo "PASS"
   	let passno=passno+1
else
    echo "FAIL: Expected 1, returned $ret"
    let failno=failno+1
fi
}

# A dot(.) on command prompt should return 1
test_dot()
{
let totalno=totalno+1
echo "Test $totalno: ."
.
ret=$?
if [ $ret = 1 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 1, returned $ret"
    let failno=failno+1
fi
}

# A colon on a command prompt should return 0
test_colon()
{
let totalno=totalno+1
echo "Test $totalno: :"
:
ret=$?
if [ $ret = 0 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

# No job control support. To be removed
test_send_job_to_bg()
{
let totalno=totalno+1
echo "Test $totalno: pwd with argument &"
pwd &
ret=$?
if [ $ret = 0 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

# No job control. To be removed
test_bg1()
{
let totalno=totalno+1
echo "Test $totalno: bg without any argument"
bg
ret=$?
if [ $ret = 127 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

# Test break command inside awhile loop
test_break1()
{
let totalno=totalno+1
echo "Test $totalno: break"
let var=1
while [ 1 ]
do
	break
	let var=2
done
ret=$?
if [ $ret = 0 ]&&[ $var = 1 ]
then
   	echo "PASS"
   	let passno=passno+1
else
	echo "FAIL: Expected return value 0, returned $ret or expected var value 1, returned $var"
   	let failno=failno+1
fi
}

test_continue1()
{
let totalno=totalno+1
echo "Test $totalno: continue within while loop" 
let var=1
while [ $var = 1 ]
do
	if [ $var = 1 ]
	then
		let var=2			
		continue
	fi
    let var=3
done
if [ $var = 2 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected value 2, returned $var"
   	let failno=failno+1
fi
}

test_declare1()
{
let totalno=totalno+1
echo "Test $totalno: declare with -i option"
declare -i var=1.23
ret=$?
if [ $ret = 0 ]
then
   	if [ $var = 1 ]
   	then
		echo "PASS"
	   	let passno=passno+1
	else
		echo "FAIL: Expected 1, returned $var"
	   	let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_disable1()
{
let totalno=totalno+1
echo "Test $totalno: disable without argument"
disable
ret=$?
if [ $ret = 0 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_disown1()
{
let totalno=totalno+1
echo "Test $totalno: disown without argument"
disown
ret=$?
if [ $ret = 1 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 1, returned $ret"
   	let failno=failno+1
fi
}

test_emulate1()
{
let totalno=totalno+1
echo "Test $totalno: emulate with -L sh argument to set options default to sh emulation mode"
emulate -L sh
ret=$?
if [ $ret = 0 ]
then
	res=(`setopt`)
   	if [ $res = banghist ]
	then
		echo "PASS"
   		let passno=passno+1
	else
		echo "FAIL: Expected banghist, returned $res"
		let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_export1()
{
let totalno=totalno+1
echo "Test $totalno: export with a variable=value as argument"
export val=1
ret=$?
if [ $ret = 0 ]
then
   	if [ $val = 1 ]
   	then
   	 	echo "PASS"
   	 	let passno=passno+1
   	else
 		echo "FAIL: Expected 1, returned $val"
    	let failno=failno+1
   	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_false1()
{
let totalno=totalno+1
echo "Test $totalno: false with no argument"
false
ret=$?
if [ $ret = 1 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 1, returned $ret"
   	let failno=failno+1
fi
}

test_fc1()
{
let totalno=totalno+1
echo "Test $totalno: fc with -ln -1 argument"
fc -ln -1
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_fg1()
{
let totalno=totalno+1
echo "Test $totalno: fg with no argument"
fg
ret=$?
if [ $ret = 1 ]
then
    echo "PASS"
    let passno=passno+1
else
   	echo "FAIL: Expected 1, returned $ret"
   	let failno=failno+1
fi
}

test_float1()
{
let totalno=totalno+1
echo "Test $totalno: float with var=integer value argument"
float var=1
ret=$?
if [ $ret = 0 ]
then	
	if [ $var = 1.000000000e+00 ]
	then
		echo "PASS"
	    let passno=passno+1
	else
		echo "FAIL: Expected 1.000000000e+00, returned $var"
   		let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}



util_functions()
{
echo hello
}

test_functions1()
{
let totalno=totalno+1
echo "Test $totalno: functions with one function name as argument"
res=(`functions util_functions`)
ret=$?
if [ $ret = 0 ]
then
	if [ $res[1] = util_functions ] && [ $res[2] = "()" ] 
    then
		echo "PASS"
    	let passno=passno+1
	else
		echo "FAIL: Expected util_functions definition, but not returned."
		let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

# getln read the top value from the buffer stack and put it in the shell parameter name
test_getln1()
{
let totalno=totalno+1
echo "Test $totalno: getln with 2 variable arguments"
pushln first second
getln a b
ret=$?
if [ $ret = 0 ]
then
	if [ $a = first ] && [ $b = second ]
	then
    	echo "PASS"
    	let passno=passno+1
	else
		echo "FAIL: Expected first and second, returned $a and $b"
   		let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

# @internal test function
util_getopts()
{
aflag=
bflag=
while getopts ab: name
do
    case $name in
    a)    aflag=1;;
    b)    bflag=1
          bval="$OPTARG";;
    *)   printf "Usage: %s: [-a] [-b value] args\n" $0
          return 2;;
    esac
done
if [ ! -z "$aflag" ] && [ ! -z "$bflag" ] && [ $bval = 1 ]
then
    shift $(($OPTIND - 1))
    if [ $1 = 2 ] && [ $2 = 3 ]
	then
		return 0
	else
		return 1
	fi
else
    return 1
fi
}

# getopts command will parse the arguments and can be used to enforce options
test_getopts1()
{
let totalno=totalno+1
echo "Test $totalno: getopts with arguments to parse options and values instead of positional parameters"
util_getopts -a -b 1 2 3
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1	
fi
}

test_integer1()
{
let totalno=totalno+1
echo "Test $totalno: integer with var=float value argument"
integer var=1.23
ret=$?
if [ $ret = 0 ]
then	
    if [ $var = 1 ]
    then	
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected 1, returned $var"
        let failno=failno+1
    fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_jobs1()
{
let totalno=totalno+1
echo "Test $totalno: jobs to display current jobs"
jobs
ret=$?
if [ $ret = 127 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 127, returned $ret"
   	let failno=failno+1
fi
}

# Negative test to check whether it returns 127 or not.
test_kill1()
{
let totalno=totalno+1
echo "Test $totalno: kill -l to display all the signals"
kill 100
ret=$?
if [ $ret = 127 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 127, returned $ret"
   	let failno=failno+1
fi
}

# @internal test function
util_local1()
{
yy=1
local xx=2
}

# local variables are accessible only in the block of its definition.
test_local1()
{
let totalno=totalno+1
echo "Test $totalno: local declare variables local to function"
util_local1
if [ -z "$xx" ] && [ ! -z "$yy"  ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 1 and empty string, returned $yy and $xx"
   	let failno=failno+1
fi
}


test_rehash1()
{
let totalno=totalno+1
echo "Test $totalno: rehash function empty the current hash table"
rehash
res=$?
if [ $res = 127 ]
then
    echo "PASS"
    let passno=passno+1
else
   	echo "FAIL: Expected 127, returned $res"
   	let failno=failno+1
fi
}

# @internal test function
util_shift()
{
arg1=$2
shift
if [ $1 = $arg1 ]
then
    return 0
else
    return 1
fi
}

# shift command shifts the positional parameters one position left
test_shift1()
{
let totalno=totalno+1
echo "Test $totalno: shift to shift the positional argument left once"
util_shift 10 20
res=$?
if [ $res = 0 ]
then
    echo "PASS"
    let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $res"
   	let failno=failno+1
fi
}

# source command is used to execute existing script file
test_source1()
{
let totalno=totalno+1
echo "Test $totalno: source to execute existing script file"
if [ -f util_source.sh ]
then
	source util_source.sh
	ret=$?
	if [ $ret = 100 ]
	then
		echo "PASS"
    	let passno=passno+1
	else
		echo "FAIL: Expected 100, returned $res"
		let failno=failno+1
	fi
else
   	echo "FAIL: util_source.sh does not exist in the current directory"
   	let failno=failno+1
fi
}

test_true1()
{
let totalno=totalno+1
echo "Test $totalno: true with no argument"
true
ret=$?
if [ $ret = 0 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_ttyctl1()
{
let totalno=totalno+1
echo "Test $totalno: ttyctl with no argument to know the frozen status of tty"
ttyctl
ret=$?
if [ $ret = 127 ]
then	
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 127, returned $ret"
   	let failno=failno+1
fi
}



test_trap1()
{
let totalno=totalno+1
echo "Test $totalno: trap to print the line number of command last executed"
trap 'print $LINENO' DEBUG
ret=$?
if [ $ret = 0 ]
then
   	echo "PASS"
   	let passno=passno+1
	trap -
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_type1()
{
let totalno=totalno+1
echo "Test $totalno: type to display the type of the identifier"
alias test_al=1
res=`type test_al`
ret=$?
if [ $ret = 0 ]
then
	unalias test_al
	if [ $res = "test_al is an alias for 1" ]
	then
   		echo "PASS"
   		let passno=passno+1	
	else
		echo "FAIL: Expected \"test_al is an alias for 1\", returned $res"
   		let failno=failno+1		
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_umask1()
{
let totalno=totalno+1
echo "Test $totalno: umask to get the current value of file creation mask"
res=`umask`
ret=$?
if [ $ret = 127 ]
then
    echo "PASS"
    let passno=passno+1	
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

# @internal test function
util_unhash()
{
return
}

test_unhash1()
{
let totalno=totalno+1
echo "Test $totalno: unhash to remove function from hash table"
util_unhash
ret=$?
if [ $ret = 0 ]
then
	unhash -f util_unhash
	ret=$?
	if [ $ret = 0 ]
	then
		util_unhash
		ret=$?
		if [ $ret = 127 ]
		then
			echo "PASS"
   			let passno=passno+1
		else
			echo "FAIL: Expected 127, returned $ret"	
			let failno=failno+1	
		fi
	else
		echo "FAIL: Expected 0, returned $ret"
		let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_where1()
{
let totalno=totalno+1
echo "Test $totalno: where to display the path of a command"
res=`where unset`
ret=$?
if [ $ret = 0 ]
then
	if [ $res = "unset: shell built-in command" ]
	then
   		echo "PASS"
	    let passno=passno+1
	else
		echo "FAIL: Expected \"unset: shell built-in command\", returned $res"
   		let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

test_zmodload1()
{
let totalno=totalno+1
echo "Test $totalno: zmodload to display the path of loaded modules"
res=(`zmodload`)
ret=$?
if [ $ret = 127 ]
then	
    echo "PASS"
    let passno=passno+1
else
   	echo "FAIL: Expected 127, returned $ret"
   	let failno=failno+1
fi
}

test_cdablevars1()
{
let totalno=totalno+1
echo "Test $totalno: setopt cdablevars to enable cd to variable values=directory name"
setopt cdablevars
ret=$?
if [ $ret = 0 ]
then
	foo=c:\cdtest
	cd foo
	res=`pwd`
    if [ $res = "c:\cdtest" ]
    then
        echo "PASS"
        let passno=passno+1
		cd ..
    else
        echo "FAIL: Expected \"c:\cdtest\", returned $res"
        let failno=failno+1
    fi
	unsetopt cdablevars
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}


# Pipe to redirect the output of one command to input of another
test_pipe1()
{
let totalno=totalno+1
echo "Test $totalno: pipe test to give the output of one command to another"
echo "Hello" | read var
ret=$?
if [ $ret = 0 ]
then
	if [ $var = "Hello" ]
	then
   		echo "PASS"
		let passno=passno+1
	else
		echo "FAIL: Expected Hello, returned $var"
		let failno=failno+1
	fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}



test_precommand_modifier_hiphen1()
{
let totalno=totalno+1
echo "Test $totalno: Precommand modifier hiphen"
var1=`pwd`
var2=`- pwd`
ret=$?
if [ $ret = 0 ]
then	
    if [ $var1 =  $var2 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected $var1, returned $var2"
        let failno=failno+1
    fi
else
   	echo "FAIL: Expected 0, returned $ret"
   	let failno=failno+1
fi
}

# for looping construct to iterate for specified values in the list
test_for1()
{
let totalno=totalno+1
echo "Test $totalno: Test for loop"
let total=0
for i in 1 2 3
do
	let total=total+1
done
if [ $total = 3 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 3, returned $total"
   	let failno=failno+1
fi
}


test_until1()
{
let totalno=totalno+1
echo "Test $totalno: Test until loop"
let total=3
until total=1
do
    let total=total-1
done
if [ $total = 1 ]
then
   	echo "PASS"
   	let passno=passno+1
else
  	echo "FAIL: Expected 1, returned $total"
   	let failno=failno+1
fi
}


test_repeat1()
{
let totalno=totalno+1
echo "Test $totalno: Test repeat loop"
let total=0
repeat 10 let total=total+1
if [ $total = 10 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 1, returned $total"
   	let failno=failno+1
fi
}

test_case1()
{
let totalno=totalno+1
echo "Test $totalno: Test case construct"
char=a
case $char in
	'a')
		echo "PASS"
    	let passno=passno+1	
		;;
	*)
		echo "FAIL: Expected a, returned $char"
    	let failno=failno+1	
		;;
	esac
}


# @internal test function
util_longbadcmd1()
{
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
return $?
}

test_longbadcmd1()
{
let totalno=totalno+1
echo "Test $totalno: Test case construct"
util_longbadcmd1
res=$?
if [ $res = 127 ]
then
   	echo "PASS"
   	let passno=passno+1
else
   	echo "FAIL: Expected 127, returned $res"
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
rm readtest.txt
rmdir chdirtest
rmdir cdtest
rmdir pushdtest
rmdir popdtest
rmdir dirstest
rm redirectedfile
}

#Function calls
preamble
init
test_pwd1
test_echo1
test_print1
# test_printf1
test_read1
test_chdir1
test_cd1
test_pushd1
test_popd1
test_dirs1
test_alias1
# test_unalias1
test_set1
test_unset1
test_which1
test_eval1
test_redirectionop1
test_typeset1
# test_whence1
#test_hash1
test_autoload1

# @internal test function - replacement for builtin pwd
pwd()
{
echo hello
}

test_builtin1
# test_command1
# test_exec1
# test_noglob1
test_open_sq_bkt
test_dot
test_colon
# test_send_job_to_bg
#test_bg1
test_break1
test_continue1
test_declare1
#test_disable1
#test_disown1
test_emulate1
test_export1
test_false1
test_fc1
#test_fg1
test_float1
test_functions1
test_getln1
test_getopts1
test_integer1
#test_jobs1
#test_kill1
test_local1
#test_rehash1
test_shift1
test_source1
test_true1
#test_ttyctl1
#test_trap1
test_type1
#test_umask1
#test_unhash1
test_where1
#test_zmodload1
# test_cdablevars1 # manual test
# test_pipe1
test_precommand_modifier_hiphen1
test_for1
test_until1
test_repeat1
test_case1
# test_longbadcmd1
postamble
report
