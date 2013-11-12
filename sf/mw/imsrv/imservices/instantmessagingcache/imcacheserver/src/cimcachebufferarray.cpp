/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  a message class implementation 
*
*/


#include "cimcachebufferarray.h"
 //logs
#include "imcachedebugtrace.h"

	
//-----------------------------------------------------------------------------
// CIMCacheBufferArray::NewL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCacheBufferArray* CIMCacheBufferArray::NewL(const TDesC8& aPacketBuffer , TIMCacheOperationsCode aOperationCode )
	{
	TRACE( T_LIT( "CIMCacheBufferArray::NewL begin") );
	CIMCacheBufferArray* self = new ( ELeave ) CIMCacheBufferArray( aOperationCode );
	CleanupStack::PushL( self );
	self->ConstructL( aPacketBuffer );
	CleanupStack::Pop( self );
	TRACE( T_LIT( "CIMCacheBufferArray::NewL end") );
	return self;
	}
//-----------------------------------------------------------------------------
// CIMCacheBufferArray::CIMCacheBufferArray
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCacheBufferArray::CIMCacheBufferArray( TIMCacheOperationsCode aOperationCode )
	:iOperationCode ( aOperationCode )
	{
	TRACE( T_LIT( "CIMCacheBufferArray::CIMCacheBufferArray") );
	}

	
//-----------------------------------------------------------------------------
// CIMCacheBufferArray::ConstructL
// ( Other items commented in header )
//-----------------------------------------------------------------------------
void CIMCacheBufferArray::ConstructL( const TDesC8& aPacketBuffer )
    {
    TRACE( T_LIT( "CIMCacheBufferArray::NewL start") );
    iPacketBuffer = aPacketBuffer.AllocL();
    TRACE( T_LIT( "CIMCacheBufferArray::NewL end") );
    }
	
//-----------------------------------------------------------------------------
// CIMCacheBufferArray::OperationCode
// ( Other items commented in header )
//-----------------------------------------------------------------------------
TIMCacheOperationsCode CIMCacheBufferArray::OperationCode()
    {
    TRACE( T_LIT( "CIMCacheBufferArray::OperationCode") );
   	return iOperationCode;
    }
    
 //-----------------------------------------------------------------------------
// CIMCacheBufferArray::PacketData
// ( Other items commented in header )
//-----------------------------------------------------------------------------
const TDesC8& CIMCacheBufferArray::PacketData()
    {
    TRACE( T_LIT( "CIMCacheBufferArray::PacketData") );
   	return *iPacketBuffer;
    }
//-----------------------------------------------------------------------------
// CIMCacheBufferArray::~CIMCacheBufferArray
// ( Other items commented in header )
//-----------------------------------------------------------------------------
CIMCacheBufferArray::~CIMCacheBufferArray()
	{
	TRACE( T_LIT( "CIMCacheBufferArray::~CIMCacheBufferArray start") );
	delete iPacketBuffer;	
	TRACE( T_LIT( "CIMCacheBufferArray::~CIMCacheBufferArray end") );
	}

    
// end of file
