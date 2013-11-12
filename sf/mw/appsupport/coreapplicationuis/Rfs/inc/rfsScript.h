/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*  Script class that parses the RFS commands from a script descriptor.
*
*
*/


#ifndef RFS_SCRIPT_H
#define RFS_SCRIPT_H

//  INCLUDES

#include <e32std.h>
#include <e32base.h>
#include <s32file.h>

// CONSTANTS

// Needed actions if adding commands:
//
// In CRfsScript::ParseNextCommand():
// - add the new command literals in the command array
// - add the allowed number of parameters for the new commands
//   in the code where the number of parameters is checked
//
// In CRfsCommand::ExecuteL():
// - add the code for the new command
//
// Also update:
// - TRfsCommandId
// - KRfsNumberOfCommands

const TInt KRfsNumberOfCommands = 8;

_LIT( KRfsCommand1, "DEL" );
_LIT( KRfsCommand2, "RD" );
_LIT( KRfsCommand3, "CP" );
_LIT( KRfsCommand4, "CR" );
_LIT( KRfsCommand5, "PLUGINS" );
_LIT( KRfsCommand6, "FORMAT" );
_LIT( KRfsCommand7, "MD" );
_LIT( KRfsCommand8, "CD" );

enum TRfsCommandId
    {
    ERfsCommandUnknown  = -1,
    ERfsCommandDel      = 0,
    ERfsCommandRd       = 1,
    ERfsCommandCp       = 2,
    ERfsCommandCr       = 3,
    ERfsCommandPlugins  = 4,
    ERfsCommandFormat   = 5,
    ERfsCommandMd       = 6,
    ERfsCommandCd       = 7
    };

const TInt KRfsEndOfScript = -100; // return code when end of script detected

// FORWARD DECLARATIONS

class CRfsCommand;
class CRfsServer;
class CRfsSession;

// CLASS DECLARATION

/**
*  Script class that parses the rfs commands from a script descriptor.
*/
class CRfsScript : public CBase
    { 

    public:  // Constructors and destructor

        /**
        * Creates a new script instance.
        */
        static CRfsScript* NewLC( const TDesC& aScript );

        /**
        * destructor
        */
        virtual ~CRfsScript();

    public: // New functions

        /**
        * Parses the whole script and returns an error code if there is a
        * syntax error somewhere in script.
        *
        * @return errorcode, KErrNone if correct script
        */
        TInt CheckScriptSyntaxL();

        /**
        * Executes the next command in script.
        *
        * @param aServer pointer to RFS server
        * @param aSession pointer to the server session 
        *
        * @return KRfsEndOfScript if there were no more commands in script
        *         Symbian OS error code if an error occurred in command execution
        */
        TInt ExecuteNextCommandL( CRfsServer* aServer, CRfsSession* aSession );


        /**
        * Tells if a DOS command has been executed, thus an RFS request
        * has been sent to DOS side.
        *
        * @result ETrue if DOS RFS has been activated, EFalse otherwise.
        */
        inline TBool DosCallSent();

    protected:

        /**
        * C++ default constructor.
        */      
        CRfsScript( const TText* aScript, TInt aLength );

    private:    // new functions

        /**
        * Parses the next command in script. If the command was unidentified or
        * the number of parameters wrong, returns errorcode.
        * If the aCommand parameter is not NULL, stores the command information
        * in the command instance it points to.
        *
        * @param aCommand command instance to store the command information to
        *                 If NULL, only syntax is checked.
        *
        * @return KErrGeneral if the command syntax was wrong in some way.
        *         KErrNone if the command was OK.
        */
        TInt ParseNextCommandL( CRfsCommand* aCommand );

        /**
        * Increases given index until it doesn't point to white space in the
        * script text. Increasing is stopped when the index goes beyond the
        * script end. Index can point 1 character beyond the script end after
        * this.
        *
        * @param aIndex index pointing somewhere in the script text.
        * @param aQuotes if not NULL, this will be set ETrue, if the first
        *                character after the skipped space is a quotation mark.
        */
        void SkipSpace( TInt& aIndex, TBool* aQuotes = NULL );

        /**
        * Increases given index until it points to white space or line end
        * (aQuotes = EFalse) or a quotation mark (aQuotes = ETrue) in the
        * script. Increasing is stopped when the index goes beyond the script
        * end. Index can point 1 character beyond the script end after this.
        *
        * @param aIndex index pointing somewhere in the script text.
        * @param aQuotes flag for finding an ending quotation mark instead of
        *                white space or line end
        *
        * @return EFalse if aQuotes was ETrue and the end of script occurred
        *         before a quotation mark was found.
        *
        *         ETrue otherwise.
        */
        TBool FindEnd( TInt& aIndex, TBool aQuotes = EFalse );

        /**
        * First increases given index until it points to line end in the script.
        * Then increases it until it doesn't point to line end, resulting in
        * pointing to the start of the next non-empty line. Increasing is
        * stopped when the index goes beyond the script end. Index can point 1
        * character beyond the script end after this.
        *
        * @param aIndex index pointing somewhere in the script text.
        *
        * @return Index pointing to the start of the next non-empty line.
        */
        TInt NextLine( TInt aIndex );

        /**
        * Checks if the given index points to a comment start in the script.
        *
        * @return ETrue if comment, EFalse if not
        */
        TBool IsComment( TInt aStartIndex );

        /**
        * Checks if the given number of parameters is legal for the given
        * command ID.
        *
        * @param aId command ID
        * @param aNumberOfParams number of parameters
        *
        * @result ETrue if number of parameters is OK, EFalse otherwise.
        */
        TBool VerifyNumberOfParameters( TRfsCommandId aId,
                                        TInt aNumberOfParams );

    private:    // data

        const TText* iScript;   // not owned
        TInt iLength;
        TInt iIndex;
	    TBool iDos;
    };

    #include "rfsScript.inl"

#endif      // RFS_SCRIPT_H
