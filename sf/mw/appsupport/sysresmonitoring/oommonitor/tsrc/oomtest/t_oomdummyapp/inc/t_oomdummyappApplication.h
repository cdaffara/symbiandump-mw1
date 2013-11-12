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




#ifndef __T_OOMDUMMYAPPAPPLICATION_H__
#define __T_OOMDUMMYAPPAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "t_oomdummyapp.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidt_oomdummyappApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * Ct_oomdummyappApplication application class.
 * Provides factory to create concrete document object.
 * An instance of Ct_oomdummyappApplication is the application part of the
 * AVKON application framework for the t_oomdummyapp example application.
 */
class Ct_oomdummyappApplication : public CAknApplication
	{
public:
	Ct_oomdummyappApplication(TUid aUid, TUint aAlloc);
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidt_oomdummyappApp).
	 */
	TUid AppDllUid() const;

	~Ct_oomdummyappApplication();
protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates Ct_oomdummyappDocument document object. The returned
	 * pointer in not owned by the Ct_oomdummyappApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	
private:
	TUid iUID;
	TUint iAlloc;
	TAny* iMemory;
	};

#endif // __T_OOMDUMMYAPPAPPLICATION_H__
// End of File
