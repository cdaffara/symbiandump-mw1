/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of MXIMPContext
 *
*/

#include "ximpcontextimp.h"
#include "ximpcontexteventpump.h"
#include "ximpserviceinfoimp.h"
#include "ximpfeatureinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpoperationdefs.h"

#include "ximpobjectfactoryimp.h"

#include "ximpcontextclientinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpcontextclientproxy.h"


_LIT( KZeroId, "0" );

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPContextImp::NewLC()
// ---------------------------------------------------------------------------
//
CXIMPContextImp* CXIMPContextImp::NewLC()
    {
    CXIMPContextImp* self = new( ELeave ) CXIMPContextImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPContextImp, 
                                     MXIMPContext )

XIMPIMP_IF_BASE_GET_INTERFACE(MXIMPContextInternal, this)

XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPContextImp, 
                                           MXIMPContext )
XIMPIMP_IF_BASE_GET_CONST_INTERFACE(MXIMPContextInternal, this)

XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CXIMPContextImp::~CXIMPContextImp()
// ---------------------------------------------------------------------------
//
CXIMPContextImp::~CXIMPContextImp()
    {
    delete iObjFactory;
    delete iEventPump;
    delete iCtxProxy;
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::CXIMPContextImp()
// ---------------------------------------------------------------------------
//
CXIMPContextImp::CXIMPContextImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPContextImp::ConstructL()
    {
    iCtxProxy = new ( ELeave ) CXIMPContextClientProxy;
    iEventPump = CXIMPContextEventPump::NewL( *this, *iCtxProxy );
    iObjFactory = CXIMPObjectFactoryImp::NewL();
    }

// ---------------------------------------------------------------------------
// CXIMPContextImp::RegisterObserverL()
// ---------------------------------------------------------------------------
//
void CXIMPContextImp::RegisterObserverL(
        MXIMPContextObserver& aObserver,
        const TArray< TInt32 >* aEventFilter )
    {
    iEventPump->RegisterObserverL( aObserver, aEventFilter );
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::UnregisterObserver()
// ---------------------------------------------------------------------------
//
void CXIMPContextImp::UnregisterObserver(
        MXIMPContextObserver& aObserver )
    {
    iEventPump->UnRegisterObserver( aObserver );
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::BindToL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextImp::BindToL(
        TUid aProtocolImpUid,
        const TDesC16& aServiceAddress,
        const TDesC16& aUsername,
        const TDesC16& aPassword,
        TInt32 aIapId )

    {
    //Setup context connection
    
    return BindToL( aProtocolImpUid,
                    aServiceAddress,
                    aUsername,
                    aPassword,
                    KZeroId,
                    aIapId );

    }

// ---------------------------------------------------------------------------
// CXIMPContextImp::BindToL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextImp::BindToL(
        TUid aProtocolImpUid,
        const TDesC16& aServiceAddress,
        const TDesC16& aUsername,
        const TDesC16& aPassword,
        const TDesC16& aClientId,
        TInt32 aIapId )

    {
    //Setup context connection
    iCtxProxy->InitServerSessionLC( aProtocolImpUid ); //Pushes session close
    iEventPump->StartEventPumpLC(); //Pushes pump close


    //Combine bind parameters and issue request
    CXIMPServiceInfoImp* serviceInfo = CXIMPServiceInfoImp::NewLC( aProtocolImpUid,
                                                                   aServiceAddress,
                                                                   aUsername,
                                                                   aPassword,
                                                                   aIapId );


    CXIMPContextClientInfoImp* clientInfo = CXIMPContextClientInfoImp::NewLC();
        {
        CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
        identity->SetIdentityL( aClientId );
        clientInfo->SetClientIdL( identity );
        CleanupStack::Pop( identity );
        }


    HBufC8* packBufDesc = NULL;
        {
        CBufFlat* packBuf = CBufFlat::NewL( KObjectBufGranurality );
        CleanupStack::PushL( packBuf );

        RBufWriteStream ws;
        ws.Open( *packBuf ); // CSI: 65 #
        CleanupClosePushL( ws );

        serviceInfo->ExternalizeL( ws );
        clientInfo->ExternalizeL( ws );

        ws.CommitL();
        CleanupStack::PopAndDestroy(); //ws

        packBufDesc = packBuf->Ptr(0).AllocL();
        CleanupStack::PopAndDestroy( packBuf ); 
        }

    CleanupStack::PushL( packBufDesc );

    TXIMPRequestId reqId = iCtxProxy->QueueAsyncConnectionOpenL( NXIMPOps::EXIMPBindContext,
                                                                 *packBufDesc );

    CleanupStack::PopAndDestroy( packBufDesc );
    CleanupStack::PopAndDestroy( clientInfo );
    CleanupStack::PopAndDestroy( serviceInfo );
    

    CleanupStack::Pop(); //Pop the pump close
    CleanupStack::Pop(); //Pop the session close

    return reqId;
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::BindToL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextImp::BindToL(
        TUid aProtocolImpUid,
        TInt aSettingsId )
        
    {
    //Setup context connection
    iCtxProxy->InitServerSessionLC( aProtocolImpUid ); //Pushes session close
    iEventPump->StartEventPumpLC(); //Pushes pump close


    //Combine bind parameters and issue request
    CXIMPServiceInfoImp* serviceInfo = CXIMPServiceInfoImp::NewLC( aProtocolImpUid,
                                                                   KNullDesC,
                                                                   KNullDesC,
                                                                   KNullDesC,
                                                                   aSettingsId );

     CXIMPContextClientInfoImp* clientInfo = CXIMPContextClientInfoImp::NewLC();
        {
        CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
        identity->SetIdentityL( KZeroId );
        clientInfo->SetClientIdL( identity );
        CleanupStack::Pop( identity );
        }


    HBufC8* packBufDesc = NULL;
        {
        CBufFlat* packBuf = CBufFlat::NewL( KObjectBufGranurality );
        CleanupStack::PushL( packBuf );

        RBufWriteStream ws;
        ws.Open( *packBuf ); // CSI: 65 #
        CleanupClosePushL( ws );

        serviceInfo->ExternalizeL( ws );
        clientInfo->ExternalizeL( ws );

        ws.CommitL();
        CleanupStack::PopAndDestroy(); //ws

        packBufDesc = packBuf->Ptr(0).AllocL();
        CleanupStack::PopAndDestroy( packBuf ); 
        }

    CleanupStack::PushL( packBufDesc );

    TXIMPRequestId reqId = iCtxProxy->QueueAsyncConnectionOpen2L( NXIMPOps::EXIMPBindContext2,
                                                                 *packBufDesc );

    CleanupStack::PopAndDestroy( packBufDesc );
    CleanupStack::PopAndDestroy( clientInfo );
    CleanupStack::PopAndDestroy( serviceInfo );
    

    CleanupStack::Pop(); //Pop the pump close
    CleanupStack::Pop(); //Pop the session close

    return reqId;
    }

// ---------------------------------------------------------------------------
// CXIMPContextImp::BindToL()
// ---------------------------------------------------------------------------
//
 TXIMPRequestId CXIMPContextImp::BindToL( TUid aProtocolImpUid,
			                const TDesC16& aUsername,
			                const TDesC16& aPassword,
			                TInt  aSettingsId )
	{
	 //Setup context connection
    iCtxProxy->InitServerSessionLC( aProtocolImpUid ); //Pushes session close
    iEventPump->StartEventPumpLC(); //Pushes pump close


    //Combine bind parameters and issue request
    CXIMPServiceInfoImp* serviceInfo = CXIMPServiceInfoImp::NewLC( aProtocolImpUid,
                                                                   KNullDesC,
                                                                   aUsername,
                                                                   aPassword,
                                                                   aSettingsId );

     CXIMPContextClientInfoImp* clientInfo = CXIMPContextClientInfoImp::NewLC();
        {
        CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
        identity->SetIdentityL( KZeroId );
        clientInfo->SetClientIdL( identity );
        CleanupStack::Pop( identity );
        }


    HBufC8* packBufDesc = NULL;
        {
        CBufFlat* packBuf = CBufFlat::NewL( KObjectBufGranurality );
        CleanupStack::PushL( packBuf );

        RBufWriteStream ws;
        ws.Open( *packBuf ); // CSI: 65 #
        CleanupClosePushL( ws );

        serviceInfo->ExternalizeL( ws );
        clientInfo->ExternalizeL( ws );

        ws.CommitL();
        CleanupStack::PopAndDestroy(); //ws

        packBufDesc = packBuf->Ptr(0).AllocL();
        CleanupStack::PopAndDestroy( packBuf ); 
        }

    CleanupStack::PushL( packBufDesc );

    TXIMPRequestId reqId = iCtxProxy->QueueAsyncConnectionOpenL( NXIMPOps::EXIMPBindContext2,
                                                                 *packBufDesc );

    CleanupStack::PopAndDestroy( packBufDesc );
    CleanupStack::PopAndDestroy( clientInfo );
    CleanupStack::PopAndDestroy( serviceInfo );
    

    CleanupStack::Pop(); //Pop the pump close
    CleanupStack::Pop(); //Pop the session close

    return reqId;	
	}
// ---------------------------------------------------------------------------
// CXIMPContextImp::UnbindL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextImp::UnbindL()
    {
    return iCtxProxy->QueueAsyncConnectionCloseL( NXIMPOps::EXIMPUnbindContext,
                                                  KNullDesC8 );
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::GetContextFeaturesLC()
// ---------------------------------------------------------------------------
//
MXIMPFeatureInfo* CXIMPContextImp::GetContextFeaturesLC() const
    {
    CXIMPFeatureInfoImp* features = CXIMPFeatureInfoImp::NewLC();

    HBufC8* featurePck = iCtxProxy->DirectSessionL().GetContextFeaturesL();
    CleanupStack::PushL( featurePck );
    TXIMPObjectPacker<CXIMPFeatureInfoImp>::UnPackL( *features, *featurePck );
    CleanupStack::PopAndDestroy( featurePck );

    return features;
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::ObjectFactory()
// ---------------------------------------------------------------------------
//
MXIMPObjectFactory& CXIMPContextImp::ObjectFactory() const
    {
    return *iObjFactory;
    }


// ---------------------------------------------------------------------------
// CXIMPContextImp::ObjectFactory()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CXIMPContextImp::QueueOperationL( TInt aOperationType, 
                                const TDesC8& aOperationParams )
    {
    return iCtxProxy->QueueOperationL(aOperationType, aOperationParams );
    }

// ---------------------------------------------------------------------------
// CXIMPContextImp::ConvertStringToUidL()
// ---------------------------------------------------------------------------
//
void CXIMPContextImp::ConvertStringToUidL( const TDesC8& aData,
        TUid& aUid )
    {
    // Make sure aData is in the correct format - "0x12345678"
    _LIT8(K0x, "0x");
    _LIT8(K0X, "0X");

    if( (aData.Length() == 10) &&
            ( (aData.FindF( K0x ) == 0) || ( aData.FindF( K0X ) == 0 ) ) )
        {
        // only take the right 8 characters (ie discard the "0x")
        TLex8 lex( aData.Right( 8 ) );
        TUint32 value = 0;
        User::LeaveIfError( lex.Val( value, EHex ) );
        aUid.iUid = value;
        }
    else
        {
        User::Leave( KErrCorrupt );
        }
    }


