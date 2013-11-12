/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  main.cpp
 *
 */

#include <QIconEnginePlugin>
#include <QStringList>
#include <QDebug.h>

#include "camificonengine.h"
#include "cambmiconengine.h"

QT_BEGIN_NAMESPACE

class CaMifIconPlugin : public QIconEnginePluginV2
{
public:
    QStringList keys() const;
    QIconEngineV2 *create(const QString &filename = QString());
};

QStringList CaMifIconPlugin::keys() const
{
    return QStringList() << QLatin1String("mif") << QLatin1String("mbm");
}

QIconEngineV2 *CaMifIconPlugin::create(const QString &file)
{
    QIconEngineV2 *engine(0);
    if (file.endsWith(QLatin1String(".mif"), Qt::CaseInsensitive)) {
        engine  = new CaMifIconEngine;
    } else if (file.endsWith(QLatin1String(".mbm"), Qt::CaseInsensitive)) {
        engine  = new CaMbmIconEngine;
    }
    return engine;
}

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (QT macro)

Q_EXPORT_STATIC_PLUGIN(CaMifIconPlugin)
Q_EXPORT_PLUGIN2(camificon, CaMifIconPlugin)

#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

QT_END_NAMESPACE


