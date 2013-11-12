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
 * Description:  ?Description
 *
 */

#include <hbtextresolversymbian.h>
#include <f32file.h>

#include "cautils.h"
#include "cadef.h"

_LIT(KHexPrefix, "0x");

// ---------------------------------------------------------
// MenuUtils::GetTUint
// ---------------------------------------------------------
//
EXPORT_C TInt MenuUtils::GetTUint( const TDesC& aStr, TUint& aInt )
    {
    TInt position( aStr.Find( KHexPrefix ) );
    TPtrC string( aStr );
    TRadix radix( EDecimal );
    if( position == 0 )
        {
        // is hex
        radix = EHex;
        string.Set( aStr.Mid( KHexPrefix().Length() ) );
        }

    return TLex( string ).Val( aInt, radix );
    }

// ---------------------------------------------------------
// MenuUtils::InitTextResolverSymbian
// ---------------------------------------------------------
//
EXPORT_C void MenuUtils::InitTextResolverSymbianL( const TDesC& aFilename )
    {
    if( !HbTextResolverSymbian::Init( aFilename, KLocalizationFilepathC ) )
       {
       if( !HbTextResolverSymbian::Init( aFilename, KLocalizationFilepathZ ) )
           {
           // this should not be called too often 
           TChar currentDriveLetter;
           TDriveList driveList;
           RFs fs;
           CleanupClosePushL( fs );
           User::LeaveIfError( fs.Connect() );
           User::LeaveIfError( fs.DriveList( driveList ) );

           RBuf path;
           CleanupClosePushL( path );
           path.CreateL( KLocalizationFilepath().Length() + 1 );
           
           for( TInt driveNr=EDriveY; driveNr >= EDriveA; driveNr-- )
               {
               if( driveList[driveNr] )
                   {
                   User::LeaveIfError( fs.DriveToChar( driveNr,
                           currentDriveLetter ) );
                   path.Append( currentDriveLetter );
                   path.Append( KLocalizationFilepath );
                   if( HbTextResolverSymbian::Init( aFilename, path ) )
                       {
                       break;
                       }
                   }
               path.Zero();
               }
           CleanupStack::PopAndDestroy( &path );
           CleanupStack::PopAndDestroy( &fs );
           }
        }
    }

