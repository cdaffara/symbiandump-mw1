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

#ifndef CAINSTALLNOTIFIER_H
#define CAINSTALLNOTIFIER_H

#include <e32base.h>
#include <usif/sif/sifnotification.h>

/**
 * Interface for updating after installer events.
 *
 * @since S60 v5.0
 */
class MCaInstallListener
    {

public:

    /**
     * Pure virtual method.
     * @param aEvent event type.
     */
    virtual void HandleInstallNotifyL() = 0;
    };

/**
 *  CA Install notifier.
 *
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCaInstallNotifier ) : public CBase,
        public Usif::MSifOperationsHandler
    {

public:

    /**
     * Enum defining notification type.
     */
    enum TNotificationType
        {
        ENoNotification,            ///< No notification.
        ESisInstallNotification,    ///< System installation notification.
        EAllTypesNotification,    ///< System installation notification.
        };

    /**
     * Creates an instance of CCaInstallNotifier.
     * @param aNotifier Reference to notifier interface.
     * @param aNotificationType Notification type.
     */
IMPORT_C static CCaInstallNotifier* NewL( MCaInstallListener& aListener,
            TNotificationType aNotificationType = EAllTypesNotification );

    /**
     * Called when a new operation is started. The client should use the S
     * ubscribeL() method (@see CSifOperationsNotifier) to register for progress
     * and end notification for this operation. This function should return
     * quickly as this is run as part of an active object request completion
     * handler.

     * @param aKey The Start End key of the operation.
     * @param aStartData Start operation related information.
     */
    void StartOperationHandler(
            TUint aKey, const Usif::CSifOperationStartData& aStartData);

    /**
     * Called when an operation completes.
     * This function should return quickly as this is run as part of an active
     * object request completion handler.

     * @param aEndData End operation related information.
     */
    void EndOperationHandler(const Usif::CSifOperationEndData& aEndData);

    /**
     * Called whenever a progress update is available.
     * This function should return quickly as this is run as part of an active
     * object request completion handler.

     * @param aProgressData Progress operation related information.
     */
    void ProgressOperationHandler(
            const Usif::CSifOperationProgressData& aProgressData);

    /**
     * Destructor.
     */
    virtual ~CCaInstallNotifier();

private:

    /**
     * Constructor.
     * @param aNotifier Reference to notifier interface.
     */
    CCaInstallNotifier( MCaInstallListener& aListener );

    /**
     * Symbian 2nd phase constructor.
     * @param aNotificationType Notification type.
     */
    void ConstructL( TNotificationType aNotificationType );

private:

    /**
     * Interface for notifying changes in folder.
     * Not Own.
     */
    MCaInstallListener& iListener;

    /**
     * USIF notifier
     * Own.
     */
    Usif::CSifOperationsNotifier* iNotifier;

    TUint iKey;

    TNotificationType iNotificationType;

    };

#endif /* CAINSTALLNOTIFIER_H */

// End of File

