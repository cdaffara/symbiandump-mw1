/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base class for XIMP API implementations.
*
*/

#ifndef PRESENCEAPIOBJBASE_H
#define PRESENCEAPIOBJBASE_H


/**
 * Presence plugin class implementation UIDs.
 *
 * These do not have to be "real" Symbian UIDs, but such ID's,
 * that they do not overlap with XIMP or Presence Plugin API interface IDs.
 */
 
// 90xx - ximp related
// 91xx - presence related
// 92xx - instant messaging related

// Presence plugin events
#define PRIMP_CLSID_CPRESENTITYEVENTIMP               9100
#define PRIMP_CLSID_COWNPRESENCEEVENTIMP              9101
#define PRIMP_CLSID_CPRESENCEGRANTREQUESTLISTEVENTIMP 9102
#define PRIMP_CLSID_CPRESENTITYGROUPLISTEVENTIMP      9103
#define PRIMP_CLSID_CPRESENTITYPRESENCEEVENTIMP       9104
#define PRIMP_CLSID_CPRESENCEBLOCKLISTEVENTIMP        9105
#define PRIMP_CLSID_CPRESENTITYGROUPCONTENTEVENTIMP   9106
#define PRIMP_CLSID_CPRESENCEWATCHERLISTEVENTIMP      9107
#define PRIMP_CLSID_CPRESENCEGRANTREQUESTINFOIMP      9108


// Presence datamodel classes
#define PRIMP_CLSID_CPRESENCEINFOIMP                  9110
#define PRIMP_CLSID_CPRESENCEINFOFILTERIMP            9111
#define PRIMP_CLSID_CPRESENCEBLOCKINFOIMP             9112
#define PRIMP_CLSID_CPRESENTITYGROUPINFOIMP           9113
#define PRIMP_CLSID_CPRESENTITYGROUPMEMBERINFOIMP     9114
#define PRIMP_CLSID_CPRESENCEWATCHERINFOIMP           9115
#define PRIMP_CLSID_CPRESENCEINFOFIELDIMP             9116
#define PRIMP_CLSID_CPRESENCEINFOFIELDVALUETEXTIMP    9117
#define PRIMP_CLSID_CPRESENCEINFOFIELDVALUEBINARYIMP  9118
#define PRIMP_CLSID_CPRESENCEINFOFIELDVALUEENUMIMP    9119
#define PRIMP_CLSID_CDEVICEPRESENCEINFOIMP            9120
#define PRIMP_CLSID_CPERSONPRESENCEINFOIMP            9121
#define PRIMP_CLSID_CSERVICEPRESENCEINFOIMP           9122
#define PRIMP_CLSID_CPRESENCEINFOFIELDCOLLECTIONIMP   9123
#define PRIMP_CLSID_CPRESENCEDATACACHE                9124


// Presence plugin host environment classes
#define PRIMP_CLSID_CPROTOCOLPRESENTITYGROUPSDATAHOST      9130
#define PRIMP_CLSID_CPROTOCOLPRESENCEPUBLISHINGDATAHOST    9131
#define PRIMP_CLSID_CPROTOCOLPRESENCEWATCHINGDATAHOST      9132
#define PRIMP_CLSID_CPROTOCOLPRESENCEAUTHORIZATIONDATAHOST 9133
#define PRIMP_CLSID_CPROTOCOLPRESENCEDATAHOSTIMP           9134


// Presence Management classes
#define PRIMP_CLSID_CPRESENCEPUBLISHINGIMP            9140
#define PRIMP_CLSID_CPRESENTITYGROUPSIMP              9141
#define PRIMP_CLSID_CPRESENCEWATCHINGIMP              9142
#define PRIMP_CLSID_CPRESENCEAUTHORIZATIONIMP         9143
#define PRIMP_CLSID_CPRESENCEFEATURESIMP              9144
#define PRIMP_CLSID_CPRESENCEOBJECTFACTORYIMP         9145

// Presence Cache classes
#define PRIMP_CLSID_CXIMPPRESENCEBUDDYINFOIMP           9150
#define PRIMP_CLSID_CXIMPPRESENCEBUDDYINFOLIST          9151
#define PRIMP_CLSID_CPRESENCECACHECLIENT                9152




#endif // PRESENCEAPIOBJBASE_H
