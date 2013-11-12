// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef HWRMUIPLUGIN_H
#define HWRMUIPLUGIN_H

#include <e32base.h>
#include <coemain.h>

#include <hwrm/hwrmuipluginservice.h>

/*		
*	Cone Plugin implementation class: registers with CONE
*   and listens for foreground events on behalf of data member 
*	iMHWRMForegroundObserver, the handling of which is delegated to the 
*	iMHWRMForegroundObserver data member
*/
class CHwrmUiPlugin : public CHwrmUiPluginService, public MCoeForegroundObserver
	{
public:
	static CHwrmUiPlugin* NewL();	
	~CHwrmUiPlugin();

protected: //From CHwrmUiPluginService
	virtual void AddForegroundObserverL(MHwrmForegroundObserver* aObserver);
	virtual TBool InForeground() const;

protected: //From MCoeForegroundObserver
	virtual void HandleGainingForeground();
	virtual void HandleLosingForeground();	

protected:	
	CHwrmUiPlugin();
	void ConstructL();
    
private:	
	MHwrmForegroundObserver* iHwrmForegroundObserver; //NOT OWNED
	};
	
#endif
