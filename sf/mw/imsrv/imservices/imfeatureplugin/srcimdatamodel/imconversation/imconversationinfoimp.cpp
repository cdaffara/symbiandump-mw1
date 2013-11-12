/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MImConversationInfo API object implementation.
*
*/

#include "imconversationinfoimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
//#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CImConversationInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CImConversationInfoImp* CImConversationInfoImp::NewLC()
    {
    CImConversationInfoImp* self = new( ELeave ) CImConversationInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CImConversationInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CImConversationInfoImp* CImConversationInfoImp::NewL()
    {
    CImConversationInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CImConversationInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CImConversationInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CImConversationInfoImp, MImConversationInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CImConversationInfoImp, MImConversationInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CImConversationInfoImp::~CImConversationInfoImp()
// ---------------------------------------------------------------------------
//
CImConversationInfoImp::~CImConversationInfoImp()
    {
    iTextMessage.Close();
    if(iIdentity != NULL)
	    {
	    delete iIdentity;	
	    }
    if(iRecipients != NULL)
    	{
    	iRecipients->Reset();
		delete iRecipients;   
    	}
    
    }

// ---------------------------------------------------------------------------
// CImConversationInfoImp::CImConversationInfoImp()
// ---------------------------------------------------------------------------
//
CImConversationInfoImp::CImConversationInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CImConversationInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CImConversationInfoImp::ConstructL()
    {
    iTextMessage.CreateL( 0 );
    iRecipients = new( ELeave ) CDesCArrayFlat( KArrayGranularity );
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    }

// ---------------------------------------------------------------------------
// CImConversationInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CImConversationInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iIdentity->ExternalizeL( aStream );
    XIMPRBuf16Helper::ExternalizeL( iTextMessage, aStream );
    for(TInt index=0;index<iRecipients->MdcaCount();index++)
		{
			RBuf16 userId;			
			userId.Create(iRecipients->MdcaPoint(index));
	    	XIMPRBuf16Helper::ExternalizeL(userId, aStream );
	    	userId.Close();
		}    
    }

// ---------------------------------------------------------------------------
// CImConversationInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CImConversationInfoImp::InternalizeL( RReadStream& aStream )
    {
    iIdentity->InternalizeL( aStream );
    XIMPRBuf16Helper::InternalizeL( iTextMessage, aStream ); 
    TInt length = 0;
    
    TRAP_IGNORE(length = aStream.ReadInt32L());    
    for ( TInt i = 0; i < length; i++ )
        {
        	RBuf16 userId; 	
   			XIMPRBuf16Helper::GrowIfNeededL( userId, length );
   			aStream.ReadL( userId, length );
			iRecipients->AppendL(userId);
			userId.Close();
			TRAPD(leavecode,length = aStream.ReadInt32L());
			if(leavecode != KErrNone) // if end of the Stream leavecode = KErrEof(-25)
   				break;
		} 
    }
  
    
// ---------------------------------------------------------------------------
// From MImConversationInfo class.
// CImConversationInfoImp::GroupId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CImConversationInfoImp::MessageId() const
    {
    return *iIdentity;
    }


// ---------------------------------------------------------------------------
// From MImConversationInfo class.
// CImConversationInfoImp::TextMessage()
// ---------------------------------------------------------------------------
//
const TDesC16& CImConversationInfoImp::TextMessage() const
    {
    return iTextMessage;
    }
    
// ---------------------------------------------------------------------------
// From MImConversationInfo class.
// CImConversationInfoImp::RecipientL() 
// ---------------------------------------------------------------------------
//
const MDesCArray& CImConversationInfoImp::RecipientL() const
    {
    return *iRecipients;
    }

// ---------------------------------------------------------------------------
// From MImConversationInfo class.
// CImConversationInfoImp::SetMessageIdL()
// ---------------------------------------------------------------------------
//
void CImConversationInfoImp::SetMessageIdL( MXIMPIdentity* aIdentity )
    {
    CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );

    delete iIdentity;
    iIdentity = identityImp; 
    }
  

// ---------------------------------------------------------------------------
// From MImConversationInfo class.
// CImConversationInfoImp::SetTextMessageL()
// ---------------------------------------------------------------------------
//
void CImConversationInfoImp::SetTextMessageL( 
    const TDesC16& aTextMessage )
    {
    HBufC16* TextMessageBuf = aTextMessage.AllocL();
    iTextMessage.Close();
    iTextMessage.Assign( TextMessageBuf );
    }
 
// ---------------------------------------------------------------------------
// From MImConversationInfo class.
// CImConversationInfoImp::SetRecipientL() recipient
// ---------------------------------------------------------------------------
//
void CImConversationInfoImp::SetRecipientL(const MDesCArray* aRecipients) 
    {
	    for(TInt index=0;index<aRecipients->MdcaCount();index++)
		{
			HBufC* userId = aRecipients->MdcaPoint( index ).AllocL();
			CleanupStack::PushL( userId );
	    	iRecipients->AppendL(*userId);
	    	CleanupStack::PopAndDestroy( userId ); // buf
		}
    }
// ---------------------------------------------------------------------------
// CImConversationInfoImp::Identity
// ---------------------------------------------------------------------------
//
EXPORT_C const CXIMPIdentityImp& CImConversationInfoImp::IdentityImp() const
    {
    return *iIdentity;
    }
// ---------------------------------------------------------------------------
// CImConversationInfoImp::SetIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CImConversationInfoImp::SetIdentity( CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }
// ---------------------------------------------------------------------------
// CImConversationInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CImConversationInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& /*aOtherInstance*/ ) const
    {
    return ETrue;
    }

// End of file
