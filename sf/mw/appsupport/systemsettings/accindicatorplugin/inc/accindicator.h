/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Accessory Indicator class
 *
 */

#ifndef ACCINDICATOR_H
#define ACCINDICATOR_H

#include <QObject>

#include <QVariant>
#include <QtCore/QProcess>

#include <hbindicatorinterface.h>
#include <hbindicatorplugininterface.h>

#include <accpolaccessorymode.h>

/**
 * Accessory indicator class. 
 * Handles client request and shows the indications. 
 */
class AccIndicatorPlugin : public HbIndicatorInterface, public HbIndicatorPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(HbIndicatorPluginInterface)
public:

    AccIndicatorPlugin();
    ~AccIndicatorPlugin();
public:
    //from HbindicatorInterface    
    bool handleInteraction(InteractionType type);
    QVariant indicatorData(int role) const;
    
public:
    //from HbIndicatorPluginInterface
    QStringList indicatorTypes() const;
    bool accessAllowed(const QString &indicatorType, const QVariantMap &securityInfo) const;   
    HbIndicatorInterface* createIndicator(const QString &indicatorType);
    int error() const;

protected:
    //from HbindicatorInterface
    bool handleClientRequest(RequestType type, const QVariant &parameter);
    
private: 
    void prepareDisplayName();
   
private:

    QString mDisplayName;
    TAccMode mAccMode;
    int mAccType;
    QStringList mIndicatorTypes;
    QStringList mArgs;
    
private:
    Q_DISABLE_COPY(AccIndicatorPlugin)
    int mError;
    };

#endif // ACCINDICATOR_H

