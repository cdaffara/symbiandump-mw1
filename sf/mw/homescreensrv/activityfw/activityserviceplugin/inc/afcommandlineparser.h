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
#ifndef AFCOMMANDLINEPARSER_H
#define AFCOMMANDLINEPARSER_H

#include "afactivities_global.h"
#include <QVariant>
#include <QString>
#include <QHash>
#include <QStringList>

class AfCommandLineParser {
public:
    static void parseCommandLine(const QStringList &commandLineParams,
            Af::ActivationReason &reason,
            QString &name,
            QVariantHash &parameters);   
private: 
    static bool isServiceCommand(const QStringList &commandLineParams);
    static bool isActivityCommand(const QStringList &commandLineParams);
    static void parseServiceParameters(const QStringList &commandLineParams,
            Af::ActivationReason &reason,
            QString &name,
            QVariantHash &parameters); 
    static void parseActivityParameters(const QStringList &commandLineParams,
            Af::ActivationReason &reason,
            QString &name,
            QVariantHash &parameters); 
public: // TODO make privete when deprecated api removed
    static QVariantHash getActivityParameters(const QStringList &commandLineParams);
};

#endif // AFCOMMANDLINEPARSER_H
