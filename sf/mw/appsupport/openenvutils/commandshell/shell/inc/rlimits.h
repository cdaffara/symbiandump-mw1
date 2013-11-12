/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
/** rlimits.h                              **/
/** architecture-customized limits for zsh **/

#define ZSH_NLIMITS 15

static char const *recs[ZSH_NLIMITS] = {
	"cputime",
	"filesize",
	"datasize",
	"stacksize",
	"coredumpsize",
	"memoryuse",
	"maxproc",
	"descriptors",
	"memorylocked",
	"addressspace",
	"maxfilelocks",
	"sigpending",
	"msgqueue",
	"NICE",
	"RTPRIO",
};

static int limtype[ZSH_NLIMITS] = {
	ZLIMTYPE_TIME,
	ZLIMTYPE_MEMORY,
	ZLIMTYPE_MEMORY,
	ZLIMTYPE_MEMORY,
	ZLIMTYPE_MEMORY,
	ZLIMTYPE_MEMORY,
	ZLIMTYPE_NUMBER,
	ZLIMTYPE_NUMBER,
	ZLIMTYPE_MEMORY,
	ZLIMTYPE_MEMORY,
	ZLIMTYPE_NUMBER,
	ZLIMTYPE_NUMBER,
	ZLIMTYPE_NUMBER,
	ZLIMTYPE_UNKNOWN,
	ZLIMTYPE_UNKNOWN,
};
