/**
* Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __CONSOLEALARMALERTCONSOLE_H__
#define __CONSOLEALARMALERTCONSOLE_H__

// System includes
#include <e32base.h>
#include <calprogresscallback.h>

// User includes
#include <asaltdefs.h>

// Type definitions

// Constants

// Classes referenced
class CConsoleAlarmAlertSession;

// Enumerations

//Helper Class
// The following class is only needed to allow the establishment of the proper
// CalInterimApi classes used to verify that the associated data from an
// alarm created through the CalInterimApi is able to find the relevant CCalEntry
// for the alarm
class CCAASHelperCallBack : public CBase, public MCalProgressCallBack
	{
	public:
	
	CCAASHelperCallBack();

	/** Progress callback.
	
	This calls the observing class with the percentage complete of the current operation.
	
	@param aPercentageCompleted The percentage complete. */
	void Progress(TInt aPercentageCompleted);
	
	/** Completion callback.
	
	This calls the observing class when the current operation is finished.
	
	@param aError The error if the operation failed, or KErrNone if successful. */
	void Completed(TInt aError);
	
	/** Asks the observing class whether progress callbacks are required.
	
	@return If the observing class returns EFalse, then the Progress() function will not be called. */
	TBool NotifyProgress();

	};

///////////////////////////////////////////////////////////////////////////////////////
// ----> CConsoleAlarmAlertConsole (header)
///////////////////////////////////////////////////////////////////////////////////////
class CConsoleAlarmAlertConsole : public CActive
	{
///////////////////////////////////////////////////////////////////////////////////////
public:										// CONSTRUCT / DESTRUCT
///////////////////////////////////////////////////////////////////////////////////////
	CConsoleAlarmAlertConsole(CConsoleAlarmAlertSession& aSession);
	~CConsoleAlarmAlertConsole();

///////////////////////////////////////////////////////////////////////////////////////
public:										// ACCESS
///////////////////////////////////////////////////////////////////////////////////////
	void									GetKey();
	void									CancelKey();
	void									DecCounterL();
	void									UpdateDisplayL();
	TInt									GetTimeInterval() const;
	void									SetVisibilityL(TInt aVis);
	void									SetAlertStateL(TInt aFlags);

///////////////////////////////////////////////////////////////////////////////////////
protected:									// FROM CActive
///////////////////////////////////////////////////////////////////////////////////////
	void									RunL();
	void									DoCancel();

///////////////////////////////////////////////////////////////////////////////////////
protected:									// INTERNAL METHODS
///////////////////////////////////////////////////////////////////////////////////////
	void									Hide();
	void									CreateWinL();
	void									DoDeferTime(TASAltAlertServerResponse aResponse);

	static TBool							CountDownCallBackL(TAny* aSelf);

///////////////////////////////////////////////////////////////////////////////////////
private:									// MEMBER DATA
///////////////////////////////////////////////////////////////////////////////////////

	TInt									iAlertState;

	TInt									iTimeInterval;

	CPeriodic*								iCountDown;

	CConsoleBase*							iWin;

	CConsoleAlarmAlertSession&				iSession;

	friend class							CConsoleAlarmAlertSession;

	TInt									iInstructionIndex;
	};

#endif
