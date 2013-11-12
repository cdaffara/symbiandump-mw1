/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef SENSORDATACOMPENSATORPLGTESTTYPES_H
#define SENSORDATACOMPENSATORPLGTESTTYPES_H

/**
* Test Uid for simulating states like orientations, flip etc.
*/
const TUid KTestPSUid = {0x0AA01554};

/**
* Value used for resetting ETestKeyState -> same test case
* can be run sequentially several times. Must be in synch 
* with psvalue assigned for SetUndefined in 101FB3E7.xml. 
*/
const TInt KTestValueUndefined = -1;

/**
* Test states
*/
enum TTestPSIntKeys
    {
    ETestKeyStateNotUsed = 0,
    ETestKeyState,
    ETestIntKeyCount
    };

const TInt KTestMaxDataSize = 512;

enum TTestPSDes8Keys
    {
    ETestKeyPluginDataAppend = ETestIntKeyCount + 1,
    ETestDes8KeyCount
    };

/**
* Test data streams
*/
enum TTestDataStreams
    {
    ETestInputDataItem = 0,
    ETestOutputDataItem,
    ETestPluginDataItem
    };

#endif // SENSORDATACOMPENSATORPLGTESTTYPES_H

// End of File
