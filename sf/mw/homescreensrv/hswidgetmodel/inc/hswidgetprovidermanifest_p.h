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
* Description:  HsWidget provider manifest private implementation.
*
*/


#ifndef HSWIDGETPROVIDERMANIFEST_P_H
#define HSWIDGETPROVIDERMANIFEST_P_H

#include <QDomElement>
#include "hsiwidgetprovider.h"


class HsWidgetProviderManifest;

class HsWidgetProviderManifestPrivate
{
public:

    HsWidgetProviderManifestPrivate(HsWidgetProviderManifest *handle);
    ~HsWidgetProviderManifestPrivate();

    QList<HsWidgetToken> widgets() const;
	bool loadFromXml(const QString &fileName);

    HsWidgetProviderManifest *mPublic;
    QList<HsWidgetToken> mWidgetTokens;

    bool mLoadOnQuery;

private:

    QString parseAttribute(QDomElement &element, 
            const QString& attributeName, bool isRequired = true) const;

    Q_DISABLE_COPY(HsWidgetProviderManifestPrivate)
};




#endif
