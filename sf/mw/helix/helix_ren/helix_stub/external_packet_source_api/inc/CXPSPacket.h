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



#ifndef __CXPSPACKET_H__
#define __CXPSPACKET_H__

#include <e32base.h>

class CXPSPacketHdr
{
public:
    TUint32 m_uTimeStamp;
	TUint16 m_uSeqNo;
	TUint32 m_uPayloadSize;
	TUint8  m_uMarker;
    TUint8  m_uStreamNumber;
};

class CXPSPacket 
{
public:
    CXPSPacketHdr m_PktHdr;
    TUint8* m_pPayload;
public: 
	CXPSPacket();
    CXPSPacket(CXPSPacketHdr* pHdr);
	CXPSPacket(TUint32 _ts, TUint16 _seq, TUint32 _size, TUint8 _sNo, TUint8 _marker);

	~CXPSPacket();
	TUint8* SetPayload(TUint8* pData) ; 
};




#endif // end of #ifndef __CXPSPACKET_H__
