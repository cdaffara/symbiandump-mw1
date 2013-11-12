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
* Description:  Test support file tool
 *
*/


#ifndef CPRFWTESTFILETOOL_H
#define CPRFWTESTFILETOOL_H 

#include <e32base.h>
#include <ximpbase.h>

class CXIMPApiEventBase;
class CXIMPTestFileSrvMsg;
class MXIMPTestFileObserver;

/**
 * File support for tests.
 * Support functions for externalizing a class to a known directory and
 * retrieving the classes.
 *
 * This is a sort of "pseudo-IPC" from the test protocol (ecom plugin)
 * to EUnit test code. A server could also be used, but this is faster
 * to do and simpler.
 *
 * @since S60 v4.0
 */
class CXIMPTestFileTool : public CBase
    {
    public:

        IMPORT_C static CXIMPTestFileTool* NewL( TUint32 aUid, const TDesC16& aInstanceId );
        IMPORT_C static CXIMPTestFileTool* NewL( TUint32 aUid, TInt aInstanceIdAsNum );
        virtual ~CXIMPTestFileTool();

    private:

        CXIMPTestFileTool( TUint32 aUid );
        void ConstructL( const TDesC16& aInstanceId );
        void ConstructL( TInt aInstanceIdAsNum );

    public:

        /**
         * Clean all files from the filetool directory.
         * Remove also the directory itself.
         * Removes EVERYTHING from ALL DIRECTORIES used
         * by filetool!!! So don't call it everytime, only once
         * upon test case startup.
         */
        IMPORT_C static void CleanAllL();

        /**
         * Prepares for writing only the directory used by this
         * filetool instance
         */
        IMPORT_C void PrepareL();

        /**
         * Store the object to file.
         * The given descriptor is externalized as given,
         * with the length of the descriptor written first,
         * as a 32-bit TUint:
         * <length of descriptor 32-bit><descriptor data>
         * @param aExternalizedObject The externalized object
         */
        IMPORT_C void PluginStoreL( const TDesC8& aExternalizedObject );
        
        /**
         * Restore a stored object from file.
         * Ownership is transferred!
         * @param aObjIndex The index for the object
         */
        IMPORT_C CXIMPApiEventBase* PluginRestoreLC( TInt aObjIndex );
        
        /**
         * Store the object to file. (SERVER MESSAGE)
         * The given descriptor is externalized as given,
         * with the length of the descriptor written first,
         * as a 32-bit TUint:
         * <length of descriptor 32-bit><descriptor data>
         * @param aExternalizedObject The externalized object
         */
        IMPORT_C void SrvMsgStoreL( CXIMPTestFileSrvMsg* aSrvMsg );
        
        /**
         * Restore a stored object from file. (SERVER MESSAGE)
         * Ownership is transferred!
         * @param aIndex The index for the object
         */
        IMPORT_C CXIMPTestFileSrvMsg* SrvMsgRestoreLC( TInt aIndex );

        /**
         * Return the number of objects currently stored in the directory.
         */
        IMPORT_C TInt NumObjectsL();
        
        /**
         * Return the number of objects depicting server-originated
         * message currently stored in the directory.
         */
        IMPORT_C TInt NumSrvMsgL();
        
        /**
         * @return Instance id
         */
        IMPORT_C const TDesC16& InstanceId() const;

    private:

        /**
         * @return The directory name (with backslash)
         * (E.g. "C:/temp/prfw/1234/0/SrvMsg/")
         */
        HBufC* GetDirNameAndBackslashLC( 
                const TDesC& aTemplate );

        /**
         * @return Wildcard for file deletion 
         * (E.g. "C:/temp/prfw/1234/0/SrvMsg/file*.prfw")
         * @param aDirTemplate The directory template for the wildcard
         */
        HBufC* GetWildcardLC( 
                const TDesC& aDirTemplate );

        /**
         * Get the full file name, properly expanded.
         * @param aDirTemplate The template for the dirs
         * @param aObjIndex The object index
         */
        HBufC* GetFileNameLC( 
                const TDesC& aDirTemplate,
                TInt aObjIndex );

        /**
         * @return Length of the given number as if it were a string.
         * @param aNum The number
         */
        TInt NumLenInChars( TInt aNum );

        /**
         * @return Listing of the directory
         * @param aTemplate The wildcard template for the files
         */
        CDir* GetDirListL( const TDesC& aTemplate );
        
        /**
         * Common helper for NumObjects methods.
         */
        TInt DoNumObjectsL( const TDesC& aTemplate );
        
        /**
         *
         */
        static TInt Process( TAny* aMyself );
        void DoProcessL();
        
    public:
      
        /**
         * Observer registration
         */
        void RegisterObserverL( MXIMPTestFileObserver* aObserver );        
        void UnregisterObserver( MXIMPTestFileObserver* aObserver );
        
        
    private: // data

        TUint32 iUid;       // protocoluid
        HBufC16* iInstance;     // the global instance id

        TInt iObjIndex;     // object index for created objects
        TInt iSrvObjIndex;  // object index for created srvmsg objects
        RFs iFs;
        
        
        /**
         * Monitor on when there is observers. 
         * Call HandleL for observers for every new file.         
         */
        RPointerArray< MXIMPTestFileObserver > iObservers;
        
        // timer
        CPeriodic* iTimer;        
    };

#endif // CPRFWTESTFILETOOL_H 
