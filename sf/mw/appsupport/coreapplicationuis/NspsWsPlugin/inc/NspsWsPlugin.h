/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CNspsWsPlugin class definition
*
*/


#ifndef NSPSWSPLUGIN_H
#define NSPSWSPLUGIN_H

// INCLUDES
#include <w32adll.h>        // CWindowAnim

#ifdef _DEBUG
#define TRACES(x)    x
#else
#define TRACES(x)
#endif

/**
*  CNspsWsPlugin class definition
*
*  @lib   nspswsplugin
*  @since 1.0
*/

class CNspsWsPlugin: public CWindowAnim
    {
public:
    /** C'tor. */
    CNspsWsPlugin();
    /** D'tor. */
    ~CNspsWsPlugin();

public: // From CAnim
    /** Nothing. Returns always KErrNone.
    *   @see CAnim::CommandReplyL 
    */
    virtual TInt CommandReplyL(TInt aOpcode, TAny *aArgs);

    virtual void Command(TInt aOpcode, TAny *aArgs);
    /** Not needed. Inherited from base class.
    *   @see CAnim::Animate
    */
    virtual void Animate(TDateTime *aDateTime);

public: // From CWindowAnim
    /** Constructor, called by framework.
    *   @see CWindowAnim::ConstructL 
    */
    virtual void ConstructL(TAny *aArgs, TBool aHasFocus);
    /** Not needed. Inherited from base class.
    *   @see CWindowAnim::Redraw
    */
    virtual void Redraw();
    /** Not needed. Inherited from base class.
    *   @see CWindowAnim::FocusChanged
    */
    virtual void FocusChanged(TBool aState);

public: // From MEventHandler
    /** Function is called for every key event.
    *   @see MEventHandler::OfferRawEvent
    */
    virtual TBool OfferRawEvent(const TRawEvent &aRawEvent);

private:
    TBool                 iForwardRawKeyeventsToSysAp;
    TBool                 iForwardRawKeyeventsToNcn;
    TBool                 iForwardRawKeyeventsToLights;
    };

#endif

// End Of File
