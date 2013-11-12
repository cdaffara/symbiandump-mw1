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
 * Description: power save mode Indicator class
 *
 */

#ifndef PSMINDICATOR_H
#define PSMINDICATOR_H

#include <QObject>

#include <QVariant>
#include <QtCore/QProcess>

#include <hbindicatorinterface.h>
#include <hbindicatorplugininterface.h>
#include <hbtranslator.h>


/**
 * PSM indicator class. 
 * Handles client request and shows the indications. 
 */
class PsmIndicatorPlugin : public HbIndicatorInterface, public HbIndicatorPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(HbIndicatorPluginInterface)
    
public:

    PsmIndicatorPlugin();
    ~PsmIndicatorPlugin();
public:
    //from HbindicatorInterface    
    bool handleInteraction(InteractionType type);
    QVariant indicatorData(int role) const;
    
public:
    //from HbIndicatorPluginInterface
    QStringList indicatorTypes() const;
    bool accessAllowed(const QString &indicatorType,const QVariantMap &  securityInfo) const;   
    HbIndicatorInterface* createIndicator(const QString &indicatorType);
    int error() const;

      
protected:
    //from HbindicatorInterface
    bool handleClientRequest(RequestType type, const QVariant &parameter);
   
   
private:

    QString iDisplayName;
    QString iIcon;
    QProcess iProcess;
    QStringList iIndicatorTypes;
    HbTranslator *iTrans;
  
    
private slots:
    void processError(QProcess::ProcessError err); // handler for error codes

private:
    Q_DISABLE_COPY(PsmIndicatorPlugin)
    int mError;
    };

#endif // PSMINDICATOR_H

