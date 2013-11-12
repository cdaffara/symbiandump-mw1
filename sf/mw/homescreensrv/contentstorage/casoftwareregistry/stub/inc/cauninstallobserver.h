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
 *  Description : Content Arsenal client session
 *
 */

#ifndef CAUINSTALLOBSERVER_H_
#define CAUINSTALLOBSERVER_H_

#include <QObject>


class CaUninstallObserver: public QObject
{
    Q_OBJECT
public:
    /**
     * Constructor.
     * @param parent pointer to parent. Defaults to NULL.
     */
    CaUninstallObserver(QObject *parent = 0);

    /**
     * Destructor.
     */
    ~CaUninstallObserver();
    
};


#endif /* CAUINSTALLOBSERVER_H_ */
