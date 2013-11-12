/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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


// SYSTEM INCLUDES
#include <e32property.h>
#include <data_caging_path_literals.hrh>
#include <driveinfo.h>
#include <outofdiskmonitor.rsg>
#include <bautils.h>                // BaflUtils
#include <StringLoader.h>


// USER INCLUDES
#include "UiklafInternalCRKeys.h"
#include "OodTraces.h"
#include "outofdiskglobalnote.h"
#include "outofdiskmonitor.h"

// CONSTANTS
_LIT(KOODWatcherResourceFileName, "outofdiskmonitor.rsc");
// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskGlobalNote* COutOfDiskGlobalNote::NewL
    (  COutOfDiskMonitor* aOutOfDiskMonitor, RFs& aFs )
    {
    TRACES("COutOfDiskGlobalNote::NewL");
    COutOfDiskGlobalNote* self = new (ELeave) COutOfDiskGlobalNote( aOutOfDiskMonitor, aFs );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACES("COutOfDiskGlobalNote::NewL: End");
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskGlobalNote::~COutOfDiskGlobalNote()
    {
    TRACES("COutOfDiskGlobalNote::~COutOfDiskGlobalNote");
    iOODResourceFile.Close();
    delete iNote;
    iNote=NULL;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfDiskGlobalNote::COutOfDiskGlobalNote( COutOfDiskMonitor* aOutOfDiskMonitor, RFs& aFs ) :
    iOutOfDiskMonitor( aOutOfDiskMonitor ),
    iFs( aFs )
    {
    TRACES("COutOfDiskGlobalNote::COutOfDiskGlobalNote");
    TRACES("COutOfDiskGlobalNote::COutOfDiskGlobalNote: End");    
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskGlobalNote::ConstructL()
    {
    TRACES("COutOfDiskGlobalNote::ConstructL");
    iNoteInfo.iNoteId = KErrNotFound;
    iNoteInfo.iStatus = DISK_SPACE_OK;
    iNoteInfo.iDrive = KErrNotFound;
    iNote= CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EWarning);
    TRACES("COutOfDiskGlobalNote::ConstructL: Open OOD resource file");
    TFileName OODFileName;
    OODFileName.Append(_L("Z"));
    OODFileName.Append(_L(":"));    
    OODFileName.Append(KDC_RESOURCE_FILES_DIR);
    OODFileName.Append(KOODWatcherResourceFileName);
    BaflUtils::NearestLanguageFile(iFs, OODFileName);
    iOODResourceFile.OpenL(iFs, OODFileName);
    TRACES1("COutOfDiskGlobalNote::ConstructL: Opened %S",&OODFileName);    
    TRACES("COutOfDiskGlobalNote::ConstructL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfDiskGlobalNote::DisplayL(const TDesC& aMessage)
    {
    TRACES("COutOfDiskGlobalNote::DisplayL");
       
     TRACES("COutOfDiskGlobalNote::COutOfDiskGlobalNote::DisplayL: set text and observer for the note");        
     iNote->SetTextL(aMessage);
     iNote->SetObserver(this);
     iNote->SetTimeout(0);
     iNote->ShowL();
          
    TRACES("COutOfDiskGlobalNote::DisplayL: End");
    }

// ---------------------------------------------------------
// Use a global query to display the message but make sure that eiksrv is
// there (state must not be initialising (AVKON + UIKON)
// ---------------------------------------------------------
//
void COutOfDiskGlobalNote::ShowGlobalQueryL(TInt aStatus, TInt aDrive)
    {
    TRACES("COutOfDiskGlobalNote::ShowGlobalQueryL");

    if (NoteOnDisplay())
        {
        TRACES("COutOfDiskGlobalNote::ShowGlobalQueryL: Note already on display");
        return;    
        }

    if (iOutOfDiskMonitor->GetGlobalNotesAllowed())
        {
    
        TResourceReader resReader;
        HBufC8* str(NULL);
        CDesCArray* strings = new ( ELeave ) CDesCArrayFlat( 2 );
        CleanupStack::PushL( strings );

        TDriveName driveName;
        TChar driveChar;
        User::LeaveIfError( iFs.DriveToChar( aDrive, driveChar ) );
        driveName.Append(driveChar);
        driveName.Append(_L(":"));        
        
        TVolumeInfo volInfo;
        iFs.Volume( volInfo, aDrive );
        TInt nameLength(volInfo.iName.Length());        

        TUint driveStatus(0);
        DriveInfo::GetDriveStatus( iFs, aDrive, driveStatus );

        if (aStatus==DISK_SPACE_WARNING)
            {
            TRACES1("COutOfDiskGlobalNote::ShowGlobalQueryL: Warning note! Drive: %c", aDrive+'A');
            
            if (aDrive == iOutOfDiskMonitor->GetDefaultPhoneMemory())
                {
                str = iOODResourceFile.AllocReadLC(R_QTN_MEMLO_DEVICE_MEMORY_LOW);
                resReader.SetBuffer(str);    
                strings->AppendL( driveName );
                }
            else if (driveStatus & DriveInfo::EDriveRemovable)
                {
                TRACES1("COutOfDiskGlobalNote::ShowGlobalQueryL: Warning note! volNameLength: %d", nameLength);
                TBufC<KMaxFileName> name(volInfo.iName);
                if (nameLength)
                    {
        	        str = iOODResourceFile.AllocReadLC(R_QTN_MEMLO_MEMORY_CARD_LOW_NAME);
    	            resReader.SetBuffer(str);    
		            strings->AppendL( driveName );
		            strings->AppendL( name );
                    }
                else
                    {
        	        str = iOODResourceFile.AllocReadLC
        	              (R_QTN_MEMLO_MEMORY_CARD_LOW_DEFAULTNAME);
    	            resReader.SetBuffer(str);    
		            strings->AppendL( driveName );
                    }
                }
            else
                {
                str = iOODResourceFile.AllocReadLC(R_QTN_MEMLO_MASS_STORAGE_MEMORY_LOW);
                resReader.SetBuffer(str);    
                strings->AppendL( driveName );                
                }
            }
        else if (aStatus==DISK_SPACE_CRITICAL)
            {
            TRACES1("COutOfDiskGlobalNote::ShowGlobalQueryL: Critical note! Drive: %c", aDrive+'A');
            if (aDrive == iOutOfDiskMonitor->GetDefaultPhoneMemory())
                {
                str = iOODResourceFile.AllocReadLC(R_QTN_MEMLO_DEVICE_MEMORY_FULL);
                resReader.SetBuffer(str);    
                strings->AppendL( driveName );                
                }                
            else if (driveStatus & DriveInfo::EDriveRemovable)
                {
                TBufC<KMaxFileName> name(volInfo.iName);
                if (nameLength)
                    {
        	        str = iOODResourceFile.AllocReadLC(R_QTN_MEMLO_MEMORY_CARD_FULL_NAME);
    	            resReader.SetBuffer(str);    
		            strings->AppendL( driveName );
		            strings->AppendL( name );
                    }
                else
                    {
        	        str = iOODResourceFile.AllocReadLC
        	              (R_QTN_MEMLO_MEMORY_CARD_FULL_DEFAULTNAME);
    	            resReader.SetBuffer(str);    
		            strings->AppendL( driveName );
                    }                    
                }
            else
                {
                str = iOODResourceFile.AllocReadLC(R_QTN_MEMLO_MASS_STORAGE_FULL);
                resReader.SetBuffer(str);
                strings->AppendL( driveName );                
                }
            }
        resReader.SetBuffer(str);
	      HBufC* resHandle = resReader.ReadHBufCL();
        CleanupStack::PushL( resHandle );
        HBufC* message(FormatStringL(resHandle->Des(),*strings));
        CleanupStack::PushL( message );
	      TRACES1("COutOfDiskMonitor::ShowGlobalQueryL: txt: %S",message);
        DisplayL(message->Des());

        iNoteInfo.iStatus = aStatus;
        iNoteInfo.iDrive = aDrive;        
        CleanupStack::PopAndDestroy(message);
        CleanupStack::PopAndDestroy(resHandle);
        CleanupStack::PopAndDestroy( str );
        CleanupStack::PopAndDestroy( strings ); 
        iOutOfDiskMonitor->SetAsDisplayedL(aDrive, aStatus);
        }
    TRACES("COutOfDiskGlobalNote::ShowGlobalQueryL: End");
    }

// ---------------------------------------------------------------------------
// COutOfDiskGlobalNote::FormatStringL( TDesC& aSource,
//                             const MDesCArray& aStrings, 
//                             TInt aParamNumber )
// ---------------------------------------------------------------------------
//
HBufC* COutOfDiskGlobalNote::FormatStringL(
    const TDesC& aSource,
    const MDesCArray& aStrings)
    {
    TRACES1("COutOfDiskGlobalNote::FormatStringL: input: %S",&aSource);

    TInt count( aStrings.MdcaCount() ); //Number of input strings.
    TRACES1("COutOfDiskGlobalNote::FormatStringL: Number of parameters: %d",count);

    TInt length = aSource.Length();
    for(TInt i=0; i < count; i++)
        {
        length += aStrings.MdcaPoint( i ).Length();
        }

    HBufC* retbuf = HBufC::NewLC( length );
    TPtr retptr( retbuf->Des() );
    
    if(count > 1)
        {
        HBufC* temp = HBufC::NewLC( length );
        *temp = aSource;            
        for(TInt i=0; i < count; i++)
            {
            StringLoader::Format( retptr, *temp, i, aStrings.MdcaPoint( i ));
            *temp = *retbuf;
            }
        CleanupStack::PopAndDestroy( temp );
        }
    else
        {
        StringLoader::Format( retptr, aSource, -1, aStrings.MdcaPoint( 0 ));
        }
    CleanupStack::Pop( retbuf );

    TRACES("COutOfDiskGlobalNote::FormatStringL: End");
    return retbuf;
    }

// -----------------------------------------------------------------------------
// COutOfDiskGlobalNote::MessageBoxClosed
// -----------------------------------------------------------------------------
//
void COutOfDiskGlobalNote::MessageBoxClosed(const CHbDeviceMessageBoxSymbian* aMessageBox,
                CHbDeviceMessageBoxSymbian::TButtonId aButton)
    {
    TRACES("COutOfDiskGlobalNote::closed");
    iNoteInfo.iNoteId = KErrNotFound;
    iNoteInfo.iStatus = DISK_SPACE_OK;
    iNoteInfo.iDrive = KErrNotFound;

    TInt drive = iOutOfDiskMonitor->GetTopPriorityDrive();
    if (drive != KErrNotFound)        
        {
        ShowGlobalQueryL(iOutOfDiskMonitor->GetDriveStatus(drive), drive);
        }

    TRACES("COutOfDiskGlobalNote::RunL: End");    
    }


// -----------------------------------------------------------------------------
// COutOfDiskGlobalNote::NoteOnDisplay
// -----------------------------------------------------------------------------
//
TBool COutOfDiskGlobalNote::NoteOnDisplay()
    {
    TRACES("COutOfDiskGlobalNote::NoteOnDisplay");    
    TBool note( EFalse);
    if (iNoteInfo.iNoteId > KErrNotFound)
        {
        note = ETrue;
        }
    TRACES1("COutOfDiskGlobalNote::NoteOnDisplay: End: return %d",note);    
    return note;
    }

// -----------------------------------------------------------------------------
// COutOfDiskGlobalNote::GetNoteInfo
// -----------------------------------------------------------------------------
//
TNoteInfo COutOfDiskGlobalNote::GetNoteInfo()
    {
    TRACES("COutOfDiskGlobalNote::GetNoteInfo");    
    return iNoteInfo;
    }
