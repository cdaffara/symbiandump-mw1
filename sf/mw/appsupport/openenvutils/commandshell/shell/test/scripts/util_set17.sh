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

arg1=$1;arg2=$2;arg3=$3
old=(`echo $@`)
set a b c
if [ $1 = a ] && [ $2 = b ] && [ $3 = c ]
then
 ret=0
else
 ret=1
fi
echo "First set returned $ret"
set $old
if [ $1 = $arg1 ] && [ $2 = $arg2 ] && [ $3 = $arg3 ]
then
 ret=0
else
 ret=1
fi
echo "second set returned $ret"
echo $ret > result_set.txt
