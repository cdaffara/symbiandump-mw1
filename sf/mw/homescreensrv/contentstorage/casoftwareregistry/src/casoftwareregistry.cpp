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
 * Description:  ?Description
 *
 */
#include <QtGlobal>
#include <QString>
#include <QStringList>

#include <cauninstallnotifier.h>
#include <casoftwareregistry.h>

#include "casoftwareregistry_p.h"


// ======== MEMBER FUNCTIONS ========

/*!

 \class CaSoftwareRegistry.
 \brief This class provides access to USIF provided data and converting
 them from Symbian to Qt types.

 CaSoftwareRegistry class provides a factory method which returns smart pointer
 to CaSoftwareRegistry instnce to ensure
 automatical memory cleanup once the reference count drops to 0.

 \code
 QSharedPointer<CaSoftwareRegistry> service = CaSoftwareRegistry::create();
 \endcode

 Subsequent calls to CaSoftwareRegistry::create() may return pointers to different
 instances. It is a case when between the calls instance counter of the created
 object dropped to 0 and it was deleted.

 */

/*! \typedef typedef QHash<QString, QString> DetailMap;
 * Defines map type for component details.
 *
 */

/*!
 \var CaSoftwareRegistryPrivate::m_q
 Points to the CaSoftwareRegistry instance that uses this private implementation.
 */


// Initialization of a static member variable.
QWeakPointer<CaSoftwareRegistry> CaSoftwareRegistry::m_instance =
    QWeakPointer<CaSoftwareRegistry>();


/*!
 Constructor.
 \param parent pointer to a parent. It defaults to NULL.
 */
CaSoftwareRegistry::CaSoftwareRegistry(QObject *parent) :
    QObject(parent), m_d(new CaSoftwareRegistryPrivate(this))
{

}

/*!
 Returns a pointer to an instance of the CaSoftwareRegistry class.
 \retval A pointer to an instance of the CaSoftwareRegistry class.
 */
QSharedPointer<CaSoftwareRegistry> CaSoftwareRegistry::create()
{
    QSharedPointer<CaSoftwareRegistry> service(m_instance);
    if (!service) {
        service = QSharedPointer<CaSoftwareRegistry>(new CaSoftwareRegistry);
        m_instance = service.toWeakRef();
    }
    return service;
}

/*!
 Destructor.
 */
CaSoftwareRegistry::~CaSoftwareRegistry()
{
    delete m_d;
}

/*!
 Provides details needed for uninstalling process of Java applications.
 \code
 QSharedPointer<CaSoftwareRegistry> service = CaSoftwareRegistry::create();
 int componentId(20);
 QString &componentName,
 QStringList applicationsUids;
 QString confirmationMessage;
 CaSoftwareRegistry::DetailMap detailMap = service->getUninstallDetails(
    componentId,
    componentName,
    applicationsUids,
    confirmationMessage);
 \endcode
 \param[in] componentId component id of an application to be uninstalled.
 \param[out] componentName a name of the component.
 \param[out] applicationsUids a list of uids of applications in the package
      of the given component id.
 \param[out] confirmationMessage optional deletion confirmation message,
      null string means the lack of the message.
 \retval true if there is no error.
 */
bool CaSoftwareRegistry::getUninstallDetails(int componentId,
    QString &componentName,
    QStringList &applicationsUids,
    QString &confirmationMessage)
{
    return m_d->getUninstallDetails(componentId,
        componentName,
        applicationsUids,
        confirmationMessage);
}

/*!
 Provides a list of uids of applications installed by the given package.
 \param[in] componentId component id of an application to be uninstalled.
 \param[out] applicationsUids a list of uids of applications in the package
      of the given component id.
 \retval true if there is no error.
 */

bool CaSoftwareRegistry::getApplicationsUids(int componentId,
    QStringList &applicationsUids)
{
    return m_d->getApplicationsUids(componentId, applicationsUids);
}

/*!
 The method provides component details from USIF for a given component id.
 \code
 QSharedPointer<CaSoftwareRegistry> service = CaSoftwareRegistry::create();
 CaSoftwareRegistry::DetailMap detailMap = service->entryDetails(5);
 QString appName = detailMap[CaSoftwareRegistry::componentNameKey()];
 \endcode
 \param componentId Component id of the entry details are requested for.
 \return Map of the component details if component id was greater than 0 or
 empty map otherwise.

 */
CaSoftwareRegistry::DetailMap CaSoftwareRegistry::entryDetails(
    int componentId) const
{
    return m_d->entryDetails(componentId);
}

/*!
 The method provides installation details from USIF.
 \return QList of DetailMap.
 */
QList<CaSoftwareRegistry::DetailMap>
        CaSoftwareRegistry::retrieveLogEntries() const
{
    return m_d->retrieveLogEntries();
}

/*!
 Creates uninstall notifier.
 \retval pointer to created CaUninstallNotifier instance.
 */
CaUninstallNotifier *CaSoftwareRegistry::createUninstallNotifier() const
{
    return m_d->createUninstallNotifier();
}

/*!
 * \return Component name key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentNameKey()
{
    static const QString key("name");
    return key;
}

/*!
 * \return Component version key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentVersionKey()
{
    static const QString key("version");
    return key;
}

/*!
 * \return Component vendor key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentVendorKey()
{
    static const QString key("vendor");
    return key;
}

/*!
 * \return Component drive info key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentDriveInfoKey()
{
    static const QString key("driveInfo");
    return key;
}

/*!
 * \return Component protection domainkey in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentProtectionDomainKey()
{
    static const QString key("protectiondomain");
    return key;
}

/*!
 * \return Component size info key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentSizeKey()
{
    static const QString key("size");
    return key;
}

/*!
 * \return Component type key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentTypeKey()
{
    static const QString key("type");
    return key;
}

/*!
 * \return Component description key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentDescriptionKey()
{
    static const QString key("description");
    return key;
}

/*!
 * \return Component instalation/uninstallation
 * time key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentTimeKey()
{
    static const QString key("time");
    return key;
}

/*!
 * \return Component instalation/uninstallation/upgrade/hidden
 * operation type key in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentOperationTypeKey()
{
    static const QString key("operationType");
    return key;
}

/*!
 * \return Component instalation
 * operation value in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentInstallValue()
{
    static const QString value("install");
    return value;
}

/*!
 * \return Component uninstallation
 * operation value in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentUninstallValue()
{
    static const QString value("uninstall");
    return value;
}

/*!
 * \return Component upgrade
 * operation value in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentUpgradeValue()
{
    static const QString value("upgrade");
    return value;
}

/*!
 * \return Component hidden
 * operation value in CaSoftwareRegistry::DetailMap.
 */
QString CaSoftwareRegistry::componentHiddenValue()
{
    static const QString value("hidden");
    return value;
}

