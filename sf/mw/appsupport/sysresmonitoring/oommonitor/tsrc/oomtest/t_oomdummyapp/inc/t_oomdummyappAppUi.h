/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*/




#ifndef __T_OOMDUMMYAPPAPPUI_h__
#define __T_OOMDUMMYAPPAPPUI_h__

// INCLUDES
#include <aknappui.h>

// FORWARD DECLARATIONS
class Ct_oomdummyappAppView;
class CMsgHandler;

// CLASS DECLARATION
/**
 * Ct_oomdummyappAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class Ct_oomdummyappAppUi : public CAknAppUi
	{
public:
	
	void SetPriorityBusy();
	void SetPriorityHigh();
	void SetPriorityNormal();
	
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * Ct_oomdummyappAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	Ct_oomdummyappAppUi();

	/**
	 * ~Ct_oomdummyappAppUi.
	 * Virtual Destructor.
	 */
	virtual ~Ct_oomdummyappAppUi();

	void HandleHarnessCommandL(TInt aCommand);
private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();

	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 */
	CArrayFix<TCoeHelpContext>* HelpContextL() const;

private:
	// Data

	/**
	 * The application view
	 * Owned by Ct_oomdummyappAppUi
	 */
	Ct_oomdummyappAppView* iAppView;
	CMsgHandler* iMsgHandler;

	};

#endif // __T_OOMDUMMYAPPAPPUI_h__
// End of File
