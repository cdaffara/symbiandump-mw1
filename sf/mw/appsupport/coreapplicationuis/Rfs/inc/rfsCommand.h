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
*  Command class that executes RFS commands. This is where the
*  actual functionality related to RFS is.
*  
*
*/


#ifndef RFS_COMMAND_H
#define RFS_COMMAND_H

//  INCLUDES

#include "rfsScript.h"
#include <e32base.h>
#include "rfsPlugin.h"

// FORWARD DECLARATIONS

class CApDataHandler;
class CApAccessPointItem;
class CCommsDatabase;
class CRfsApArray;

struct SRfsApMapping
    {
    TUint32 iOldUid;
    TUint32 iNewUid;
    };

/**
* Plugin subtypes
*/
_LIT( KPluginFormat, "F" );
_LIT( KPluginPostUI, "P" );

/**
* Script command for running the plug-ins
*/
_LIT( KPluginsCommand, "PLUGINS" );

// CLASS DECLARATION

/**
*  Command class that executes RFS commands.
*/

class CRfsCommand : public CBase
    { 
    public:  // Constructors and destructor

        /**
        * two-phase constructor.
        */      
        static CRfsCommand* NewLC( CRfsServer* aServer, CRfsSession* aSession );

        /**
        * destructor
        */
        virtual ~CRfsCommand();

    public:     // new functions

        /**
        * Sets all information the command needs to be executed.
        *
        * @param aCommandId command ID
        * @param aNumberOfParams number of parameters
        * @param aParams array containing the parameter descriptors
        */
        void Set( TRfsCommandId aCommandId, TInt aNumberOfParams,
                  TPtrC* aParams );

        /**
        * Executes the RFS command.
        */
        TInt ExecuteL();

        /**
        * Gives the command ID
        */
        TRfsCommandId CommandId();

    protected:  // constructors

		/**
        * C++ default constructor.
        */
        CRfsCommand( CRfsServer* aServer, CRfsSession* aSession );

    private:    // new functions

        /**
        * Converts string to number
        */
        void StringToNumberL( const TDesC& aString, TUint& aNumber,
                              TRadix aRadix = EDecimal );

        void StringToNumberL( const TDesC& aString, TUid& aNumber,
                              TRadix aRadix = EHex );
                              
        void StringToNumberL( const TDesC& aString, TUint32& aNumber,
                              TRadix aRadix = EHex );                              

        /**
        * Executes CP command
        * @return TInt (error code)
        */
        TInt ExecuteCpCommandL();

        /**
        * Executes CR command
        * @return TInt (error code)
        */
        TInt ExecuteCrCommandL();

        /**
        * Executes FORMAT command
        * @return TInt (error code)
        */
        TInt ExecuteFormatCommand();

        /**
        * Executes CD command
        * @return TInt (error code)
        */
        TInt ExecuteCdCommandL();

        /**
        * Executes PLUGINS command
        * @return TInt (error code)
        */
        TInt ExecutePluginsCommandL();

		TBool IsCenRepKey(TDes& aPath);
		
		TInt NumberOfRepositories(TDes& aScript);
		
        /**
        * Fetches target file
        */
        void FetchTargetFileL( const TDesC& aFilename );

		void GetKeyL( TUid aUid, TUint32 aKey, TDes& aBuf );

        /**
        * Removes read only flag from target file
        * @return TInt (error code)
        */
        TInt RemoveReadOnlyFlagFromTarget( const TDesC& aSource,
                                           TDes& aTarget );

        /**
        * Handles plugin script
        * @return TInt (error code)
        */
        TInt HandlePluginScriptL( TDes& aPath, CRFSPlugin* aPlugin, const TRfsReason aType );

        /**
        * Gives length of current command
        * @return TInt
        */
		TInt GetCommandLength(const TDesC& aScript, TInt aIndex);

        /**
        * Handles next command of plugin custom script
        */
		void HandleNextPluginCommandL(CRFSPlugin* aPlugin, 
                                      const TDesC& aScript, 
                                      TInt aIndex, 
                                      TInt aLength, 
                                      const TRfsReason aType);

        /**
        * Returns KErrNone if next command of plugin script is supported
        * @return TInt (error code)
        */
		TInt IsNextCommandValid(const TDesC& aScript, TInt aIndex, TInt aLength);

        /**
        * Reorders plug-in implementation array if opaque_data field of some plug-in is filled
        */		
		void ReOrderImplArrayL(RImplInfoPtrArray& aImplInfoArray);
		
        /**
        * Moves plug-in in implementation array from position to other. Only moves to earlier positions
        * allowed.
        */		
		void MoveImplementation(RImplInfoPtrArray& aImplInfoArray, 
		                        TInt aSourcePos, 
		                        TInt aDestinationPos);		

		
    private:    // data

        CRfsServer* iServer;        // not owned
        CRfsSession* iSession;      // not owned

        TRfsCommandId iCommandId;            
        TInt iNumberOfParams;
        TPtrC* iParams;             // owned (array)

        TFileName iTargetFile;
        TInt iNumberOfPlugins;
    };


#endif      // RFS_COMMAND_H
