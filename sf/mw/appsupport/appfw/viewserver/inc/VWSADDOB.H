// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __VWSADDOB_H__
#define __VWSADDOB_H__

/**
 * The MVwsViewAdditionObserver class specifies an interface for making callbacks when a view is added to
 * the server. This enables server event processing which relies on applications being started and new views
 * being added.
 *
 * @internalComponent
 */ 
class MVwsViewAdditionObserver
	{
public:
	virtual void HandleViewAdded(const TVwsViewId& aViewId, const TThreadId& aNewViewClientThreadId)=0;
	};

/*
 * MVwsDeactivationObserver.
 *
 * @internalComponent
 */ 
class MVwsDeactivationObserver
	{
public:
	virtual void HandleDeactivationL(const TVwsViewId& aDeactivatedViewId, const TVwsViewId& aActivatedViewId)=0;
	};

/*
 * MVwsActivationObserver.
 *
 * @internalComponent
 */ 
class MVwsActivationObserver
	{
public:
	virtual void HandleActivationL(const TVwsViewId& aActivatedViewId, const TVwsViewId& ViewIdToBeDeactivated)=0;
	};

#endif
