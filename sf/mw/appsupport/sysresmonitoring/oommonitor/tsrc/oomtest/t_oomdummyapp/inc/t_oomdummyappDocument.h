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




#ifndef __T_OOMDUMMYAPPDOCUMENT_h__
#define __T_OOMDUMMYAPPDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class Ct_oomdummyappAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * Ct_oomdummyappDocument application class.
 * An instance of class Ct_oomdummyappDocument is the Document part of the
 * AVKON application framework for the t_oomdummyapp example application.
 */
class Ct_oomdummyappDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a Ct_oomdummyappDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of Ct_oomdummyappDocument.
	 */
	static Ct_oomdummyappDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a Ct_oomdummyappDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of Ct_oomdummyappDocument.
	 */
	static Ct_oomdummyappDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~Ct_oomdummyappDocument
	 * Virtual Destructor.
	 */
	virtual ~Ct_oomdummyappDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a Ct_oomdummyappAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * Ct_oomdummyappDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	Ct_oomdummyappDocument(CEikApplication& aApp);

	};

#endif // __T_OOMDUMMYAPPDOCUMENT_h__
// End of File
