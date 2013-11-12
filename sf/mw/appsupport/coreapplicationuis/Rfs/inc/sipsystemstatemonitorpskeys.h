/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Name          : sipsystemstatemonitorpskeys.h
* Part of       : RFs
*
*/

#ifndef SIPSYSTEMSTATEMONITORPSKEYS_H
#define SIPSYSTEMSTATEMONITORPSKEYS_H

// CLASS DECLARATION
/**
* @publishedPartner
* @released
*/

static const TUid KPSSipRfsUid = { 0x101F413C };

const TUint32 KSipRfsState = 0x00000001;

enum TSipRfsState
    {
    ESipRfsStarted = 0,
    ESipRfsFailed,
    ESipRfsCompleted,
    ESipRfsEventProcessingCompleted
    };

#endif // SIPSYSTEMSTATEMONITORPSKEYS_H
