/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SensorUtil class implementation.
 *
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "sensorutil.h"
#include "sensortrace.h"

const TUint KLimit = 0xFFFFFFFF;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
SensorUtil::SensorUtil( )
    {
    // Nothing to do.
    }

//-----------------------------------------------------------------------------
// SensorUtil::ConvertToHex
//-----------------------------------------------------------------------------
//
TInt SensorUtil::ConvertToHex( const TDesC& aString, TUint32& aValue )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    TLex lex( aString );
    
    // Get rid of '0x'
    lex.Inc( 2 );
    if( lex.BoundedVal( aValue, EHex, KLimit ) != KErrNone )
        {
        err = KErrNotFound;
        }
    
    return err;
    }

// End of file
