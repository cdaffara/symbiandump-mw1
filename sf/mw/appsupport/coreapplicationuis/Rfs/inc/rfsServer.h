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
*  The RFS server class.
*
*
*/


#ifndef RFS_SERVER_H
#define RFS_SERVER_H

//  INCLUDES
#include <e32base.h>
#include <s32file.h>
#include <eikenv.h>
#include <eikappui.h>
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
#include <driveinfo.h>
#endif //RD_MULTIPLE_DRIVE_ENABLERS

#include "rfsClient.h"
#include "rfsPluginDef.h"

// CONSTANTS

const TUint KRfsServMajorVersionNumber = 1;
const TUint KRfsServMinorVersionNumber = 0;
const TUint KRfsServBuildVersionNumber = 0;

// needed for creating server thread.
const TUint KDefaultHeapSize = 0x10000;

// opcodes used in message passing between client and server
enum TRfsServRequest
    {
    ERfsServRunScriptInDescriptor,
    ERfsServRunScriptInFile,
    ERfsServPerformRfs,
    ERfsServPerformPostEikonRfs
    };

_LIT( KRfsServerName, "RfsServer" );

#ifdef RD_MULTIPLE_DRIVE_ENABLERS
_LIT( KRfsScriptLocation, "\\private\\100059C9\\");
_LIT( KRfsScriptFileBody, "Script");
_LIT( KRfsCenRepScript, "cenrep_rfs.txt" );
_LIT( KRfsRFSNormalScript, "script0.txt" );
_LIT( KRfsDriveMarker, ":");
#else
_LIT( KRfsScriptFileStartRom, "Z:\\private\\100059C9\\Script");
_LIT( KRfsScriptFileStartSys, "c:\\private\\100059C9\\Script" );
_LIT( KRfsCenRepScript, "z:\\private\\100059C9\\cenrep_rfs.txt" );
_LIT( KRfsRFSScript, "z:\\private\\100059C9\\script0.txt" );
_LIT( KRfsMergedScript, "c:\\private\\100059C9\\script0.txt" );
_LIT( KRfsPluginScriptPath, "c:\\Private\\100059C9\\" );
#endif //RD_MULTIPLE_DRIVE_ENABLERS
_LIT( KRfsScriptFileExtension, ".txt" );
_LIT( KRfsInitScriptIdentifier, "Init" );
_LIT( KRfsPluginScriptFileBody, "plg_script" );

// FUNCTION PROTOTYPES

// function to start the server thread/process
GLREF_C TInt StartServer();

// thread function for WINS builds
#ifdef __WINS__
GLREF_C TInt ThreadFunction( TAny* aThreadParams );
#endif

// FORWARD DECLARATIONS
class CRfsSession;
class CRfsShutdown;
class CRfsCompleter;
class CCommsDatabase;

// CLASS DECLARATIONS

// classes for creating a dummy eikon env

class CRfsEikonEnv : public CEikonEnv
    {
    public:
        void ConstructL();
        void DestroyEnvironment();
    };

class CRfsAppUi : public CEikAppUi
    {
    public:
        void ConstructL();
    };

/**
*  The server class, an active object.
*  Contains an instance of RServer, a handle to the kernel server
*  representation which is used to receive messages. 
*/

class CRfsServer : public CServer2
    { 

    public:

        // some small help classes

        class TSignal   // signal class for waiting while thread/process
            {           // is being set up
            public:
                inline TSignal();
                inline TInt Set( const TDesC& aData );
                inline TSignal( TRequestStatus& aStatus );
                inline TPtrC Get() const;
                TRequestStatus* iStatus;
                TThreadId iId;
            };

    public:  // Constructors and destructor

        /**
        * constructs and starts the server.
        */
        static CRfsServer* New();

        /**
        * destructor
        */
        virtual ~CRfsServer();

    public: // New functions

        /**
        * Opens a new session to the server.
        *
        * @param aVersion required version of the server.
        *
        * @return pointer to a new session
        *
        * Leaves if:
        * - Required version not supported (KErrNotSupported)
        * - Out of memory (KErrNoMemory)
        */
        CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;

        /**
        * Server bootstrap code.
        *
        * @param aSignal signal that's used to inform the client thread when the
        *                server is up.
        */
        IMPORT_C static TInt ThreadStart( CRfsServer::TSignal& aSignal );

        /**
        * Runs descriptor script
        * @return TInt (error code)
        */
        TInt RunScriptInDescriptorL( const TDesC& aScript,
                                     CRfsSession* aSession );

        /**
        * Handles file script
        * @return TInt (error code)
        */
        TInt HandleScriptInFileL( const TDesC& aFileName, CRfsSession* aSession );

        /**
        * Handles script
        * @return TInt (error code)
        */
        TInt HandleScriptL( const TDesC& aScript, CRfsSession* aSession );

        /**
        * Runs script
        * @return TInt (error code)
        */
        TInt RunScriptL( HBufC* aBuffer, CRfsSession* aSession );

        /**
        * Performs custom scripts of plugins
        */
        void PerformPluginScriptsL( CRfsSession* aSession );

        /**
        * Performs RFS
        * @return TInt (error code)
        */
        TInt PerformRfsL( TRfsType aType, CRfsSession* aSession );

        /**
        * Performs RFS after startup of Eikon server
        * @param aType RFS type
        * @param aSession pointer to the session requesting RFS
        * @return TInt (error code)
        */
        TInt PerformPostEikonRfs( TRfsType aType, CRfsSession* aSession );


        /**
        * Increments sessions
        */
        void IncrementSessions();

        /**
        * Decrements sessions
        */
        void DecrementSessions();

        /**
        * Signals the client
        */
        static void SignalClient();

        /**
        * Gives RFS reason
        * @return TRfsReason
        */
        TRfsReason GetRfsReason();

        /**
        * Gives additional value used in executing just the matched plugins
        * @return The match value
        */
        inline TPtrC PluginMatch() const { return iPluginMatch; }

    protected:

        /**
        * C++ default constructor.
        */
        CRfsServer();

        /**
        * Second phase constructor.
        */      
        void ConstructL();

    private:    // new functions

        /**
        * 
        */
        TInt MergeCenRepScriptL();

        /**
        * Finds start and end points of given script
        */
        void FindBoundariesOfCommand( TInt& aBegin, 
                                      TInt& aEnd, 
                                      TInt aOffset,
                                      TInt aScriptLength,
                                      const TDesC& aScript);

        /**
        * Handles %ROOTPATH% tag
        */
        void HandleRootpathTagsL(HBufC* aBuffer);

        /**
        * Handles %INTERNALDRIVES% tag
        */
        void HandleInternalDrivesTagsL(HBufC* aBuffer);

        /**
        * Handles %MASSSTORAGES% tag
        */
        void HandleMassStoragesTagsL(HBufC* aBuffer);
        
        /**
        * Handles %SYSTEMDRIVE% tag
        */
        void HandleSystemDriveTagsL(HBufC* aBuffer);
        
        /**
        * Handles %ROMDRIVE% tag
        */
        void HandleRomDriveTagsL(HBufC* aBuffer);

        /**
         * Handles %RAMDRIVE% tag
         */        
        void HandleRamDriveTagsL(HBufC* aBuffer);
        
        /**
        * Handles %GAMESPATH% tag
        */
        void HandleGamesPathTagsL(HBufC* aBuffer);

        /**
        * Handles %PICTURESPATH% tag
        */
        void HandlePicturesPathTagsL(HBufC* aBuffer);

        /**
        * Handles %DIGITALSOUNDSPATH% tag
        */
        void HandleDigitalSoundsPathTagsL(HBufC* aBuffer);

        /**
        * Handles %SIMPLESOUNDSPATH% tag
        */
        void HandleSimpleSoundsPathTagsL(HBufC* aBuffer);

        /**
        * Handles %INSTALLSPATH% tag
        */
        void HandleInstallsPathTagsL(HBufC* aBuffer);

        /**
        * Handles %OTHERSPATH% tag
        */
        void HandleOthersPathTagsL(HBufC* aBuffer);

        /**
        * Handles %VIDEOSPATH% tag
        */
        void HandleVideosPathTagsL(HBufC* aBuffer);

        /**
         * Removes pointed command
         */        
        void RemoveDoubleBackslashesL(HBufC* aBuffer);
        
        /**
        * Removes pointed command
        */
        void RemovePointedCommandL(HBufC* aBuffer, TInt offset);

        /**
        * Runs the plug-ins
        */
        void DoRunPluginsL( CRfsSession* aSession );

        /**
        * Runs the plug-ins by match
        */        
        TInt DoRunPlugins( CRfsSession* aSession, const TDesC& aPluginMatch );

    private:    // data

        RFs iFs;

        CFileMan* iFileMan;         // owned
        CRfsShutdown* iShutdown;    // owned

        TInt iSessionCount;
        
        TRfsReason iRfsReason;
        TChar iSystemDriveChar;
        TChar iRomDriveChar;
        TChar iRamDriveChar;
        TPtrC iPluginMatch;

        /** List of plugin scripts. */
        RPointerArray<HBufC> iPluginScriptList;

    private:    // friend classes

        friend class CRfsSession;
        friend class CRfsCommand;
        friend class CRfsIsiMsg;
    };

#include "rfsServer.inl"

#endif      // RFS_SERVER_H
