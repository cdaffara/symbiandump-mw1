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




#ifndef _SYMBIAN_TE_UTIL_OBS_H_
#define _SYMBIAN_TE_UTIL_OBS_H_

#include <e32std.H>
#include <FBS.H>

class MHXThumbnailUtilityImplObserver
{
    public:
    virtual void MetaDataReady(TInt aError)  = 0;
    virtual void PacketReady(TInt aError, void *pData, TUint32 aDataSize) = 0;    
    virtual void EndOfPackets() = 0;
};

#endif _SYMBIAN_TE_UTIL_OBS_H_

