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

test_typeset2()
{
let totalno=totalno+1
echo "Test $totalno: typeset to set a variable to have always floating point value with 2 decimal places"
typeset -F2 var=12.3456789
ret=$?
if [ $ret = 0 ]
then
    if [ $var = 12.35 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"12.35\", returned \"$var\""
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}

test_typeset3()
{
let totalno=totalno+1
echo "Test $totalno: typeset to set a variable to have always value with width 2, zero stripped, left justified"
typeset -ZL2 var=00123
ret=$?
if [ $ret = 0 ]
then
    if [ $var = 12 ]
    then
        echo "PASS"
        let passno=passno+1
    else
        echo "FAIL: Expected \"12\", returned \"$var\""
        let failno=failno+1
    fi
else
    echo "FAIL: Expected return value 0, returned $ret"
    let failno=failno+1
fi
}

test_typeset4()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset name=val
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

unset name
}

test_typeset5()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset name1=val1 name2=val2
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
unset name1,name2
}

test_typeset6()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset name
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
unset name
}

test_typeset7()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset name1 name2
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
unset name1 name2
}

test_typeset8()
{
let totalno=totalno+1
echo "Test $totalno:"
setopt TYPESET_SILENT
typeset name
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
unset name
unsetopt TYPESET_SILENT
}

test_typeset9()
{
let totalno=totalno+1
echo "Test $totalno:"
setopt TYPESET_SILENT
typeset name1 name2
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
unset name1 name2
unsetopt TYPESET_SILENT
}

test_typeset10()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -p
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

test_typeset11()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +p
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

test_typeset12()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -p name
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

test_typeset13()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +p name
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

test_typeset14()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -p name1 name2
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

test_typeset15()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +p name1 name2
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

test_typeset16()
{
let totalno=totalno+1
echo "Test $totalno:"

typeset -p  +h name
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

test_typeset17()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -p  +h name1 name2
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

test_typeset18()
{
let totalno=totalno+1
echo "Test $totalno:"

typeset -p  -h name
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

test_typeset19()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -p  -h name1 name2
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


test_typeset20()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -h name3=val3
typeset -h
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
unset name3
}

test_typeset21()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -h name4=val4
typeset -h name4
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
unset name4
}

test_typeset22()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -T
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

test_typeset23()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -T name6=val61 val62 val63
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
unset name6
}

test_typeset24()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -T name6=val61 val62
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
unset name6
}

test_typeset25()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -xT name6=val61 val62 :
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
unset name6
}


test_typeset26()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -gT name6=val61 val62 :
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


test_typeset27()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -gT 
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

test_typeset28()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -g
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

test_typeset29()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -g name7=val7
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
unset name7
}

test_typeset30()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -g name7
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
unset name7
}

test_typeset31()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +g name7
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
unset name7
}


test_typeset32()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +g
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

test_typeset33()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +g name8=val8
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
unset name8
}

test_typeset34()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +g name8
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
unset name8
}

test_typeset35()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +g name8=val8 name9=val9
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
unset name8 name9
}

test_typeset36()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +g name8 name9
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
unset name8
}

test_typeset37()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -m name8
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
unset name8
}

test_typeset38()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +m name8
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
unset name8
}

test_typeset39()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +m
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


test_typeset40()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -m
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


test_typeset41()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -mf
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


test_typeset42()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +m -f
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

test_typeset43()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +mf
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

test_typeset44()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -m +f
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

test_typeset45()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -m +g name10=val10
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
unset name10
}

test_typeset46()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -m +g name10
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
unset name10
}

test_typeset47()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -m +g 
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

test_typeset48()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -a ordarr
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
unset ordarr
}

test_typeset49()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -A assarr
assarr=(key1 val1 key2 val2)
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
unset assarr
}

test_typeset50()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -A name key1 val1 key2 val2
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
unset name
}

test_typeset51()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -A assarr
assarr=()
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

unset assarr
}

test_typeset52()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -L name11
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
unset name11
}

test_typeset53()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -L name11=12.34
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
unset name11
}

test_typeset54()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -L3 name11=12.34
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
unset name11
}

test_typeset55()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -L
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


test_typeset56()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +L
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


test_typeset57()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -R name11
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
unset name11
}

test_typeset58()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -R name11=12.34
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
unset name11
}

test_typeset59()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -R3 name11=12.34
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
unset name11
}

test_typeset60()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -R3 name11=12.34
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
unset name11
}


test_typeset61()
{let totalno=totalno+1
echo "Test $totalno:"
typeset -aU array1
array1=(1 2 2 3 4)
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

test_typeset62()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -aU 
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

test_typeset63()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -Z
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

test_typeset64()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +Z 
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

test_typeset65()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -Z name13=123
unset name13
}

test_typeset66()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -Z2 name13=123
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
unset name13
}

test_typeset67()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +Z name13=123
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
unset name13
}

test_typeset68()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +Z2 name13=123
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
unset name13
}

test_typeset69()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +a
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
 
test_typeset70()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -a
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

test_typeset71()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +A
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

test_typeset72()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -A
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

test_typeset73()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -tf test_typeset2
typeset +tf test_typeset2
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
unset -f test_typeset2
}

test_typeset74()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -tf 
typeset +tf
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

test_typeset75()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -uf test_typeset2
typeset -uf
typeset +uf test_typeset2
ret=$?
if [ $ret = 1 ]
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
unset -f test_typeset2
}

test_typeset76()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -Uf test_typeset2
typeset -Uf
typeset +Uf test_typeset2
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
unset -f test_typeset2
}

test_typeset77()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +uf
ret=$?
if [ $ret = 1 ]
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

test_typeset78()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +Uf
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

test_typeset79()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +H
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

test_typeset80()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -H
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

test_typeset81()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +H name14=val14
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
unset name14
}

test_typeset82()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -H name15=val15
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
unset name15
}

test_typeset83()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -i2 var15=19.0
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
unset var15
}

test_typeset84()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -i2 var15=19.0
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
unset var15
}

test_typeset85()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +i2 var15=19.0
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
unset var15
}

test_typeset86()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -i 
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

test_typeset87()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +i
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

test_typeset88()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -E2 var15=19.0
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
unset var15
}

test_typeset89()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -E2 var15=19.0
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
unset var15
}

test_typeset90()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +E2 var15=19.0
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
unset var15
}

test_typeset91()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -E
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

test_typeset92()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +E
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


test_typeset93()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -F2 var15=19.0
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
unset var15
}

test_typeset94()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -F2 var15=19.0
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
unset var15
}

test_typeset95()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +F2 var15=19.0
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
unset var15
}

test_typeset96()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -F
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

test_typeset97()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +F
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



test_typeset98()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -l var16
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
unset var16
}

test_typeset99()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -l var16=F
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
unset var16
}

test_typeset100()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +l var16
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
unset var16
}

test_typeset101()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -l
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

test_typeset102()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +l
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


test_typeset103()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -rF2 var15=19.0
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

test_typeset104()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -rF2 var15=19.0
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

test_typeset105()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +rF2 var15=19.0
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

test_typeset106()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -rF
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

test_typeset107()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +rF
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


test_typeset108()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -tF2 var16=19.0
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
unset var16
}

test_typeset109()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -tF2 var16=19.0
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
unset var16
}

test_typeset110()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +tF2 var16=19.0
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
unset var16
}

test_typeset111()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -tF
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

test_typeset112()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +tF
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


test_typeset113()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -u var16
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
unset var16
}

test_typeset114()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -u var16=f
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
unset var16
}

test_typeset115()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +u var16
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
unset var16
}

test_typeset116()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -u
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

test_typeset117()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +u
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

test_typeset118()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -ux var16
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
unset var16
}

test_typeset119()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -ux var16=f
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

test_typeset120()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +ux var16
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
unset var16
}

test_typeset121()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset -ux
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

test_typeset122()
{
let totalno=totalno+1
echo "Test $totalno:"
typeset +ux
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
test_typeset2
test_typeset3
test_typeset4
test_typeset5
test_typeset6
test_typeset7
test_typeset8
test_typeset9
test_typeset10
test_typeset11
test_typeset12
test_typeset13
test_typeset14
test_typeset15
test_typeset16
test_typeset17
test_typeset18
test_typeset19
test_typeset20
test_typeset21
test_typeset22
test_typeset23
test_typeset24
# test_typeset25
test_typeset26
test_typeset27
test_typeset28
test_typeset29
test_typeset30
test_typeset31
test_typeset32
test_typeset33
test_typeset34
test_typeset35
test_typeset36
test_typeset37
test_typeset38
test_typeset39
test_typeset40
test_typeset41
test_typeset42
test_typeset43
test_typeset44
test_typeset45
test_typeset46
test_typeset47
test_typeset48
test_typeset49
test_typeset50
test_typeset51
test_typeset52
test_typeset53
test_typeset54
test_typeset55
test_typeset56
test_typeset57
test_typeset58
test_typeset59
test_typeset60
test_typeset61
test_typeset62
test_typeset63
test_typeset64
test_typeset65
test_typeset66
test_typeset67
test_typeset68
test_typeset69
test_typeset70
test_typeset71
test_typeset72
test_typeset73
test_typeset74
test_typeset75
test_typeset76
test_typeset77
test_typeset78
test_typeset79
test_typeset80
test_typeset81
test_typeset82
test_typeset83
test_typeset84
test_typeset85
test_typeset86
test_typeset87
test_typeset88
test_typeset89
test_typeset90
test_typeset91
test_typeset92
test_typeset93
test_typeset94
test_typeset95
test_typeset96
test_typeset97
test_typeset98
test_typeset99
test_typeset100
test_typeset101
test_typeset102
test_typeset103
test_typeset104
test_typeset105
test_typeset106
test_typeset107
test_typeset108
test_typeset109
test_typeset110
test_typeset111
test_typeset112
test_typeset113
test_typeset114
test_typeset115
test_typeset116
test_typeset117
test_typeset118
test_typeset119
# test_typeset120
# test_typeset121
# test_typeset122
report
