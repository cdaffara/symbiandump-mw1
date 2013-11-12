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
 * Description:
 *
 */

#ifndef CA_QT_SF_HANDLER_LOADER_H
#define CA_QT_SF_HANDLER_LOADER_H

#include <QMap>
#include <QString>
#include <QScopedPointer>

#include "cahandlerloader.h"

class CaQtSfHandlerLoader:
    public CaHandlerLoader
{
public:
    CaQtSfHandlerLoader();
    CaHandler *loadHandler(const QString &entryTypeName,
                           const QString &commandName);
protected:
    void registerPlugins() const;
};

#endif
