/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  volume_control_api
 *
*/

#ifndef C_TESTSDKVOLUMECONTAINER_H
#define C_TESTSDKVOLUMECONTAINER_H
#include <coecntrl.h>
    /**
     * container class
     */
class CVolumeControlContainer : public CCoeControl
    {
public:
    // constructors and destructor
    CVolumeControlContainer();

    /**
     * Symbian 2nd constructor
     */
    void ConstructL( const TRect& aRect );
    /**
     * Destructor
     */
    virtual ~CVolumeControlContainer();

public:
    // new functions

    /**
     * Set component control, and container will own the control
     * @param aControl pointer to a control.
     */
    void SetControlL( CCoeControl* aControl );

    /**
     * Delete control
     */
    void ResetControl();

    /**
     * get iCoeEnv
     */
    CCoeEnv* GetCoeEnv();
    // from CCoeControl

    /**
     * Return count of component controls
     */
    TInt CountComponentControls() const;

    /**
     * Return pointer to component control specified by index
     * @param aIndex, a index to specify a component control
     */
    CCoeControl* ComponentControl(TInt aIndex) const;

private:

    // from CCoeControl

    /**
     * From CCoeControl, Draw.
     * Fills the window's rectangle.
     * @param aRect Region of the control to be (re)drawn.
     */
    void Draw(const TRect& aRect) const;

private:
    // data

    /**
     * Pointer to component control.
     * own
     */
    CCoeControl* iControl;

    };

#endif /*C_TESTSDKVOLUMECONTAINER_H*/
