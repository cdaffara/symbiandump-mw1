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
* Description:  Interface for controlpanel plugins.
*
*/

#ifndef CPPLUGININTERFACE_H
#define CPPLUGININTERFACE_H

#include <QtPlugin>
#include <QList>

/*!
    \class CpPluginInterface
    \brief The class CpPluginInterface is an interface for controlpanel plugins.
 */

class CpSettingFormItemData;
class CpItemDataHelper;

class CpPluginInterface
{
public:
    /*!
     Destructor of CpPluginInterface.
     */
    virtual ~CpPluginInterface()
    {
    }
    
    /*!
     Create a set of data form mode item, by which the control panel framework can build a data form model.
     */
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const = 0;
};

Q_DECLARE_INTERFACE(CpPluginInterface, "com.nokia.controlpanel.plugin.interface/1.0");

#endif /* CPPLUGININTERFACE_H */
