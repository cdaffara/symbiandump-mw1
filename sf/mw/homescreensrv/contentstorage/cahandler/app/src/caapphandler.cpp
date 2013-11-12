/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QString>
#include <e32base.h>
#include <w32std.h>
#include <apgtask.h>
#include <apgcli.h>
#include <eikenv.h>
#include <eikappui.h>
#include <AknDef.h>
#include <AknTaskList.h>

#include <usif/usifcommon.h>
#include <usif/scr/scr.h>
#include <usif/usiferror.h>

#include <cadefs.h>
#include <caentry.h>

#include "caapphandler.h"
#include "cainnerentry.h"
#include "causifuninstalloperation.h"
#include "catasklist.h"
#include "cautils.h"
#include "caclient_defines.h"
#include "cadef.h"

using namespace Usif;

static const char caTypeApp[] = "application";
static const char caTypePackage[] = "package";
static const char caTypeWidget[] = "widget";
static const char caAttrView[] = "view";
static const char caAttrComponentId[] = "component_id";

/*!
 *  Command handler for application entries.
 *
 *  \lib caclient.lib
 */

/*!
 * Constructor.
 */
CaAppHandler::CaAppHandler(QObject *parent):
    iEikEnv(CEikonEnv::Static()),
    iUsifUninstallOperation(NULL)
{
    Q_UNUSED(parent);
}

/*!
 * Destructor.
 */
CaAppHandler::~CaAppHandler()
{
    delete iUsifUninstallOperation;
}

/*!
 * Execute a given command.
 * \param entry a reference to a CaEntry instance.
 * \param command a given command.
 * \param receiver a QObject class with slot to invoke.
 * \param member a slot to invoke.
 * \retval an error code.
 */
int CaAppHandler::execute(const CaEntry &entry, const QString &command,
        QObject* receiver, const char* member)
{
    if ( receiver && member ) {
        connect( this, SIGNAL(uninstallFailed(int)), receiver, member, Qt::UniqueConnection );
    }
    
    int result(KErrGeneral);
    if (command == caCmdOpen && entry.entryTypeName() == caTypeApp) {
        QString viewIdValue = entry.attribute(caAttrView);
        bool viewIdIsCorrect(true);
        int viewId(-1);
        if (!viewIdValue.isNull()) {
            viewId = viewIdValue.toInt(&viewIdIsCorrect, 0);
        }
        if (viewIdIsCorrect) {
            QString uidValue =
                entry.attribute(APPLICATION_UID_ATTRIBUTE_NAME);
            int uid = uidValue.toInt();
            TRAP(result, launchApplicationL(TUid::Uid(uid), viewId));
        }
    } else if (command == caCmdRemove) {
        QString componentId(entry.attribute(caAttrComponentId));
        result = handleRemove(entry.flags(),
            entry.entryTypeName(),
            componentId);
    } else {
        result = KErrNotSupported;
    }

    return result;
}

/*!
 * Launch application
 * \param uid UID of the application to launch.
 * \param viewId id of the view the application is to start in.
 */
void CaAppHandler::launchApplicationL(const TUid uid, TInt viewId)
{
    if (viewId > 0 && iEikEnv) {
        TVwsViewId view(uid, TUid::Uid(viewId));
        iEikEnv->EikAppUi()->ActivateViewL( view );
    } else {
        RWsSession wsSession;
        User::LeaveIfError(wsSession.Connect());
        CleanupClosePushL<RWsSession>(wsSession);

        CCaTaskList* taskList = CCaTaskList::NewLC(wsSession);
        TApaTask task = taskList->FindRootApp(uid);
        CleanupStack::PopAndDestroy(taskList);

        if (task.Exists()) {
            task.BringToForeground();
        } else {
            // TApaAppInfo size is greater then 1024 bytes
            // so its instances should not be created on the stack.
            TApaAppInfo* appInfo = new(ELeave) TApaAppInfo;
            CleanupStack::PushL(appInfo);
            TApaAppCapabilityBuf capabilityBuf;
            RApaLsSession appArcSession;
            User::LeaveIfError(appArcSession.Connect());
            CleanupClosePushL<RApaLsSession>(appArcSession);

            User::LeaveIfError(appArcSession.GetAppInfo(*appInfo, uid));
            User::LeaveIfError(appArcSession.GetAppCapability(
               capabilityBuf, uid));

            TApaAppCapability &caps = capabilityBuf();
            CApaCommandLine *cmdLine = CApaCommandLine::NewLC();
            cmdLine->SetExecutableNameL(appInfo->iFullName);

            if (caps.iLaunchInBackground) {
                cmdLine->SetCommandL(EApaCommandBackground);
            } else {
                cmdLine->SetCommandL(EApaCommandRun);
            }

            cmdLine->SetTailEndL(KNullDesC8);

            User::LeaveIfError(appArcSession.StartApp(*cmdLine));

            CleanupStack::PopAndDestroy(cmdLine);
            CleanupStack::PopAndDestroy(&appArcSession);
            CleanupStack::PopAndDestroy(appInfo);
        }
        CleanupStack::PopAndDestroy( &wsSession );
    }
}


/*!
 * Uninstall application.
 * \param flags flags of the application which is to uninstall.
 * \param typeName type name of the entry.
 * \param componentId component id.
 * \retval an error code.
 */
int CaAppHandler::handleRemove(const EntryFlags &flags,
    const QString &typeName,
    const QString &componentId)
{
    int result(KErrNone);
    if (flags.testFlag(RemovableEntryFlag)) {
        if (typeName == caTypeApp
            || typeName == caTypePackage
            || typeName == caTypeWidget) {
            if(componentId.isNull()) {
                result = KErrNotFound;
            } else {
                bool convertStatus(false);
                int id = componentId.toInt(&convertStatus);
                if (convertStatus) {
                    TRAP(result, startUsifUninstallL(id));
                } else {
                    result = KErrGeneral;
                }
            }
        }
        else {
            result = KErrNotSupported;
        }
    } else {
        result = KErrAccessDenied;
    }
    return result;
}

/*!
 * Start uninstall operation via usif.
 * \param componentId component id.
 */
void CaAppHandler::startUsifUninstallL(TInt componentId)
{
    if (iUsifUninstallOperation && iUsifUninstallOperation->IsActive()) {
        uninstallError(Usif::EInstallerBusy);
    } else {
        delete iUsifUninstallOperation;
        iUsifUninstallOperation = NULL;
        iUsifUninstallOperation = CCaUsifUninstallOperation::NewL(componentId);
        iUsifUninstallOperation->AddObserver(this);
    }
}

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT 
/*!
 * Show information message about unninstall error.
 * \param error uninstall error.
 * \retval void.
 */
void CaAppHandler::uninstallError(int error)
{
   	emit uninstallFailed(error);
}
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT


