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
#ifndef AFACTIVATION_H
#define AFACTIVATION_H

#include "afactivities_global.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

class AfActivationPrivate;

class AFACTIVITIES_EXPORT AfActivation : public QObject
{
    Q_OBJECT
    
public:
    AfActivation(QObject *parent = 0);
    
signals:
    void activated(Af::ActivationReason reason, QString name, QVariantHash parameters);
    
public slots:
    QVariantHash parameters() const;
    Af::ActivationReason reason() const;
    QString name() const;
        
private:
    AfActivationPrivate *d_ptr;

};

#endif // AFACTIVATION_H
