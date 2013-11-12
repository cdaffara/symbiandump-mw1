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
* Description:  Resource reader utility classes for key event map.
*
*/


// INCLUDES
#include "kefresource.h"
#include "keflogger.h"
#include "kefmapinternal.hrh"

// CONSTANTS

// Granularity of key event array.
const TInt KKefKeyEventArrayGranularity = 5;

// ============================ MEMBER FUNCTIONS ===============================

//------------------------------------------------------------------------------
// CKefResource::NewLC
//------------------------------------------------------------------------------
//
CKefResource* CKefResource::NewLC( TResourceReader& aReader )
    {
    CKefResource* self = new (ELeave) CKefResource;
    CleanupStack::PushL( self );
    self->ConstructL( aReader );
    return self;
    }

//------------------------------------------------------------------------------
// CKefResource::~CKefResource
//------------------------------------------------------------------------------
//
CKefResource::~CKefResource()
    {
    delete iScanCodes;
    delete iKeyCodes;
    delete iDeviceModes;
    delete iEventTypes;
    }

//------------------------------------------------------------------------------
// CKefResource::ConstructL
//------------------------------------------------------------------------------
//
void CKefResource::ConstructL( TResourceReader& aReader )
    {
    iScanCodes = new (ELeave) CArrayFixFlat<TUint16>( KKefMaxComboKeys );
    iKeyCodes = new (ELeave) CArrayFixFlat<TUint16>( KKefMaxDeviceMode );
    iDeviceModes = new (ELeave) CArrayFixFlat<TUint16>( KKefMaxDeviceMode );
    iEventTypes = new (ELeave) CArrayFixFlat<TUint8>( KKefMaxDeviceMode );
    AddScanCodesL( aReader );
    AddKeyCodesL( aReader );
    AddDeviceModesL( aReader );
    AddEventTypesL( aReader );

#ifdef RD_TACTILE_FEEDBACK
    AddFeedbackTypeL( aReader );
#endif // RD_TACTILE_FEEDBACK
    }

//------------------------------------------------------------------------------
// CKefResource::AddScanCodesL
//------------------------------------------------------------------------------
//
void CKefResource::AddScanCodesL( TResourceReader& aReader )
    {
    TUint n;
    n = aReader.ReadUint8();
    if ( n > KKefMaxComboKeys )
        {
        User::Leave( KErrGeneral );
        }
        
    for ( TUint i = 0; i < n; i++ ) 
        {
        TUint16 tryCode = aReader.ReadUint16();
        if ( tryCode < KKefMaxScanCode ) // tryCode >= 0 (because TUint16)
            {
            iScanCodes->AppendL( tryCode );
            __KEFLOGSTRING2("CKefResource::AddScanCodesL: iScanCodes[%d] = %d",i,iScanCodes->At(i));
            }
        else
            {
            __KEFLOGSTRING2("CKefResource::AddScanCodesL: Erroneus scancode[%d] = %d",i,tryCode);
            }
        }
    }

//------------------------------------------------------------------------------
// CKefResource::AddKeyCodesL
//------------------------------------------------------------------------------
//
void CKefResource::AddKeyCodesL( TResourceReader& aReader )
    {
    TUint n;
    n = aReader.ReadUint8();
    for ( TUint i = 0; i < n; i++ )
        {
        TUint16 tryCode = aReader.ReadUint16();
        if ( tryCode < 0xFFFF ) // tryCode >= 0 (because TUint16)
            {
            iKeyCodes->AppendL( tryCode );
            __KEFLOGSTRING2( "CKefResource::AddKeyCodesL: iKeyCodes[%d] = %d",i,iKeyCodes->At(i));
            }
        else 
            {
            __KEFLOGSTRING2("CKefResource::AddKeyCodesL: Erroneus keycode[%d] = %d",i,tryCode);
            }
            
        }
    }

//------------------------------------------------------------------------------
// CKefResource::AddDeviceModesL
//------------------------------------------------------------------------------
//
void CKefResource::AddDeviceModesL( TResourceReader& aReader )
    {
    TUint n;
    n = aReader.ReadUint8();
    for ( TUint i = 0; i < n; i++ )
        {
        TUint16 tryMode = aReader.ReadUint16();
        if ( tryMode < KKefKeyMapRows ) // modeWithoutTypes >= 0
            {            
            iDeviceModes->AppendL( tryMode );
            __KEFLOGSTRING2("CKefResource::AddDeviceModesL: iDeviceModes[%d] = %d",i,iDeviceModes->At(i));
            }
        else 
            {
            __KEFLOGSTRING("CKefResource::AddDeviceModesL: Invalid mode");
            __KEFLOGSTRING1("CKefResource::AddDeviceModesL: rows=%d", KKefKeyMapRows );
            }
        }
    }

//------------------------------------------------------------------------------
// CKefResource::AddEventTypesL
//------------------------------------------------------------------------------
//
void CKefResource::AddEventTypesL( TResourceReader& aReader )
    {
    TUint n;
    n = aReader.ReadUint8();
    for ( TUint i = 0; i < n; i++ )
        {
        TUint8 tryType = aReader.ReadUint8();
        if ( tryType < 256 ) 
            {            
            iEventTypes->AppendL(tryType);
            __KEFLOGSTRING2("CKefResource::AddEventTypesL: iEventTypes[%d] = %d",i,iEventTypes->At(i));
            }
        else 
            {
            }            
        }
    }


//------------------------------------------------------------------------------
// CKefResource::AddFeedbackTypeL
//------------------------------------------------------------------------------
//

void CKefResource::AddFeedbackTypeL( TResourceReader& aReader )
    {
    #ifdef RD_TACTILE_FEEDBACK
    iFeedbackType = aReader.ReadUint16();
    #endif // RD_TACTILE_FEEDBACK
    }


//------------------------------------------------------------------------------
// CKefResource::GetScanCodes  
//------------------------------------------------------------------------------
//
CArrayFixFlat<TUint16>& CKefResource::GetScanCodes()
    {
    return *iScanCodes;
    }

//------------------------------------------------------------------------------
// CKefResource::GetKeyCodes
//------------------------------------------------------------------------------
//
CArrayFixFlat<TUint16>& CKefResource::GetKeyCodes()
    {
    return *iKeyCodes;
    }

//------------------------------------------------------------------------------
// CKefResource::GetDeviceModes
//------------------------------------------------------------------------------
//
CArrayFixFlat<TUint16>& CKefResource::GetDeviceModes()
    {
    return *iDeviceModes;
    }

//------------------------------------------------------------------------------
// CKefResource::GetEventTypes
//------------------------------------------------------------------------------
//
CArrayFixFlat<TUint8>& CKefResource::GetEventTypes()
    {
    return *iEventTypes;
    }

//------------------------------------------------------------------------------
// CKefResource::GetFeedbackType
//------------------------------------------------------------------------------
//

TUint16 CKefResource::GetFeedbackType() const
    {
    #ifdef RD_TACTILE_FEEDBACK
    return iFeedbackType;
    #else //Not RD_TACTILE_FEEDBACK
    return 0;
    #endif // RD_TACTILE_FEEDBACK
    }


//------------------------------------------------------------------------------
// CKefResourceArray::NewLC
//------------------------------------------------------------------------------
//
CKefResourceArray* CKefResourceArray::NewLC()
    {
    CKefResourceArray* self = new (ELeave) CKefResourceArray;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
//------------------------------------------------------------------------------
// CKefResourceArray::~CKefResourceArray
//------------------------------------------------------------------------------
//
CKefResourceArray::~CKefResourceArray()
    {
    if ( iKeyEvents )
        {
        iKeyEvents->ResetAndDestroy();
        delete iKeyEvents;
        }
    }
    
//------------------------------------------------------------------------------
// CKefResourceArray::ConstructL
// Symbian 2nd phase constructor
//------------------------------------------------------------------------------
//
void CKefResourceArray::ConstructL()
    {
    iKeyEvents = new (ELeave) CArrayPtrFlat<CKefResource>( 
        KKefKeyEventArrayGranularity );
    }

//------------------------------------------------------------------------------
// CKefResourceArray::GetKeyEventsL
//------------------------------------------------------------------------------
//
CArrayPtrFlat<CKefResource>* CKefResourceArray::GetKeyEventsL(
        TResourceReader& aReader )
    {
    TUint number;
    number = aReader.ReadUint16();
    iKeyEvents->Reset();
    for ( TUint i = 0; i < number; i++ )
        {
        __KEFLOGSTRING1("-----< KEY_EVENT %d >-----",i);
        CKefResource* resKeyEvent = CKefResource::NewLC( aReader );
        iKeyEvents->AppendL( resKeyEvent );
        CleanupStack::Pop();
        }
    return iKeyEvents;
    }

// End of File
