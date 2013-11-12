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

test_typeset123()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -A name18
typeset "name18[1]"=100
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset "name18[1]"
unset name18
}
 


test_typeset124()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -a name19
name19+=('key with "*strange*" characters' 'value string')
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name19
}

test_typeset125()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -A name20
typeset "name20[one\"two\"three\"quotes]"=QQQ
print "$name20[one\"two\"three\"quotes]"
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name20
}


test_typeset126()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -A name21 
name21[(e)*]=star
print $name21[(e)*]
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name21
}


test_typeset127()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset PATH=$PATH:c:\sys\bin
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset128()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -h PATH
typeset PATH
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}


test_typeset129()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +h special
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset special
}



test_typeset130()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +h BAUD
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset131()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -h BAUD
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}


test_typeset132()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -h status
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset133()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset +h CPUTYPE=i686
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset134()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -h CPUTYPE=i686
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}


test_typeset135()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset CPUTYPE=i686
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset136()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -F name22=3.14
typeset -E name22=3.14
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name22
}

test_typeset137()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -rF name23=3.14
typeset -E name23=3.14
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name23
}

test_typeset138()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -rF name24=3.14
typeset -F name24=3.14
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name24
}

test_typeset139()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset ARGC=4
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}


util_typeset140_1()
{
typeset -F SECONDS
typeset -p SECONDS
typeset -E SECONDS
typeset -p SECONDS
typeset -i SECONDS
typeset -p SECONDS
typeset -ri SECONDS
typeset -p SECONDS
}

util_typeset140_2()
{
typeset -i SECONDS
typeset -p SECONDS
typeset -F SECONDS
typeset -p SECONDS
typeset -E SECONDS
typeset -p SECONDS
typeset -ri SECONDS
typeset -p SECONDS
}


test_typeset140()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -F SECONDS
typeset -E SECONDS
util_typeset140_1
typeset -F SECONDS
util_typeset140_2
typeset -i SECONDS 
typeset -ri SECONDS
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset141()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset name25=val
unset name25
typeset name25=val1
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name25
}


test_typeset142()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset name26=val
unset name26
typeset name26=val
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset143()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -i name27=10
unset name27
typeset -F name27=10.24
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset name27
}

test_typeset144()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset path
path[1]=c:\sys\bin
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset path
}

test_typeset145()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -A path=c:\sys\bin
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

util_typeset146()
{
typeset -l SECONDS
}
test_typeset146()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -i SECONDS
util_typeset146
typeset -i SECONDS
typeset -i SECONDS=100
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset147()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -i ZSH_VERSION
typeset -L ZSH_VERSION
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

test_typeset148()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -U LOGNAME
typeset -u LOGNAME
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
echo $LOGNAME
}

# readonly unique
test_typeset149()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -rU array1 
array1=(1 2 3 3 4)
echo $array1
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset array1
}

# unique elemented array redefinition
test_typeset150()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -aU array2
array2=(1 2 3 3 4)
typeset -aU array2
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
echo $array2
unset array2

}

# unique elemented array redefinition
test_typeset151()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -aU array3
array3=(1 2 3 3 4)
typeset -aU array3
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
unset array3
echo $array3

}


# try chage env
test_typeset152()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset TERM
typeset -p TERM
typeset -u TERM=xterm
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
echo $TERM
}


util_typeset153_1()
{
typeset -F seconds 
typeset -p seconds
typeset -E seconds
typeset -p seconds
typeset -i seconds
typeset -p seconds
typeset -ri seconds
typeset -p seconds
}

util_typeset153_2()
{
typeset -i seconds
typeset -p seconds
typeset -F seconds
typeset -p seconds
typeset -E seconds
typeset -p seconds
typeset -ri seconds
typeset -p seconds
}


test_typeset153()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -F seconds
typeset -E seconds
util_typeset140_1
typeset -F seconds
util_typeset140_2
typeset -i seconds
typeset -ri seconds
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

# emulataion mode sh - typeset a non-typed special variable pipestatus
test_typeset154()
{
let totalno=totalno+1
echo "Test $totalno: "
emulate sh
typeset -p pipestatus
typeset -A pipestatus
emulate -Z zsh
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

util_typeset155_1()
{
typeset -F SECONDS=12.34
typeset -p SECONDS
typeset -E SECONDS=12.35
typeset -p SECONDS
typeset -i SECONDS=34
typeset -p SECONDS
typeset -ri SECONDS=45
typeset -p SECONDS
}

util_typeset155_2()
{
typeset -i SECONDS=12
typeset -p SECONDS
typeset -F SECONDS=13.35
typeset -p SECONDS
typeset -E SECONDS=13.36
typeset -p SECONDS
typeset -ri SECONDS=36
typeset -p SECONDS
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}


test_typeset155()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -F SECONDS=12.12
typeset -E SECONDS=12.13
util_typeset155_1
typeset -F SECONDS=14.14
util_typeset155_2
typeset -i SECONDS=15
typeset -ri SECONDS=16
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
fi
}

util_typeset156_1()
{
typeset -mF SECONDS
typeset -mp SECONDS
typeset -mE SECONDS
typeset -mp SECONDS
typeset -mi SECONDS
typeset -mp SECONDS
typeset -mri SECONDS
typeset -mp SECONDS
}

util_typeset156_2()
{
typeset -mi SECONDS
typeset -mp SECONDS
typeset -mF SECONDS
typeset -mp SECONDS
typeset -mE SECONDS
typeset -mp SECONDS
typeset -mri SECONDS
typeset -mp SECONDS
}


test_typeset156()
{
let totalno=totalno+1
echo "Test $totalno: "
typeset -mF SECONDS
typeset -mE SECONDS
util_typeset140_1
typeset -mF SECONDS
util_typeset140_2
typeset -mi SECONDS
typeset -mri SECONDS
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    }
else
    {
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
    }
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
test_typeset123
test_typeset124
test_typeset125
test_typeset126
test_typeset127
test_typeset128
test_typeset129
test_typeset130
test_typeset131
test_typeset132
test_typeset133
test_typeset134
test_typeset135
test_typeset136
# test_typeset137
# test_typeset138
test_typeset139
test_typeset140
test_typeset141
test_typeset142
test_typeset143
test_typeset144
# test_typeset145
test_typeset146
test_typeset147
# test_typeset148 panics
# test_typeset149
test_typeset150
test_typeset151
test_typeset152
test_typeset153
# test_typeset154
# test_typeset155
test_typeset156
report
