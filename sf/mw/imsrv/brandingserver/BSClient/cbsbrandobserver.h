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
* Description:  CBSBrandObserver.h
*
*/



#ifndef __CBSBRANDOBSERVER_H__
#define __CBSBRANDOBSERVER_H__
#include <e32std.h>
#include <e32base.h>

class MBSBrandChangeObserver;
class MBSBackupRestoreStateObserver;
class MBSBrandObserverRegistrar;

/**
 *
 */
NONSHARABLE_CLASS( CBSBrandObserver ): public CActive
    {
	public:
		/**
		* Two-phased constructor.
		*/
		static CBSBrandObserver* NewL( MBSBrandChangeObserver* aObserver,
									   MBSBackupRestoreStateObserver* aBackupObserver,	
									   MBSBrandObserverRegistrar* aRegistrar );

    	virtual ~CBSBrandObserver();

    public: // From CActive

        void RunL();   
		void DoCancel();

	private:
		CBSBrandObserver( MBSBrandChangeObserver* aObserver,
						  MBSBackupRestoreStateObserver* aBackupObserver,	
						  MBSBrandObserverRegistrar* aRegistrar );


    	void ConstructL();

	private:


	private: // data
	
		// doesn't own: observer
		MBSBrandChangeObserver* iObserver ;
		
		// doesn't own: observer
		MBSBackupRestoreStateObserver* iBackupObserver ;
		
		// doesn't own: registrar
		MBSBrandObserverRegistrar* iRegistrar;
		
		TBool iContinueBackupStateObserving ;
		TBool iContinueDataUpdateObserving ;
    };

#endif      //  __CBSBRANDOBSERVER_H__


//  END OF FILE

