// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __CONSOLEANTESTCLIENT_H__
#define __CONSOLEANTESTCLIENT_H__

#include <asshdalarm.h>
#include <asshddefs.h>

enum TASAltTestOpCode
    {
    EASAltTestOpCodeAcknowledgeAlarm = 0x1000,
	EASAltTestOpCodeGetAttachment,
	EASAltTestOpCodeGetIsNotifying,
	EASAltTestOpCodeSetExtendedMode,
	EASAltTestOpCodeUnsetExtendedMode,		
	EASAltTestOpCodeSetUserTime,
	EASAltTestOpCodeSetMaxAlarms,
	EASAltTestOpCodeNotifyOnAlarm,
	EASAltTestOpCodeNotifyOnSoundStart,
	EASAltTestOpCodeNotifyOnSoundStop,
	EASAltTestOpCodeNotifyOnVisible,
	EASAltTestOpCodeNotifyOnState,
	EASAltTestOpCodeResponseSnoozeAlarm,
	EASAltTestOpCodeResponseSilenceAlarm,
	EASAltTestOpCodeResponseAcknowledgeAlarm,
	EASAltTestOpCodeResponsePauseSound,
	EASAltTestOpCodeCancelNotifications,
	EASAltTestOpCodeResponseAcknowledgeAll,
	EASAltTestOpCodeNotifyOnDelete,
	};

/**
Session with the Console Alarm Alert Server. The Console Alarm Alert Server must be running before using this class.
It provides methods program the behaviour of the Console Alarm Alert Server and wait for events in the Console Alarm Alert Server.
Usage example:

@code

RANTestClient client;
User::LeaveIfError(client.Connect());
CleanupClosePushL(client);

User::LeaveIfError(client.SetExtendedMode());

TASShdAlarm alarm;
TRequestStatus status;
User::LeaveIfError(client.NotifyOnAlarm(alarm, status));

User::WaitForRequest(status);

User::LeaveIfError(client.UnsetExtendedMode());
CleanupStack::PopAndDestroy();

@endcode

@internalTechnology
@test
*/

class RANTestClient: public RSessionBase
	{
public:
	IMPORT_C RANTestClient();
	IMPORT_C TInt Connect();
	IMPORT_C void AcknowledgeAlarm();
	IMPORT_C void GetAttachment(TDes8& buf);
	IMPORT_C TBool IsNotifying();
	
	IMPORT_C TInt SetExtendedMode() const;
	IMPORT_C TInt UnsetExtendedMode() const;
	IMPORT_C TInt SetUserTime(const TTime& aTime) const;
	IMPORT_C TInt SetMaxAlarms(const TInt aMax) const;
	
	IMPORT_C TInt NotifyOnAlarm(TASShdAlarm& aAlarm, TFullName& aFullname, TDes8& aSink, TRequestStatus& aStatus);
	IMPORT_C TInt NotifyOnAlarm(TASShdAlarm& aAlarm, TFullName& aFullname, TRequestStatus& aStatus);
	IMPORT_C TInt NotifyOnAlarm(TASShdAlarm& aAlarm, TRequestStatus& aStatus);
	
	IMPORT_C TInt NotifyOnSoundStart(TAlarmId& aId, TDes& aFilename, TRequestStatus& aStatus);
	IMPORT_C TInt NotifyOnSoundStop(TAlarmId& aId, TRequestStatus& aStatus);
	IMPORT_C TInt NotifyOnVisible(TAlarmId& aId, TBool& aVisible, TRequestStatus& aStatus);
	IMPORT_C TInt NotifyOnState(TAlarmId& aId, TInt& aState, TRequestStatus& aStatus);
	IMPORT_C TInt NotifyOnDelete(TAlarmId& aId, TRequestStatus& aStatus);
	
	IMPORT_C TInt SnoozeAlarm(const TAlarmId& aId, const TTime& aTime) const;
	IMPORT_C TInt SilenceAlarm(const TAlarmId& aId) const;
	IMPORT_C TInt AcknowledgeAlarm(const TAlarmId& aId) const;
	IMPORT_C TInt AcknowledgeAll() const;
	IMPORT_C TInt PauseSoundForAlarm(const TAlarmId& aId, const TTime& aTime) const;
	
	IMPORT_C TInt CancelNotifications();
	
private:
	TPtr8 iAlarmPtr;
	TPtr8 iSoundStartIdPtr;
	TPtr8 iSoundStopIdPtr;
	TPtr8 iVisibleIdPtr;
	TPtr8 iVisibleBoolPtr;
	TPtr8 iStateIdPtr;
	TPtr8 iStateIntPtr;
	TPtr8 iDeleteIdPtr;
	};


#endif //__CONSOLEANTESTCLIENT_H__
