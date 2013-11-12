/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#include "tstaskchangeinfo.h"

TsTaskChangeInfo::TsTaskChangeInfo(int newOffset, int oldOffset) :
    mNewOffset(newOffset), mOldOffset(oldOffset)
{
}
 
int TsTaskChangeInfo::newOffset() const
{
    return mNewOffset;
}

int TsTaskChangeInfo::oldOffset() const
{
    return mOldOffset;
}

TsTaskChangeInfo::ChangeType TsTaskChangeInfo::changeType() const
{
    ChangeType retVal(EChangeInsert);
    if (KInvalidOffset == mNewOffset) {
        if (KInvalidOffset == mOldOffset) {
            retVal = EChangeCancel;
		} else {
            retVal = EChangeDelete;
		}
    } else if (mOldOffset == mNewOffset) {
        retVal = EChangeUpdate;
    } else if(KInvalidOffset != mOldOffset) {
        retVal = EChangeMove;
    }
    return retVal;
}
