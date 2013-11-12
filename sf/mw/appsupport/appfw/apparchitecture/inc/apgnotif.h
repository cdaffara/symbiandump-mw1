// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __APGNOTIF_H__
#define __APGNOTIF_H__

#include <apgcli.h>


class MApaAppListServObserver
/**
MApaAppListServObserver is interface to get application list change notifications.

@publishedPartner
@released
*/
	{
public:
	enum TApaAppListEvent
		{
		EAppListChanged=1
		};
	virtual void HandleAppListEvent(TInt aEvent)=0;
protected:
	IMPORT_C MApaAppListServObserver();
private:
	IMPORT_C virtual void MApaAppListServObserver_Reserved1();
	IMPORT_C virtual void MApaAppListServObserver_Reserved2();
private:
	TInt iMApaAppListServObserver_Reserved1;
	};


class CApaAppListNotifier : public CActive
/** 
 * CApaAppListNotifier is an active object which notifies the client when an application list is changed.
 * Provides notification when applications are added, deleted or if the localisable data of an application
 * is modified.
 * 
 * Clients can create instance of this class using NewL function which takes MApaAppListServObserver
 * observer as one of the parameters. When application list is changed, CApaAppListNotifier will call 
 * HandleAppListEvent function of the provided observer. 
 * 
 * 
 * Example:
 * 
 * //Implementation of observer interface
 * class CAppListChangeObserver:public CBase, public MApaAppListServObserver
 * {
 * public:
 *     void HandleAppListEvent(TInt aEvent);    
 *     CApaAppListNotifier* iNotifier;    
 * };
 * 
 * //This function is called by CApaAppListNotifier when an application list is changed 
 * void CAppListChangeObserver::HandleAppListEvent(TInt aEvent)
 * {
 *     //Handle application list change event
 *     ...
 * }
 * 
 * 
 * TInt E32Main()
 * {
 * ....
 * 
 * CAppListChangeObserver* obs=new(ELeave) CAppListChangeObserver();
 * CApaAppListNotifier *notif=CApaAppListNotifier::NewL(obs, CActive::EPriorityStandard);
 * 
 * ....
 * }
 * 
 * 

@publishedPartner
@released */
	{
public:
	IMPORT_C ~CApaAppListNotifier();
	IMPORT_C static CApaAppListNotifier* NewL(MApaAppListServObserver* aObserver, TPriority aPriority);
private: // from CActive
	void DoCancel();
	void RunL();
    TInt RunError(TInt aError);	
private:
	CApaAppListNotifier(MApaAppListServObserver& aObserver, TPriority aPriority);
	void ConstructL();
private:
	MApaAppListServObserver& iObserver;
	RApaLsSession iLsSession;
	};

#endif
