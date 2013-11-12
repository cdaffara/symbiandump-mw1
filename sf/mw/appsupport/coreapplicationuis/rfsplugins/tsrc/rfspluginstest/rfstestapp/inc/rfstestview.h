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
* Description:  Declaration of CRfsTestView class
*
*/


#ifndef RFSTESTVIEW_H
#define RFSTESTVIEW_H

#include <coecntrl.h>

/**
*  View class for RfsTestApp application.
*
*  @since S60 S60 5.1
*/
class CRfsTestView : public CCoeControl
    {

public:

    /**
    * Constructs a CRfsTestView object.
    *
    * @param aRect Frame rectangle for control.
    * @return The new object
    */
    static CRfsTestView* NewL( const TRect& aRect );

    /**
    * Destructor.
    */
    virtual ~CRfsTestView();

private:

    /**
    * First phase constructor.
    */
    CRfsTestView();

    /**
    * Second phase constructor.
    *
    * @param aRect Frame rectangle for control.
    */
    void ConstructL( const TRect& aRect );

private: // From CCoeControl

    /**
    * @see CCoeControl
    */
    TInt CountComponentControls() const;

    /**
    * @see CCoeControl
    */
    CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
    * @see CCoeControl
    */
    void Draw( const TRect& aRect ) const;

private: // data

    };

#endif // RFSTESTVIEW_H
