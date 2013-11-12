/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence Service plugin factory
 *
*/


#include "ximppluginfactory.h"
#include "ximpobjecthelpers.h"
#include "ximpprotocolinfoimp.h"
#include "ximpfeatureinfoimp.h"

#include <ximpprotocolpluginbase.h>

#include <ximpbase.h>
#include <ecom/ecom.h>

// ============================ HELPER CLASS ===============================
// simple wrapper class with array deletion on close
class RImplInfoPtrArray2 : public RImplInfoPtrArray
    {
    public:
        void Close()
            {
            ResetAndDestroy();
            }
    };

_LIT8( KProvider,          "<p>" );
_LIT8( KFeatureId,         "<f>" );
_LIT8( KSupportedSchema,   "<s>" );
_LIT8( KTagMatch,          "*<?>*" );

const TInt KTagLength = 3;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXIMPPluginFactory::NewL()
// Singleton access method.
// -----------------------------------------------------------------------------
//
CXIMPPluginFactory* CXIMPPluginFactory::NewL()
    {
    CXIMPPluginFactory* self = new ( ELeave ) CXIMPPluginFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CXIMPPluginFactory::CXIMPPluginFactory()
// -----------------------------------------------------------------------------
//
CXIMPPluginFactory::CXIMPPluginFactory()
    {
    }

// -----------------------------------------------------------------------------
// CXIMPPluginFactory::~CXIMPPluginFactory()
// -----------------------------------------------------------------------------
//
CXIMPPluginFactory::~CXIMPPluginFactory()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPPluginFactory::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPPluginFactory::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPPluginFactory::GetProtocolsL()
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* CXIMPPluginFactory::GetProtocolsL()
    {
    RXIMPObjOwningPtrArray< CXIMPProtocolInfoImp > protocolInfoArray;
    CleanupClosePushL( protocolInfoArray );

    // For each valid plugin, create CXIMPProtocolInfoImp and fill it up
    RImplInfoPtrArray2 protocols;
    CleanupClosePushL( protocols );

    REComSession::ListImplementationsL( TUid::Uid(
                                        CXIMPProtocolPluginBase::KInterfaceId ),
                                        protocols );

    const TInt implCount = protocols.Count();
    for( TInt i( 0 ); i < implCount; i++ )
        {
        CXIMPFeatureInfoImp* featInfo =
                    ParseProtocolInfoLC( protocols[i]->OpaqueData() );
        CXIMPProtocolInfoImp* tmpProtInfo =
                    CXIMPProtocolInfoImp::NewLC( protocols[i]->DisplayName(),
                                                 protocols[i]->ImplementationUid(),
                                                 *featInfo );
        User::LeaveIfError( protocolInfoArray.Append( tmpProtInfo ) );
        CleanupStack::Pop( tmpProtInfo );
        CleanupStack::PopAndDestroy( featInfo );
        }

    CleanupStack::PopAndDestroy();  // protocols


    HBufC8* pack = TXIMPObjectPacker< CXIMPProtocolInfoImp >::PackArrayL(
                                                            protocolInfoArray );
    CleanupStack::PopAndDestroy(); // protocolInfoArray
    return pack;
    }


// -----------------------------------------------------------------------------
// CXIMPPluginFactory::GetProtocolL()
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPProtocolPluginBase* CXIMPPluginFactory::GetProtocolL( 
    TUid aProtocolUid )
    {
    TAny* requestedProtocol = NULL;
    TUid destructorId;

    requestedProtocol = REComSession::CreateImplementationL( aProtocolUid,
                                                             destructorId );
           
    CXIMPProtocolPluginBase* plugIn = ( CXIMPProtocolPluginBase* )requestedProtocol;
    plugIn->SetDestructorId( destructorId );
    return plugIn;
    }


// -----------------------------------------------------------------------------
// CXIMPPluginFactory::ParseProtocolInfoLC()
// -----------------------------------------------------------------------------
//
CXIMPFeatureInfoImp* CXIMPPluginFactory::ParseProtocolInfoLC(
                                                const TDesC8& aFeaturesBuffer )
    {
    CXIMPFeatureInfoImp* featInfo = CXIMPFeatureInfoImp::NewLC();

    ParseTaggedDataL( aFeaturesBuffer, featInfo );

    return featInfo;
    }


// -----------------------------------------------------------------------------
// CXIMPPluginFactory::ParseTaggedDataL()
// -----------------------------------------------------------------------------
//
void CXIMPPluginFactory::ParseTaggedDataL( const TDesC8& aData,
                                          CXIMPFeatureInfoImp* aFeatInfo )
    {
    TPtrC8 data(aData);
    TInt readPosition = 0;
    TBool moreData = data.Length() ? ETrue : EFalse;
    while (moreData)
        {
        // Assumes that this segment will begin with a tag
        TPtrC8 restOfData = data.Mid(readPosition);

        TInt endPos = restOfData.MatchF(KTagMatch);
        if (endPos == KErrNotFound)
            User::Leave(KErrCorrupt);

        // extract the tag
        TPtrC8 tag = restOfData.Left(KTagLength);

        readPosition += KTagLength;

        // Find the next tag
        restOfData.Set(data.Mid(readPosition));
        endPos = restOfData.MatchF(KTagMatch);

        TPtrC8 tagData;
        if (endPos == KErrNotFound)
            {
            // If we didn't find a tag, we must be at the end of the data
            tagData.Set(restOfData);
            readPosition = restOfData.Length();
            moreData = EFalse;
            }
        else
            {
            tagData.Set(restOfData.Left(endPos));
            readPosition += endPos;
            }

        ProcessTaggedDataL( tag, tagData, aFeatInfo );
        }
    }


// -----------------------------------------------------------------------------
// CXIMPPluginFactory::ProcessTaggedDataL()
// -----------------------------------------------------------------------------
//
void CXIMPPluginFactory::ProcessTaggedDataL( const TDesC8& aTag,
                                            const TDesC8& aData,
                                            CXIMPFeatureInfoImp* aFeatInfo )
    {
    // This parsing handling needs to be updated when 
    // protocol feature handling is properly defined. 

    // Also CXIMPFeatureInfoImp members should be reconsidered 
    // at that point

    if ( aTag == KProvider )
        {
        //SetProviderL( aData );
        }
    else if ( aTag == KFeatureId )
        {
        aFeatInfo->AddFeatureL( aData );
        }
    else if ( aTag == KSupportedSchema )
        {
        //SetSupportedSchemaL( aData );
        }
    else
        {
        User::Leave(KErrCorrupt);
        }
    }
// End of file


