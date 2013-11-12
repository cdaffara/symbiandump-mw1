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
* Description:  
*       test application for qt control panel public apis.
*/

#include "mycpsettingformentryitemdata.h"

/*!
    \class MyCpSettingFormEntryItemData
    \brief The MyCpSettingFormEntryItemData class is designed for the unit testing for the protected member functions in CpSettingFormEntryItemData class.
 */

/*!
 * This is designed to test one constructor in CpSettingFormEntryItemData class.
 */
MyCpSettingFormEntryItemData::MyCpSettingFormEntryItemData(const HbDataFormModelItem *parent /*= 0*/) :
CpSettingFormEntryItemData(parent)
    {
    
    }
