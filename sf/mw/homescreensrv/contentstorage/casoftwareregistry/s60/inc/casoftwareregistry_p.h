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
 * Description: casoftwareregistry_p.h
 *
 */

#ifndef CASOFTWAREREGISTRY_P_H
#define CASOFTWAREREGISTRY_P_H

#include <QHash>
#include <QString>

#include <usif/usifcommon.h>

class QStringList;
class CaSoftwareRegistry;
class CaUninstallNotifier;

namespace Usif
{
    class CComponentEntry;
}

class CaSoftwareRegistryPrivate
{
public:

    typedef QHash<QString, QString> DetailMap;

    explicit CaSoftwareRegistryPrivate(
        CaSoftwareRegistry *softwareRegistryPublic);
    ~CaSoftwareRegistryPrivate();

    bool getUninstallDetails(int componentId,
        QString &componentName,
        QStringList &applicationsUids,
        QString &confirmationMessage);

    bool getApplicationsUids(int componentId,
        QStringList &appUids);

    DetailMap entryDetails(int componentId) const;

    QList<DetailMap> retrieveLogEntries() const;
    CaUninstallNotifier *createUninstallNotifier() const;
    
private:
    QList<DetailMap> retrieveLogEntriesL() const;
    DetailMap entryDetailsL(
            const Usif::CComponentEntry& entry) const;
    void getUninstallDetailsL(int componentId,
        QString &componentName,
        QStringList &appUids,
        QString &confirmationMessage);
    void getApplicationsUidsL(int componentId,
        QStringList &appUids);
    DetailMap entryDetailsL(int componentId) const;
    //TODO int should be changed to const Usif::TScrComponentOperationType&
    //when the problem with usif headers is solved
    QString operationTypeL(int operationType) const;
    QString operationTime(TTime time) const;
private:
    CaSoftwareRegistry *const m_q;
    friend class T_CaSoftwareRegistry;
};

#endif // CASOFTWAREREGISTRY_P_H
