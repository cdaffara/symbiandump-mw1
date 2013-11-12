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
# Inititalises counters
#

init()
{
let totalno=0;
let passno=0;
let failno=0;
}

init
. ./setopttest.sh > setopttest.txt
. ./settest.sh > settest.txt
. ./shifttest.sh > shifttest.txt
. ./typesettest1.sh > typesettest1.txt
. ./typesettest2.sh > typesettest2.txt
. ./unhashtest.sh > unhashtest.txt
. ./unsetopttest.sh > unsetopttest.txt
. ./unsettest.sh > unsettest.txt
. ./whencetest.sh > whencetest.txt
. ./fctest.sh > fctest.txt
. ./cdtest2.sh > cdtest2.txt
. ./unziptest.sh > unziptest.txt