/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of the CProEngToneHandler.
*
*/



// INCLUDE FILES
#include    "CProEngToneHandler.h"

#include    <DRMHelper.h>
#include    <DRMCommon.h>
#include    <ProEngWrapper.rsg>


// ============================ LOCAL FUNCTIONS ================================

CDRMHelper::TDRMHelperAutomatedType SettingIdToAutomatedType( TProfileSettingId aSettingId )
    {
    switch( aSettingId )
        {
        case EProfileSettingIdRingingTone:
        case EProfileSettingIdRingingTone2:
            {
            return CDRMHelper::EAutomatedTypeRingingTone;
            }
        case EProfileSettingIdMsgTone:
            {
            return CDRMHelper::EAutomatedTypeMessageAlert;
            }
        case EProfileSettingIdEmailTone:
            {
            return CDRMHelper::EAutomatedTypeEmailAlert;
            }
        default:
            {
            return CDRMHelper::EAutomatedTypeOther;
            }
        }
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngToneHandler::DrmConstructL
// 2nd phase construct for DRM objects
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::DrmConstructL()
    {
    iDrmCommon = DRMCommon::NewL();
    iDrmHelper = CDRMHelper::NewL();
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::ReleaseDrm
// Destructor for DRM objects
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::ReleaseDrm()
    {
    delete iDrmHelper;
    iDrmHelper = NULL;
    delete iDrmCommon;
    iDrmCommon = NULL;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::CheckProtectedFileL
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::CheckProtectedFileL( const TDesC& aFileName )
    {
    DRMCommon::TContentProtection protection;
    HBufC8* mimeType = NULL; // ignored
    HBufC8* contentUri = NULL;
    TUint dataLength; // ignored
    TInt err = iDrmCommon->GetFileInfo( aFileName, protection, mimeType,
                                        contentUri, dataLength );
    delete mimeType;
    if( err )
        {
        delete contentUri;
        User::Leave( err );
        }
    CleanupStack::PushL( contentUri );

    TInt result( KErrGeneral );
    CDRMRights* rights = NULL;
    err = iDrmCommon->GetActiveRights( *contentUri, DRMCommon::EPlay, rights );

    CleanupStack::PopAndDestroy( contentUri );

    if( rights == NULL )
        {
        if( err == DRMCommon::ENoRights )
            {
            // Handle "No rights" situation here
            iDrmHelper->HandleErrorL( DRMCommon::ENoRights, aFileName );
            return result;
            }
        else
            {
            User::Leave( KErrGeneral );
            }
        }

    CleanupStack::PushL( rights );
    
	switch ( err )
		{
		case CDRMRights::EFullRights: // Fall through
		case CDRMRights::ERestrictedRights:
			{
			CDRMRights::TRestriction restriction; // ignore
			CDRMRights::TExpiration expiration;
			TUint32 constType;
			
			rights->GetRightsInfo( DRMCommon::EPlay, restriction, // ignore return
                                   expiration, constType );       // value
		
            // This is CFM protected file, ringingtone is set to "no"
            if ( ( rights->GetPermission().iInfoBits & ENoRingingTone )
                // This is normal DRM protected count-based tone
                || ( constType & CDRMRights::ECountBased ) )
                    {
                    ShowErrorNoteL( R_PROENG_TEXT_DRM_PREV_RIGHTS_SET );
                    }		
                else
                    {
                    switch ( expiration )
                        {
                        case CDRMRights::EValidRights:
                            {
                            result = KErrNone;
                            break;
                            }

                        case CDRMRights::EFutureRights:
                            {
                            iDrmHelper->HandleErrorL( DRMCommon::ENoRights,
                                                      aFileName );
                            break;
                            }

                        case CDRMRights::EExpiredRights: 
                            {
                            iDrmHelper->HandleErrorL( DRMCommon::ERightsExpired,
                                                      aFileName );
                            break;
                            }

                        default:
                            {
                            break;
                            }
                        }	

                    }
				

			break;
			}

		case CDRMRights::EPreviewRights:
			{
			ShowErrorNoteL( R_PROENG_TEXT_DRM_PREV_RIGHTS_SET );
			break;
			}

		case DRMCommon::ENoRights:
			{
			iDrmHelper->HandleErrorL( DRMCommon::ENoRights, aFileName );
			break;
			}

		default:
			{
			break;
			}

		}
   
    CleanupStack::PopAndDestroy( rights );

	return result;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::CanSetAutomated
// -----------------------------------------------------------------------------
//
TBool CProEngToneHandler::CanSetAutomated( const TDesC& aFileName )
    {
    TBool canSetAutomated( EFalse );
    return
        ( iDrmHelper->CanSetAutomated( aFileName, canSetAutomated ) == KErrNone
        && canSetAutomated );
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::AskAutomated
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::AskAutomated( const TDesC& aFileName )
    {
    return iDrmHelper->ShowAutomatedNote( aFileName );
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::SetAutomated
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::SetAutomated( TProfileSettingId aSettingId,
                                       const TDesC& aFileName )
    {
    TInt err = KErrNone;

    TRAP_IGNORE( err = SetAutomatedL( aSettingId, aFileName ) );

    return err;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::SetAutomatedL
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::SetAutomatedL( TProfileSettingId aSettingId,
                                       const TDesC& aFileName )
    {
    iDrmHelper->SetAutomatedType( SettingIdToAutomatedType( aSettingId ) );
    TInt err( iDrmHelper->SetAutomatedSilent( aFileName, EFalse ) );
    if( err == KErrNone )
        {
        using namespace ContentAccess;
       	TVirtualPathPtr path( aFileName, KDefaultContentObject );
        CData* data = CData::NewLC( path, EContentShareReadWrite );
        err = data->ExecuteIntent( EPlay );
        CleanupStack::PopAndDestroy( data );
        // According to Risto Vilkman from DRM, KErrAccessDenied
        // can be ignored at this point, since if
        // CanSetAutomated says the tone is OK, it's OK.
        if( err == KErrAccessDenied )
        	{
        	err = KErrNone;
        	}
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::RemoveAutomated
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::RemoveAutomated( const TDesC& aFileName )
    {
    iDrmHelper->RemoveAutomatedPassive( aFileName ); // ignore return value
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::IsProtected
// -----------------------------------------------------------------------------
//
TBool CProEngToneHandler::IsProtected( const TDesC& aFileName ) const
    {
    TBool result = EFalse;
    TRAP_IGNORE( result = IsProtectedL( aFileName ) );
    return result;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::IsProtectedL
// -----------------------------------------------------------------------------
//
TBool CProEngToneHandler::IsProtectedL( const TDesC& aFileName ) const
    {
    TInt isProtected( EFalse );
   	ContentAccess::TVirtualPathPtr path( aFileName,
   						ContentAccess::KDefaultContentObject );
   	CData* data = CData::NewLC( path, EContentShareReadWrite );
   	TInt result = data->GetAttribute( EIsProtected, isProtected );
   	CleanupStack::PopAndDestroy(); // data

    return( ( result == DRMCommon::EOk ) && isProtected );
    }

//  End of File
