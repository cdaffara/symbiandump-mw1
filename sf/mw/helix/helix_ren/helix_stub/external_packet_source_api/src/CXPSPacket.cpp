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



#include <stdio.h>
#include <string.h>
#include <e32cmn.h>

#include "CXPSPacket.h"


CXPSPacket::CXPSPacket(TUint32/* _ts*/, TUint16 /*_seq*/, TUint32 /*_size*/, TUint8 /*_sNo*/, TUint8 /*_marker*/)
{
   // do Nothing
}

CXPSPacket::CXPSPacket(CXPSPacketHdr* /* pHdr*/)
{
    // do Nothing
}


TUint8* CXPSPacket::SetPayload(TUint8* /*pBuff*/)
{
    
    
    return NULL;
}

CXPSPacket::~CXPSPacket()
{
    
   // Do Nothing
}
