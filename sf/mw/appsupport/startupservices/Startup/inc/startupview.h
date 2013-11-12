/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CStartupView class
*
*/


#ifndef STARTUPVIEW_H
#define STARTUPVIEW_H

#include <coecntrl.h>


/**
*  Main view for the Startup application.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CStartupView ) : public CCoeControl
    {

public:

    /**
    * Constructs a CStartupView object.
    *
    * @param aRect The rectangle that the control should occupy.
    * @return The new object
    */
    static CStartupView* NewL( const TRect& aRect );

    /**
    * Destructor.
    */
    virtual ~CStartupView();

    /**
    * Set a sub-control to this control.
    */
    void SetComponent( CCoeControl& aComponent );

    /**
    * Remove the sub-control from this control.
    */
    void RemoveComponent();

private:

    /**
    * From CCoeControl.
    */
    void SizeChanged();

    /**
    * From CCoeControl.
    * Return the count of the components in the container.
    *
    * @return The number of contained controls.
    */
    TInt CountComponentControls() const;

    /**
    * From CCoeControl.
    * Return a contained control by index.
    *
    * @return The contained control by index. Null if there is no contained
    * control for that index.
    */
    CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
    * From CCoeControl.
    * Draw the component on the screen.
    *
    * @param aRect The rectangular region of the control to be drawn.
    */
    void Draw( const TRect& aRect ) const;

private:

    /**
    * First phase constructor.
    */
    CStartupView();

    /**
    * Second phase constructor.
    *
    * @param aRect The rectangle that the control should occupy.
    */
    void ConstructL( const TRect& aRect );

private:

    /** Component control. */
    CCoeControl* iComponent;

 

    };

#endif // STARTUPVIEW_H
