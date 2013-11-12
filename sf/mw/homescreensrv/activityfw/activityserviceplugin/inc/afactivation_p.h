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
#ifndef AFACTIVATION_P_H
#define AFACTIVATION_P_H

#include "afactivities_global.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantHash>
#include <QSharedPointer>

#include "afstorageproxy.h"

class AfActivationPrivate : public QObject
{
    Q_OBJECT
    
public:
    AfActivationPrivate(const QSharedPointer<AfStorageProxy> &connection, QObject *parent = 0);
       
public:
    QVariantHash parameters() const;
    Af::ActivationReason reason() const;
    QString name() const;
       
signals:
    void activated(Af::ActivationReason reason, QString name, QVariantHash parameters);        
        
private slots:
    void handleActivityRequest(const QString &activityId);
    void bringToForeground();        
               
private:
    QVariantHash mParameters;
    Af::ActivationReason mReason;
    QString mName;
        
private:
    QSharedPointer<AfStorageProxy> mConnection;     

};

#endif // AFACTIVATION_P_H
