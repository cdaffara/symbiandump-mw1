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
* Description:  The user class needs to provider the implementation.
*
*/


#ifndef M_KEFPROVIDER_H
#define M_KEFPROVIDER_H

/**
 *  Provider API for generating raw/key/sound events
 *
 *  The user class needs to provider the implementation.
 *
 *  @lib KeyEventFw.lib
 *  @since S60 v3.2
 */
class MKefProvider
    {

public:

    /**
     * Generates a raw event.
     *
     * @param aRawEvent Event arguments.
     */
    virtual void KefPostRawEvent(const TRawEvent& aRawEvent) const = 0;
    
    /**
     * Generates a key event.
     *
     * @param aKeyEvent Event arguments.
     */
    virtual void KefPostKeyEvent(const TKeyEvent& aKeyEvent) const = 0;
    
    /**
     * Generates a key press sound
     *
     * @param aKey Scan code of the key press. 
     */
    virtual void KefGenerateKeySound( TInt aKey ) = 0;
    
    #ifdef RD_TACTILE_FEEDBACK
    /**
     * Generates tactile feedback for a key press.
     *
     * @param aFeedback Type of feedback to be generated. 
     */
    virtual void KefGenerateFeedback( TUint16 aFeedback ) = 0;
    #endif // RD_TACTILE_FEEDBACK
    };


#endif // M_KEFPROVIDER_H
