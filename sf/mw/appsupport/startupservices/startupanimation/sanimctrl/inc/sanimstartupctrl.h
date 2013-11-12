/*
* Copyright (c) 2007,2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CSAnimStartupCtrl class
*
*/


#ifndef SANIMSTARTUPCTRL_H
#define SANIMSTARTUPCTRL_H

#include "sanimctrl.h"
#include <MediatorCommandInitiator.h>
#include <MediatorCommandResponder.h>

class CRepository;

/**
*  Start-up Animation UI control class with support for reading animation
*  properties from Central Repository and synchronising wilt Secondary Display.
*
*  @lib sanimctrl.lib
*  @since S60 3.2
*/
class CSAnimStartupCtrl
  : public CSAnimCtrl,
    public MMediatorCommandObserver,
    public MMediatorCommandResponseObserver
    {

public:

    /**
    * Contains Central Repository keys which can be used to find out parameters
    * for the animation.
    *
    * @since S60 3.2
    */
    NONSHARABLE_CLASS( TAnimationParams )
        {
    public:
        /** Identifies the repository to use for fetching the following values. */
        TUid iRepositoryUid;
        /** Value of the key identifies the filename and path of the animation file. */
        TInt iAnimationPathKey;
        /**
        * Value of the key gives a frame delay to use unless the animation file
        * contains that information.
        */
        TInt iFrameDelayKey;
        /**
        * Value of the key indicates whether to try to scale the image to fit
        * the screen if possible.
        */
        TInt iEnableScalingKey;
        /** Value of the key identifies the filename and path of the tone file. */
        TInt iTonePathKey;
        /** Value of the key gives the volume to use for playing the tone. */
        TInt iVolumeKey;
        };

public:

    /**
    * Constructs a CSAnimStartupCtrl object.
    *
    * @since S60 3.2
    *
    * @param aRect Rectangle designated for the control.
    * @param aContainer The compound control that is the container for this control.
    * start-up sync command (only required if secondary display is supported).
    * @return The new object
    */
    IMPORT_C static CSAnimStartupCtrl* NewL(
        const TRect& aRect,
        const CCoeControl& aContainer );
    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimStartupCtrl();

    /**
    * Return ETrue if the animation has any content (image or tone).
    *
    * @since S60 3.2
    *
    * @return ETrue if the animation has any content (image or tone).
    */
    virtual TBool HasContent() const;

    /**
    * Load the parts of the show (image/animation and/or tone) from file(s).
    * Completes immediately with KErrNone if the show has no parts.
    * From CSAnimCtrl.
    *
    * @since S60 3.2
    *
    * @param aParams Central Repository keys for finding out details of the
    * animation.
    * @param aPlayDefaultBeep Indicates whether the default 'beep' tone should
    * be played during the animation, if the parameters do not specify a valid
    * tone file.
    * @param aSyncCommand Identifies the parameters for secondary display
    * @param aStatus Request to complete when loading is complete.
    */
    virtual void Load(
        const TAnimationParams& aParams,
        const TBool aPlayDefaultBeep,
        const TInt aSyncCommand,
        TRequestStatus& aStatus );

    /**
    * Send synchronization command to Secondary Display (if required) and start
    * the show (image/animation and/or tone).
    * Completes immediately with KErrNone if the show has no parts.
    * From CSAnimCtrl.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when all parts of the show have
    * finished.
    */
    virtual void Start( TRequestStatus& aStatus );

protected:

    /**
    * From CCoeControl.
    * Handles pointer events.
    *
    * @since S60 5.0
    *
    * @param aPointerEvent The pointer event.
    */
    virtual void HandlePointerEventL( const TPointerEvent& aPointerEvent );

    /**
    * MMediatorCommandObserver
    * A Mediator Service command.
    *
    * @since S60 5.0
    *
    * @param aDomain    The identifier of the domain.
    * @param aCategory  The identifier of the category.
    * @param aCommandId The identifier of the command.
    * @param aVersion   The version information of the command. Not used.
    * @param aData      The parameters of the command. Not used.
    */
    void MediatorCommandL(
        TUid aDomain,
        TUid aCategory,
        TInt aCommandId,
        TVersion aVersion,
        const TDesC8& aData );

    /**
    * MMediatorCommandObserver
    * Cancel a Mediator Service command.
    * Does nothing.
    *
    * @since S60 5.0
    *
    * @param aDomain    The identifier of the domain. Not used.
    * @param aCategory  The identifier of the category. Not used.
    * @param aCommandId The identifier of the command. Not used.
    */
    void CancelMediatorCommand(
        TUid aDomain,
        TUid aCategory,
        TInt aCommandId );

    /**
    * A response to a Mediator Service command.
    * From MMediatorCommandResponseObserver.
    *
    * @since S60 3.2
    *
    * @param aDomain The identifier of the domain
    * @param aCategory The identifier of the category.
    * @param aCommandId The identifier of the command.
    * @param aStatus Standard Symbian error code indicating the success of the command.
    * @param aData The parameters of the response.
    */
    virtual void CommandResponseL(
        TUid aDomain,
        TUid aCategory,
        TInt aCommandId,
        TInt aStatus,
        const TDesC8& aData );

protected:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    IMPORT_C CSAnimStartupCtrl();

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    *
    * @param aRect Rectangle designated for the control.
    * @param aContainer The compound control that is the container for this control.
    */
    IMPORT_C void ConstructL(
        const TRect& aRect,
        const CCoeControl& aContainer );

    /**
    * Leavable version of Load. Leaves if there is an error reading animation
    * properties.
    *
    * @since S60 3.2
    *
    * @param aParams Central Repository keys for finding out details of the
    * animation.
    * @param aStatus Request to complete when loading is complete.
    */
    virtual void LoadL(
        const TAnimationParams& aParams,
        TRequestStatus& aStatus );

    /**
    * Read a file name value from Central Repository key identified by parameter.
    * Return empty value if the file does not exist or can not be opened.
    *
    * @since S60 3.2
    *
    * @param aFs File server session for checking the existence of the file.
    * @param aRepository Central repository to read the file name from.
    * @param aKey Identifies the key to read.
    * @param aBuf On return, contains a file name.
    */
    virtual void ReadFileNameL(
        RFs& aFs,
        CRepository& aRepository,
        const TUint32 aKey,
        TDes& aBuf );

    /**
    * Read a volume level value from Central Repository key identified by
    * parameter.
    * If device ringing tone is silent in profile settings, return 0.
    * If the volume level is lower than the minimum allowed, return the minimum
    * allowed value.
    * If the volume level is higher than the maximum allowed, return the maximum
    * allowed value.
    * Otherwise return the volume level read from Central Repository.
    *
    * @since S60 3.2
    *
    * @param aRepository Central repository to read the file name from.
    * @param aKey Identifies the key to read.
    * @return Volume level for playing a welcome show tone.
    */
    virtual TInt ReadVolume( CRepository& aRepository, const TUint32 aKey );

    /**
    * Read an integer value from Central Repository key identified by parameter.
    * Return 0 on failure.
    *
    * @since S60 3.2
    *
    * @param aRepository Central repository to read the file name from.
    * @param aKey Identifies the key to read.
    * @return The value from CentralRepository.
    */
    virtual TInt ReadIntValue( CRepository& aRepository, const TUint32 aKey );

    /**
    * Start playing the animation.
    *
    * @since S60 3.2
    */
    virtual void StartAnimation();

private:

    /** Copy constructor. */
    CSAnimStartupCtrl( const CSAnimStartupCtrl& );
    /** Assignment operator. */
    CSAnimStartupCtrl& operator=( const CSAnimStartupCtrl& );

protected: // data

    /**
    * For sending commands to Secondary Display using Mediator.
    * Owned. May be NULL.
    */
    CMediatorCommandInitiator* iCommandInitiator;

    /**
    * For responding to commands from Secondary Display using Mediator.
    * Owned. May be NULL.
    */
    CMediatorCommandResponder* iCommandResponder;

    /** ETrue if default 'beep' tone should be played. */
    TBool iPlayDefaultBeep;

    /**
    * Identifies the message to send to secondary display (if supported) to
    * synchronise the animation.
    */
    TInt iSyncCommand;

    /** ETrue if the animation has any content (image or tone). */
    TBool iHasContent;

    /**
    * Request status stored here until secondary display sync command completes
    * and animation can be started. Not owned. May be NULL.
    */
    TRequestStatus* iClientStatus;

private: // data

    TAny* iReserved1; /** Reserved for future use. */
    TAny* iReserved2; /** Reserved for future use. */

    /**
    * Set to ETrue when sending a sync command to secondary display, and EFalse
    * when a response is received.
    * Responses arriving while the value is EFalse are not handled.
    */
    TBool iWaitingForSyncResponse;

    TInt iReserved4; /** Reserved for future use. */

    };

#endif // SANIMSTARTUPCTRL_H
