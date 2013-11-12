/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  FtuWizardFactory class definition
*
*/

#ifndef FTUWIZARDFACTORY_H
#define FTUWIZARDFACTORY_H


class FtuWizard;

/**
 * @ingroup group_ftuwizardmodel
 * @brief A factory class through which the wizard plugin can be instantiated.
 *
 * FtuWizardFactory contains a factory method to create a wizard plugin instance.
 * 
 * @lib ?library
 * @since S60 ?S60_version
 */

class FtuWizardFactory
{
public:
    /**
     * Instantiates a wizard plugin object.
     * @return The constructed wizard plugin object.
     * @since S60 ?S60_version.
     */    
	   virtual FtuWizard* createWizard() const = 0;
};

Q_DECLARE_INTERFACE(FtuWizardFactory, "ftu.nokia.com.FtuWizardFactory/1.0");

#endif // FTUWIZARDFACTORY_H
