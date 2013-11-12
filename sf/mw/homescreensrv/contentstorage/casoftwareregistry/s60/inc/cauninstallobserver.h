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

/**
 *  IDataObserver
 *
 */
class IUninstallObserver
{
public:
    /**
     * Method invoked when entry is changed.
     * @param entryId entry id.
     * @param changeType change type.
     */
    virtual void progressChange(TInt componentId,
                              TInt valueOfProgress) const = 0;

};



class CaUninstallObserver: public QObject, public IUninstallObserver
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
    
    /**
     * Method invoked when entry is changed.
     * @param entryId entry id.
     * @param changeType change type.
     */
    void progressChange(TInt componentId,
                              TInt valueOfProgress) const;


signals:

    void signalprogressChange(int componentId, int valueOfProgress) const;
};


#endif /* CAUINSTALLOBSERVER_H_ */
