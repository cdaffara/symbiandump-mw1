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
#include <HbParameterLengthLimiter>
#include <HbExtendedLocale>
#include <QMetaType>
#include <QScopedPointer>
#include <QStringList>
#include <QDateTime>
#undef SYMBIAN_ENABLE_SPLIT_HEADERS //TODO this is work around
#include <usif/scr/screntries.h>
#include <usif/scr/scr.h>
#include <xqconversions.h>
#include <driveinfo.h>
#include <hbtextresolversymbian.h>

#include "cauninstallnotifier.h"
#include "cauninstallnotifier_p.h"
#include "casoftwareregistry.h"
#include "casoftwareregistry_p.h"
#include "cadefs.h"
#include "caarraycleanup.inl"
#include "cautils.h"

using namespace Usif;
_LIT(KConfirmMessageKey, "MIDlet-Delete-Confirm");
_LIT(KCaScrPropertyDomainCategory, "Domain-Category");
_LIT(KCaScrPropertyMidletDescryption, "MIDlet-Description");
_LIT(KNotNamedMmc, "NO NAME");
const int maxLogsCount = 20;  // should be 50 - workaround for
// ou1cimx1#476143 Dialog crash when label contains big amount of lines

/*!
 Constructor
 \param servicePublic Pointer to object of the public class.
 */
CaSoftwareRegistryPrivate::CaSoftwareRegistryPrivate(
        CaSoftwareRegistry *servicePublic):
    m_q(servicePublic)
{
}

/*!
 Destructor.
 */
CaSoftwareRegistryPrivate::~CaSoftwareRegistryPrivate()
{

}

/*!
 Provides details needed for uninstalling process of Java applications.
 \param[in] componentId component id of an application to be uninstalled.
 \param[out] componentName a name of the component.
 \param[out] applicationsUids a list of uids of applications in the package
      of the given component id.
 \param[out] confirmationMessage optional deletion confirmation message,
      null string means the lack of the message.
 \retval true if there is no error.
 */

bool CaSoftwareRegistryPrivate::getUninstallDetails(int componentId,
        QString &componentName,
        QStringList &applicationsUids,
        QString &confirmationMessage)
{
    TRAPD(error, getUninstallDetailsL(componentId,
            componentName,
            applicationsUids,
            confirmationMessage)
             );
    return error == KErrNone;
}

/*!
 Provides a list of uids of applications installed by the given package.
 \param[in] componentId component id of an application to be uninstalled.
 \param[out] applicationsUids a list of uids of applications in the package
      of the given component id.
 \retval true if there is no error.
 */

bool CaSoftwareRegistryPrivate::getApplicationsUids(int componentId,
        QStringList &applicationsUids)
{
    TRAPD(error, getApplicationsUidsL(componentId, applicationsUids));
    return error == KErrNone;
}

/*!
 Provides details needed for uninstalling process of Java applications
   (leaving version).
 \param[in] componentId component id of an application to be uninstalled.
 \param[out] componentName a name of the component.
 \param[out] applicationsUids a list of uids of applications in the package
      of the given component id.
 \param[out] confirmationMessage optional deletion confirmation message,
      null string means the lack of the message.
 */
void CaSoftwareRegistryPrivate::getUninstallDetailsL(int componentId,
        QString &componentName,
        QStringList &appUids,
        QString &confirmationMessage)
{
    componentName.clear();
    appUids.clear();
    confirmationMessage.clear();

    if (componentId >= 1) {
        TComponentId componentIdValue(componentId);
        RArray<TUid> appUidsArray;
        CleanupClosePushL(appUidsArray);
        CLocalizablePropertyEntry *confirmationMessageProperty = NULL;

        RSoftwareComponentRegistry softwareComponentRegistry;
        CleanupClosePushL(softwareComponentRegistry);
        User::LeaveIfError(softwareComponentRegistry.Connect());

        CComponentEntry *entry = CComponentEntry::NewLC();
        softwareComponentRegistry.GetComponentL(componentId, *entry);
        softwareComponentRegistry.GetAppUidsForComponentL(
                componentIdValue, appUidsArray);

        CPropertyEntry *confirmationProperty
                = softwareComponentRegistry.GetComponentPropertyL(
                        componentId, KConfirmMessageKey);
        if (confirmationProperty
                && confirmationProperty->PropertyType()
                        == CPropertyEntry::ELocalizedProperty) {
            confirmationMessageProperty
                    = static_cast<CLocalizablePropertyEntry *>(
                            confirmationProperty);
        } else {
            delete confirmationProperty;
            confirmationProperty = NULL;
        }

        QT_TRYCATCH_LEAVING(componentName =
            XQConversions::s60DescToQString(entry->Name());
            for (TInt i = 0; i<appUidsArray.Count(); i++) {
                appUids.append(QString::number(appUidsArray[i].iUid));
            }
            if (confirmationMessageProperty) {
                confirmationMessage = XQConversions::s60DescToQString(
                        confirmationMessageProperty->StrValue());
            }
        );

        CleanupStack::PopAndDestroy(3, &appUidsArray);
    }
}

/*!
 Provides details needed for uninstalling process of Java applications
   (leaving version).
 \param[in] componentId component id of an application to be uninstalled.
 \param[out] applicationsUids a list of uids of applications in the package
      of the given component id.
 */
void CaSoftwareRegistryPrivate::getApplicationsUidsL(int componentId,
    QStringList &appUids)
{
    appUids.clear();
    if (componentId >= 1) {
        TComponentId componentIdValue(componentId);
        RArray<TUid> appUidsArray;
        CleanupClosePushL(appUidsArray);

        RSoftwareComponentRegistry softwareComponentRegistry;
        CleanupClosePushL(softwareComponentRegistry);
        User::LeaveIfError(softwareComponentRegistry.Connect());

        softwareComponentRegistry.GetAppUidsForComponentL(
                componentIdValue, appUidsArray);

        QT_TRYCATCH_LEAVING(
            for (TInt i = 0; i<appUidsArray.Count(); i++) {
                appUids.append(QString::number(appUidsArray[i].iUid));
            }
        );
        CleanupStack::PopAndDestroy(2, &appUidsArray);
    }
}

/*!
 \param componentId Component id of the entry which details are requested for.
 \return Map of component details if component id was greater than 0 or
 empty map otherwise.
 */
CaSoftwareRegistryPrivate::DetailMap CaSoftwareRegistryPrivate::entryDetails(
    int componentId) const
{
    CaSoftwareRegistry::DetailMap result;
    TRAP_IGNORE(result=entryDetailsL(componentId));
    return result;
}

/*!
 \param componentId Component id of the entry which details are requested for.
 \return Map of component details if component id was greater than 0 or
 empty map otherwise.
 */
CaSoftwareRegistryPrivate::DetailMap CaSoftwareRegistryPrivate::entryDetailsL(
    int componentId) const
{
    CaSoftwareRegistry::DetailMap result;

    if (componentId >= 1) {
        RSoftwareComponentRegistry scr;
        CleanupClosePushL(scr);
        if (scr.Connect() == KErrNone) {
            CComponentEntry* entry = CComponentEntry::NewLC();
            if (scr.GetComponentL(componentId, *entry)) {
                result = entryDetailsL(*entry);
                if (entry->SoftwareType().Compare(KSoftwareTypeJava) == 0) {
                    CPropertyEntry* domainProperty =
                        scr.GetComponentPropertyL(componentId,
                            KCaScrPropertyDomainCategory);
                    CleanupStack::PushL(domainProperty);
                    if (domainProperty &&
                        domainProperty->PropertyType() ==
                            CPropertyEntry::ELocalizedProperty) {
                        const TDesC& domainPropertyValue = static_cast<CLocalizablePropertyEntry*>(
                                domainProperty)->StrValue();
                        TInt splitIndex = domainPropertyValue.Locate(',');
                        RBuf qmFileName;
                        qmFileName.CleanupClosePushL();
                        qmFileName.CreateL(domainPropertyValue.Right(
                                domainPropertyValue.Length() - splitIndex - 1),
                                domainPropertyValue.Length() - splitIndex);
                        qmFileName.Append('_');
                        MenuUtils::InitTextResolverSymbianL(qmFileName);
                        HBufC* translatedProtectionDomain = HbTextResolverSymbian::LoadLC(
                                domainPropertyValue.Left(splitIndex));
                        result[CaSoftwareRegistry::componentProtectionDomainKey()] =
                                XQConversions::s60DescToQString(*translatedProtectionDomain);
                        CleanupStack::PopAndDestroy(translatedProtectionDomain);
                        CleanupStack::PopAndDestroy(&qmFileName);
                    }

                    CPropertyEntry *midletDescryption =
                        scr.GetComponentPropertyL(componentId,
                            KCaScrPropertyMidletDescryption);
                    CleanupStack::PushL(midletDescryption);
                    if (midletDescryption &&
                        midletDescryption->PropertyType() ==
                            CPropertyEntry::ELocalizedProperty) {
                        result[CaSoftwareRegistry::componentDescriptionKey()] =
                            XQConversions::s60DescToQString(
                                static_cast<CLocalizablePropertyEntry*>(
                                    midletDescryption)->StrValue());
                    }
                    CleanupStack::PopAndDestroy(midletDescryption);
                    CleanupStack::PopAndDestroy(domainProperty);
                }
            }
            CleanupStack::PopAndDestroy(entry);
        }
        CleanupStack::PopAndDestroy(&scr);
    }
    return result;
}

/*!
 \param entry Software registry entry providing details.
 \return Map with details for the component represented by \entry.
 */
CaSoftwareRegistryPrivate::DetailMap CaSoftwareRegistryPrivate::entryDetailsL(
    const CComponentEntry& entry) const
{
    CaSoftwareRegistry::DetailMap detailMap;

    detailMap[CaSoftwareRegistry::componentNameKey()] =
        XQConversions::s60DescToQString(entry.Name());

    detailMap[CaSoftwareRegistry::componentVersionKey()] =
        XQConversions::s60DescToQString(entry.Version());

    detailMap[CaSoftwareRegistry::componentVendorKey()] =
        XQConversions::s60DescToQString(entry.Vendor());

    QString drives;
    TChar drive;

    const TInt driveListLen(entry.InstalledDrives().Length());
    for (TInt i( 0 ); i < driveListLen; ++i) {
        if (entry.InstalledDrives()[i] != '\0') {

            if (!drives.isEmpty()) {
                drives = drives.append("\n");
            }
            if (DriveInfo::GetDefaultDrive(
                    DriveInfo::EDefaultPhoneMemory, drive ) == KErrNone
                    && QChar('A'+ i) == QChar(drive)) {
                drives = drives.append(HbParameterLengthLimiter(
                        "txt_applib_dialog_1_device_memory").arg(
                                QString(QChar('A'+ i))));
            } else if (DriveInfo::GetDefaultDrive(
                    DriveInfo::EDefaultMassStorage, drive ) == KErrNone
                    && QChar('A'+ i) == QChar(drive)) {
                drives = drives.append(HbParameterLengthLimiter(
                        "txt_applib_dialog_1_mass_storage").arg(
                                QString(QChar('A'+ i))));
                } else if (DriveInfo::GetDefaultDrive(
                    DriveInfo::EDefaultRemovableMassStorage, drive ) == KErrNone
                    && QChar('A'+ i) == QChar(drive)) {
                RFs fs;
                CleanupClosePushL(fs);
                if (fs.Connect() == KErrNone) {
                    TInt driveNumber;
                    TVolumeInfo tv;
                    DriveInfo::GetDefaultDrive(
                            DriveInfo::EDefaultRemovableMassStorage,
                            driveNumber );
                    User::LeaveIfError(fs.Volume(tv, driveNumber));
                    if (tv.iName.Length()
                            && tv.iName.Compare(KNotNamedMmc) != KErrNone) {
                        drives = drives.append(HbParameterLengthLimiter(
                                "txt_applib_dialog_1_2").arg(
                                        QString(QChar('A'+ i))).arg(
                                                XQConversions::s60DescToQString(
                                                        tv.iName)));
                    } else {
                        drives = drives.append(HbParameterLengthLimiter(
                                "txt_applib_dialog_1_memory_card").arg(
                                        QString(QChar('A'+ i))));
                    }
                }
                CleanupStack::PopAndDestroy(&fs);
            } else {
                drives = drives.append(QChar('A'+ i)).append(":");
            }
        }
    }
    detailMap[CaSoftwareRegistry::componentDriveInfoKey()] = drives;

    static const TInt64 KKilo = 1024;
    static const TInt64 KMega = KKilo * KKilo;
    if(entry.ComponentSize() >= KMega) {
        detailMap[CaSoftwareRegistry::componentSizeKey()]
                  = HbParameterLengthLimiter("txt_applib_dialog_l1_mb").arg(
                          static_cast<double>(entry.ComponentSize() / KMega));
    } else {
        detailMap[CaSoftwareRegistry::componentSizeKey()]
                  = HbParameterLengthLimiter("txt_applib_dialog_l1_kb").arg(
                          static_cast<double>(entry.ComponentSize() / KKilo));
    }
    detailMap[CaSoftwareRegistry::componentTypeKey()] =
        XQConversions::s60DescToQString(entry.SoftwareType());

    return detailMap;
}

/*!
 \param entry Software registry entry providing details.
 \return Map with details for the component represented by \entry.
 */
QList<CaSoftwareRegistryPrivate::DetailMap>
        CaSoftwareRegistryPrivate::retrieveLogEntries() const
{
    QList<CaSoftwareRegistryPrivate::DetailMap> logMap;
    TRAP_IGNORE(logMap = retrieveLogEntriesL());
    return logMap;
}
/*!
 Creating uninstall notifier
 \return CaUninstallNotifier instance
 */
CaUninstallNotifier *CaSoftwareRegistryPrivate::createUninstallNotifier() const
{
    return CaUninstallNotifier::notifier();
}

/*!
 \param entry Software registry entry providing installation log.
 \return List of Maps with installation log.
 */
QList<CaSoftwareRegistryPrivate::DetailMap>
        CaSoftwareRegistryPrivate::retrieveLogEntriesL() const
{
    QList<CaSoftwareRegistryPrivate::DetailMap> logList;

    RSoftwareComponentRegistry scr;
    CleanupClosePushL(scr);
    User::LeaveIfError(scr.Connect());

    RPointerArray<CScrLogEntry> logEntries;
    CleanupResetAndDestroyPushL(logEntries);
    scr.RetrieveLogEntriesL(logEntries);

    // 50 log items or less - ui spec
    TInt lastLog(0);
    if (logEntries.Count() > maxLogsCount) {
        lastLog = logEntries.Count() - maxLogsCount;
    }
    // iteration through most recent log to the last
    for (TInt i = logEntries.Count() - 1; i >= lastLog; i--) {
        CScrLogEntry* entry = logEntries[i];
        DetailMap logMap;
        logMap[CaSoftwareRegistry::componentNameKey()]
               = XQConversions::s60DescToQString(entry->ComponentName());
        logMap[CaSoftwareRegistry::componentVersionKey()]
               = XQConversions::s60DescToQString(entry->ComponentVersion());
        logMap[CaSoftwareRegistry::componentTimeKey()]
               = operationTime(entry->OperationTime());
        logMap[CaSoftwareRegistry::componentOperationTypeKey()]
               = operationTypeL(entry->OperationType());
        logList.append(logMap);
    }
    CleanupStack::PopAndDestroy(&logEntries);
    CleanupStack::PopAndDestroy(&scr);

    return logList;
}

/*!
 \param operation type.
 \return string representing operation type.
 */
QString CaSoftwareRegistryPrivate::operationTypeL(int operationType) const
{
    QString opType;
    switch (operationType) {
        case EScrCompInstall:
            opType = CaSoftwareRegistry::componentInstallValue();
            break;
        case EScrCompUnInstall:
            opType = CaSoftwareRegistry::componentUninstallValue();
            break;
        case EScrCompUpgrade:
            opType = CaSoftwareRegistry::componentUpgradeValue();
            break;
        case EScrCompHidden:
            opType = CaSoftwareRegistry::componentHiddenValue();
            break;
        default:
            User::Leave(KErrNotSupported);
            break;
    }
    return opType;
}

/*!
 \param operation type.
 \return string representing operation type.
 */
QString CaSoftwareRegistryPrivate::operationTime(TTime time) const
{
    TDateTime dt = time.DateTime();
    QDate date(dt.Year(), dt.Month() + 1, dt.Day() + 1);
    HbExtendedLocale hbLoc;
    return hbLoc.format(date, r_qtn_date_usual_with_zero);
}
