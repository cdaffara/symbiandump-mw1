/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*		Implements CRFsScript class which controls the parsing of the Rfs scripts
*
*/


// INCLUDE FILES

#include "rfsScript.h"
#include "rfsCommand.h"
#include "rfsSession.h"
#include "rfsCompleter.h"
#include "rfsServer.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS ==========================================

CRfsScript::CRfsScript( const TText* aScript, TInt aLength ) :
    iScript( aScript ),
    iLength( aLength )
    {}

CRfsScript::~CRfsScript()
    {}

CRfsScript* CRfsScript::NewLC( const TDesC& aScript )
    {
    CRfsScript* script = new( ELeave )
    CRfsScript( aScript.Ptr(), aScript.Length() );
    CleanupStack::PushL( script );
    return script;
    }

// -----------------------------------------------------------------------------
// CRfsScript::CheckScriptSyntaxL()
// -----------------------------------------------------------------------------

TInt CRfsScript::CheckScriptSyntaxL()
    {
    TInt ret;

    // iIndex needs to be restored after the script syntax has been checked

    TInt index( iIndex );

    while ( ( ret = ParseNextCommandL( NULL ) ) == KErrNone ){}

    iIndex = index;
    return ret == KRfsEndOfScript ? KErrNone : ret;
    }

// -----------------------------------------------------------------------------
// CRfsScript::ExecuteNextCommandL()
// -----------------------------------------------------------------------------

TInt CRfsScript::ExecuteNextCommandL( CRfsServer* aServer, 
                                      CRfsSession* aSession )
    {
    CRfsCommand* command = CRfsCommand::NewLC( aServer, aSession );

    TInt ret = ParseNextCommandL( command );
    
    if ( ret == KErrNone )
        {
        ret = command->ExecuteL();
        }
    
    CleanupStack::PopAndDestroy();  // command
    
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsScript::ParseNextCommandL()
// -----------------------------------------------------------------------------

TInt CRfsScript::ParseNextCommandL( CRfsCommand* aCommand )
    {
    const TDesC* commands[ KRfsNumberOfCommands ] =
        {    
        &KRfsCommand1,
        &KRfsCommand2,
        &KRfsCommand3,
        &KRfsCommand4,
        &KRfsCommand5,
        &KRfsCommand6,
        &KRfsCommand7,
        &KRfsCommand8
        };
    
    TInt startIndex;
    TInt endIndex;

    FOREVER
        {

        // check if index is beyond the descriptor already

        if ( iIndex >= iLength )
            {
            return KRfsEndOfScript;
            }

        // go to the beginning of the "command word"

        startIndex = iIndex;
        SkipSpace( startIndex );

        // go to the end of the "command word"

        endIndex = startIndex;
        FindEnd( endIndex );

        if ( endIndex - startIndex > 0 && !IsComment( startIndex ) )
            {
            break;
            }

        // if the line is commented out, skip it

        iIndex = NextLine( startIndex );
        }

    // create TPtrC to the "command word"

    TPtrC command( &iScript[ startIndex ], endIndex - startIndex );
    
    TRfsCommandId commandId( ERfsCommandUnknown );

    // determine the command id

    for ( TInt i = 0; i < KRfsNumberOfCommands ; i++ )
        {
        if ( !command.CompareF( *( commands[ i ] ) ) )
            {
            commandId = (TRfsCommandId)i;
            break;
            }
        }

    TInt ret( KErrGeneral ); // returns this if there is a syntax error in line

    if ( commandId != ERfsCommandUnknown )
        {
        TInt numberOfParams( 0 );        
        TInt start = endIndex;

        FOREVER
            {
            TBool quotes( EFalse );
            SkipSpace( start, &quotes );

            TInt end = start;
            if ( FindEnd( end, quotes ) )
                {
                if ( end - start > 0 && !IsComment( start) )
                    {
                    numberOfParams++;
                    start = end;
                    continue;
                    }
                }
            break;
            }

        if ( VerifyNumberOfParameters( commandId, numberOfParams ) )
            {
            // set command information if the command parameter has been given

            if ( aCommand )
                {
                TPtrC* params = new( ELeave ) TPtrC[ numberOfParams ];
                
                // fetch parameter information

                for ( TInt i = 0; i < numberOfParams ; i++ )
                    {
                    TInt paramStart = endIndex;
                    TBool quotes( EFalse );

                    SkipSpace( paramStart, &quotes );

                    TInt paramEnd = paramStart;
                    FindEnd( paramEnd, quotes );

                    TInt realStart( paramStart );
                    TInt realEnd( paramEnd );

                    if ( quotes )
                        {
                        realStart++;
                        realEnd--;
                        }

                    params[ i ].Set( &iScript[ realStart ], realEnd - realStart );

                    endIndex = paramEnd;
                    }

                // aCommand gets ownership of paramStarts and paramLengths

                aCommand->Set( commandId, numberOfParams, params );
                }

            iIndex = NextLine ( endIndex );
            ret = KErrNone;
            }
        }
    
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsScript::SkipSpace()
// -----------------------------------------------------------------------------

void CRfsScript::SkipSpace( TInt& aIndex, TBool* aQuotes )
    {
    while ( aIndex < iLength )
        {
        TText t = iScript[ aIndex ];

        if ( aQuotes && t == '"' )
            {
            *aQuotes = ETrue;
            }
        
        if ( t != EKeySpace && t != EKeyTab )
            {
            break;
            }
        aIndex++;
        }
    }

// -----------------------------------------------------------------------------
// CRfsScript::FindEnd()
// -----------------------------------------------------------------------------

TBool CRfsScript::FindEnd( TInt& aIndex, TBool aQuotes )
    {
    TBool quoteFound( EFalse );

    if ( aQuotes )
        {
        aIndex++;
        }

    while ( aIndex < iLength )
        {
        TText t = iScript[ aIndex ];
    
        if ( t == EKeyLineFeed || t == EKeyEnter )
            {
            break;
            }

        if ( aQuotes )
            {
            if ( t == '"' )
                {
                quoteFound = ETrue;
                aIndex++;
                break;
                }
            }

        else if ( t == EKeySpace || t == EKeyTab )
            {
            break;
            }

        aIndex++;
        }

    // If the trailing quotation mark was not found, return EFalse,
    // otherwise return ETrue.

    return ( !aQuotes || quoteFound );
    }

// -----------------------------------------------------------------------------
// CRfsScript::NextLine()
// -----------------------------------------------------------------------------

TInt CRfsScript::NextLine( TInt aIndex )
    {
    // first find the line end

    while ( aIndex < iLength )
        {
        TText t = iScript[ aIndex ];
        if ( t == EKeyLineFeed || t == EKeyEnter )
            {
            break;
            }
        aIndex++;
        }

    // then find the next line start

    while ( aIndex < iLength )
        {
        TText t = iScript[ aIndex ];
        if ( t != EKeyLineFeed && t != EKeyEnter )
            {
            break;
            }
        aIndex++;
        }    

    return aIndex;
    }

// -----------------------------------------------------------------------------
// CRfsScript::IsComment()
// -----------------------------------------------------------------------------

TBool CRfsScript::IsComment( TInt aStartIndex )
    {
    // comments start with "#" or "//"

    return ( aStartIndex < iLength && iScript[ aStartIndex ] == '#' ) ||
           ( aStartIndex < iLength - 1 && 
           iScript[ aStartIndex ] == '/' && iScript[ aStartIndex + 1 ] == '/' );
    }

// -----------------------------------------------------------------------------
// CRfsScript::VerifyNumberOfParameters()
// -----------------------------------------------------------------------------

TBool CRfsScript::VerifyNumberOfParameters( TRfsCommandId aId,
                                            TInt aNumberOfParams )
    {
    TInt minNumberOfParams( 1 );    // defaults to 1 parameter
    TInt maxNumberOfParams( 1 );
        
    switch ( aId )
        {
        case ERfsCommandCr:
            maxNumberOfParams = KMaxTInt;   // this has >= 1 parameters
            break;

        case ERfsCommandCp:
            maxNumberOfParams = 2;          // this has 1-2 parameters
            break;

        case ERfsCommandPlugins:
            minNumberOfParams = 0;
            maxNumberOfParams = KMaxTInt;   // this has >= 0 parameters
            break;

        default:                            // others got 1 parameter
            break;
        };

        // check the number of parameters

    return ( aNumberOfParams >= minNumberOfParams &&
             aNumberOfParams <= maxNumberOfParams );
    }

// end of file
