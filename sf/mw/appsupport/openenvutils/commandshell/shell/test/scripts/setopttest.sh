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

# Set the autocd option for the shell, so that simply typing the dir name will cd to that dir
test_setopt1()
{
let totalno=totalno+1
echo "Test $totalno: setopt to automatically cd to a directory without requiring you to type cd"
if [ -d cdtest ]
then
     oldcwd=`pwd`
     setopt auto_cd
     cdtest
     ret=$?
     if [ $ret = 0 ]
     then
         if [ $PWD = ${oldcwd}/cdtest ]
         then
             echo "PASS"
             let passno=passno+1
         else
             echo "FAIL: Expected \"$PWD\", returned \"${oldcwd}/cdtest\""
             let failno=failno+1
             return
         fi
         cd ..
     else
         echo "FAIL: Expected return value 0, but  returned $ret"
         let failno=failno+1
         return
     fi
     unsetopt auto_cd
else
     echo "FAIL:cdtest dir missing"
     let failno=failno+1
fi
}


test_nobeep1()
{
let totalno=totalno+1
echo "Test $totalno: setopt nobeep"
setopt nobeep              # i hate beeps
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt nobeep
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_noautomenu1()
{
let totalno=totalno+1
echo "Test $totalno: setopt noautomenu"
setopt noautomenu          # don't cycle completions
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt noautomenu
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_autopushd1()
{
let totalno=totalno+1
echo "Test $totalno: setopt autopushd"
setopt autopushd           # automatically append dirs to the push/pop list
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt autopushd
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_pushdignoredups1()
{
let totalno=totalno+1
echo "Test $totalno: setopt pushdignoredups"
setopt pushdignoredups     # and don't duplicate them
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt pushdignoredups
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_checkjobs1()
{
let totalno=totalno+1
echo "Test $totalno: setopt checkjobs"
setopt checkjobs           # warn me about bg processes when exiting
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt checkjobs
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_nohup1()
{
let totalno=totalno+1
echo "Test $totalno: setopt nohup"
setopt nohup               # and don't kill them, either
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt nohup
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_listpacked1()
{
let totalno=totalno+1
echo "Test $totalno: setopt listpacked"
setopt listpacked          # compact completion lists
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt listpacked
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_listtypes1()
{
let totalno=totalno+1
echo "Test $totalno: setopt listtypes"
setopt listtypes           # show types in completion
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt listtypes
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_completeinword1()
{
let totalno=totalno+1
echo "Test $totalno: setopt completeinword"
setopt completeinword      # not just at the end
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt completeinword
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_alwaystoend1()
{
let totalno=totalno+1
echo "Test $totalno: setopt alwaystoend"
setopt alwaystoend         # when complete from middle, move cursor
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt alwaystoend
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_correct1()
{
let totalno=totalno+1
echo "Test $totalno: setopt correct"
setopt correct             # spelling correction
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt correct
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_nopromptcr1()
{
let totalno=totalno+1
echo "Test $totalno: setopt nopromptcr"
setopt nopromptcr          # don't add \r which overwrites cmd output with no \n
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt nopromptcr
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_histverify1()
{
let totalno=totalno+1
echo "Test $totalno: setopt histverify"
setopt histverify          # when using ! cmds, confirm first
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt histverify
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_interactivecomments1()
{
let totalno=totalno+1
echo "Test $totalno: setopt interactivecomments"
setopt interactivecomments # escape commands so i can use them later
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt interactivecomments
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_recexact1()
{
let totalno=totalno+1
echo "Test $totalno: setopt recexact"
setopt recexact            # recognise exact, ambiguous matches
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt recexact
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_printexitvalue1()
{
let totalno=totalno+1
echo "Test $totalno: setopt printexitvalue"
setopt printexitvalue      # alert me if something's failed
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt printexitvalue
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_glob1()
{
let totalno=totalno+1
echo "Test $totalno: setopt glob"
setopt glob                # globbing support
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt glob
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_extendedglob1()
{
let totalno=totalno+1
echo "Test $totalno: setopt extendedglob"
setopt extendedglob        # blah blah
ret=$?
if [ $ret = 0 ]
then
    echo "PASS"
    let passno=passno+1
    unsetopt extendedglob
else
    echo "FAIL: Expected 0, returned $ret"
    let failno=failno+1
fi
}

test_nonomatch1()
{
let totalno=totalno+1
echo "Test $totalno: setopt nonomatch"
setopt nonomatch           # pass through any unrecognized patterns to the command
ret=$?
if [ $ret = 0 ]
then
    {
    echo "PASS"
    let passno=passno+1
    unsetopt nonomatch
    }
else
    {
    echo "FAIL: Expected 0, returned $ret"
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

# test_setopt1
test_nobeep1
test_noautomenu1
test_autopushd1
test_pushdignoredups1
test_checkjobs1
test_nohup1
test_listpacked1
test_listtypes1
test_completeinword1
test_alwaystoend1
test_correct1
test_nopromptcr1
test_histverify1
test_interactivecomments1
test_recexact1
test_printexitvalue1
test_glob1
test_extendedglob1
test_nonomatch1
report
