/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApShutdownImage class definition. This class
*                implements stuff needed to show Shutdownimage.
*
*/


#ifndef SYSAPSHUTDOWNIMAGE_H
#define SYSAPSHUTDOWNIMAGE_H

// INCLUDES
#include <coecntrl.h>

// CLASS DECLARATION
/**
*  CSysApShutdownImage
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApShutdownImage : public CCoeControl , public MCoeControlObserver
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param CSysApAppUi& aSysApAppUi,
        * @param TUint8 aObjNo
        * @return CSysApPhonetObserver* ( Pointer to the created object )
        */
        static CSysApShutdownImage* NewL();

        /**
        *  Destructor
        */
        ~CSysApShutdownImage();

    public:
        /**
        * Gets called when the shutdown is about the begin. Does the thing.
        * @param None
        * @return void
        */
        void ShowShutdownImageL(TInt aBitmapId);

        RWindow& ShutdownCoeControlWindow();
        
        void SetRectForAnimation();

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Set a sub-control to this control.
        *
        * @since S60 3.2
        */
        void SetComponent( CCoeControl& aComponent );

        /**
        * Remove the sub-control from this control.
        *
        * @since S60 3.2
        */
        void RemoveComponent();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    private:
        /**
        *  C++ default constructor.
        */
        CSysApShutdownImage();

        /**
        *  EPOC default constructor
        */
        void ConstructL();

    private:

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * From CCoeControl.
        *
        * @since S60 3.2
        */
        void SizeChanged();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        * @param None
        * @return void
        */
        TInt CountComponentControls() const;

        /**
        * @param None
        * @return void
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
        * @param None
        * @return void
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

    private: // Functions from base classes

        /**
        * Gets called by HandleSystemAgentEventL to Disable keylock when devicelock is activated.
        * This is done already when device lock is activated because this
        * @param None
        * @return void
        */
        void Draw(const TRect& aRect) const;

        /**
        * Gets called by HandleSystemAgentEventL to Disable keylock when devicelock is activated.
        * This is done already when device lock is activated because this
        * @param None
        * @return void
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

        CFbsBitmap* ReadSVGL (TFileName aFileName);
    private:    // Data

        //Used for showing user selected image
        CFbsBitmap*     iBitmap;

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        // Component control.
        CCoeControl* iComponent;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    };

#endif      // SYSAPSHUTDOWNIMAGE_H

// End of File
