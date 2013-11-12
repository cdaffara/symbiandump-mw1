/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Utility class for controlpanel plugins.
*
*/

#include "cppluginutility.h"
#include "cpdataformbuttonentryviewitem.h"
#include "cpdataformlistentryviewitem.h"

#include <hbdataform.h>

/*! \class CpPluginUtility
	\brief  This class supply the utility functions for controlpanel plugins.
 */

/*!
	Add item prototypes which are supported by controlpanel framework to a setting form.  
 */
void CpPluginUtility::addCpItemPrototype(HbDataForm *settingForm)
{
    if (settingForm) { 
        QList<HbAbstractViewItem *>prototypeList = settingForm->itemPrototypes();
        prototypeList.append(new CpDataFormButtonEntryViewItem());
        prototypeList.append(new CpDataFormListEntryViewItem());
        settingForm->setItemPrototypes(prototypeList);
    }
}
