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
. ./printtest.sh > printtest.txt
. ./sanity_auto.sh > sanity_auto.txt
. ./aliastest.sh > aliastest.txt
. ./breaktest.sh > breaktest.txt
. ./continuetest.sh > continuetest.txt
. ./dirstest.sh > dirstest.txt
. ./dottest.sh > dottest.txt
. ./evaltest.sh > evaltest.txt
#. ./exittest.sh
. ./functionstest.sh > functionstest.txt
. ./pwdtest.sh > pwdtest.txt
. ./cdtest1.sh > cdtest1.txt
. ./pstest.sh > pstest.txt
. ./miniziptest.sh > miniziptest.txt