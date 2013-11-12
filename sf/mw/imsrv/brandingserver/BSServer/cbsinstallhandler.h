/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  cbsinstallhandler.h
*
*/



#ifndef __CBSINSTALLHANDLER_H__
#define __CBSINSTALLHANDLER_H__

#include <e32base.h>

// FORWARD DECLARATIONS
class RFs;

/**
 * CBSInstallHandler
 *
 * Handles installation and uninstallation of brand packages.
 * Calls external application "bsinstall.exe" to do the actual work.
 * Can Observe the installation directory so that any sis installation
 * can be detected while the server is running.
 *
 */
NONSHARABLE_CLASS( CBSInstallHandler ): public CActive
	{
	public: 
	    
	    // Operations for folder synchronization:
	    enum TInstallOperation
	        {
	        // copy files from source to destination if they don't
	        // already exists. 
	        EInstallCopyNewToDest,
	        
	        // Delete files from source directory if the same
	        // files doesn't exist in destination directory.
	        EInstallDeleteFromSrc
	        };
	
	public: // constructors
		
		static CBSInstallHandler* NewL();

    	virtual ~CBSInstallHandler();

	private: // constructors
	
		void ConstructL();
						 
		CBSInstallHandler();

	public:
	
        /**
        * Uninstall removed brands and installs new brands.
        * This should be called right after construction to sync the
        * installation state.
        */
        void InstallNewFilesL();
        
        /**
        * Start observing installation folder. Detects new brands if they
        * are installed while the server is active.
        */
        void StartObservingL();
        
        /**
        * Stop observing installation folder. All new brands are installed
        * only when calling InstallNewFilesL.
        */
        void StopObserving();

    private: // From CActive
    
        /**
        * @see CActive
        */
        void RunL();
        
        /**
        * @see CActive
        */
        void DoCancel();

    private: // new methods
        
        /**
        * Launches external installer application. It should install all
        * available brands into server.
        */
        void LaunchInstallerAppL();
               
        /**
        * Sync files between aSrcDir and aDestDir, according to given 
        * operation. See TInstallOperation for operations. Calls 
        * OperationNotifyL before every operation.
        *
        * @param aSrcDir Source directory
        * @param aDestDir Destination directory
        * @param aOperation Sync operation to execute
        * @return ETrue  - Something synchronized
        *         EFalse - Nothing synchronized
        */
        TBool SyncFilesL( const TDesC& aSrcDir, const TDesC& aDestDir, 
                          TInstallOperation aOperation );
        
        /**
        * Notification that file is about to be synchronized. 
        *
        * @param aOperation operation type which will be executed
        * @param aSrcFile file under operation
        */
        void OperationNotifyL( TInstallOperation aOperation, 
                               const TDesC& aSrcFile );
        
        /**
        * check if some version of given brand is installed in
        * branding server. 
        *
        * @param aBrand brand installation file to check
        *        this has to be brand file name without extension and 
        *        version information.
        */
        TBool IsBrandInstalled( const TDesC& aBrand );                  

        /**
        * Helper method to get servers private path
        * @param aPath contains the path to servers private folder
        * @param aAppend text to add into private path.
        */
        void GetPrivateFolder( TDes& aPath, const TDesC& aAppend = KNullDesC );
	
        /**
        * To create a flag file if brand gets discarded
        * @param aSrcFile file under operation
        */
		void CreateFlagFile(const TDesC& aSrcFile);

        /**
        * Checks if an discarded brand exists.
        * (It also writes the RProperty for the bsinstall.exe)
        *
        * @param aDir the folder of the data storages
        * @return ETrue, in case a discarded brand was found, EFalse otherwise
        */
        TBool CheckForDiscardedBrandsL( const TDesC& aDir );
        
        
        /**
        * Updates the RProperty. The specified string is added to the
        * RProperty only if RProperty does not contain the same string yet.
        *
        * @param aUninstallationString a string
        */
        void UpdateRPropertyL( const TDesC& aUninstallationString );
        
	private: // data

        // Own. File server session handle.
        RFs     iFs;
    };

#endif      //  __CBSINSTALLHANDLER_H__
//  END OF FILE
