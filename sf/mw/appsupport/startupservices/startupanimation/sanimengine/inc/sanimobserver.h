/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of MSAnimObserver class
*
*/


#ifndef SANIMOBSERVER_H
#define SANIMOBSERVER_H

#include <e32def.h>

class CFbsBitmap;

/**
*  Observer interface for start-up animation events.
*
*  @lib None
*  @since S60 3.2
*/
class MSAnimObserver
    {

public:

    /**
    * Draw an animation frame on the screen with a mask.
    *
    * @since S60 3.2
    *
    * @param aFrame Contains the animation frame to draw on the screen.
    * @param aMask Contains mask for the animation frame to draw on the screen.
    */
    virtual void UpdateScreen( const CFbsBitmap& aFrame, const CFbsBitmap& aMask ) = 0;

    /**
    * Draw an animation frame on the screen without a mask.
    *
    * @since S60 3.2
    *
    * @param aFrame Contains the animation frame to draw on the screen.
    */
    virtual void UpdateScreen( const CFbsBitmap& aFrame ) = 0;

    };


#endif // SANIMOBSERVER_H
