/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Includes some common defines used in the Splash Screen
*
*/


#ifndef SPLASHSCREENDEFINES_H
#define SPLASHSCREENDEFINES_H

//CONSTANTS
_LIT(KSplashBitmapName, "Z:QGN_STARTUP_SCREEN.SVG");
_LIT(KPanicMsg,"SplashScreen");

_LIT(KSplashScreenWindowGroup, "S60SplashScreenGroup");
#define END_SPLASHSCREEN_EVENT (EEventUser + 999)

_LIT_SECURITY_POLICY_C1(KReadPolicy, ECapabilityReadDeviceData);
_LIT_SECURITY_POLICY_C1(KWritePolicy, ECapabilityWriteDeviceData);
#define SIZE_X 360
#define SIZE_Y 360

// MACROS
#define TRACE_ADDPREFIX(aText) (TPtrC((const TText *)L"SplashScreen: \"" L##aText L"\""))

#ifdef _DEBUG
#define TRACES(aMsg) RDebug::Print( TRACE_ADDPREFIX(aMsg) )
#define TRACES1(aFormat,aP1) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1) )
#define TRACES2(aFormat,aP1,aP2) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2) )
#define TRACES3(aFormat,aP1,aP2,aP3) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2),(aP3) )
#else
#define TRACES(aMsg)
#define TRACES1(aFormat,aP1)
#define TRACES2(aFormat,aP1,aP2)
#define TRACES3(aFormat,aP1,aP2,aP3)
#endif

const TInt KTriesToConnectServer( 2 );
const TInt KTimeBeforeRetryingServerConnection( 50000 );

#endif      // SPLASHSCREENDEFINES_H
            
// End of File
