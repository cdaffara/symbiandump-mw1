/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:         container
*
*/









#ifndef C_BCTESTPANECONTAINER_H
#define C_BCTESTPANECONTAINER_H

#include <coecntrl.h>

class CBCTestPaneCase;

/**
* container class
*/
class CBCTestPaneContainer: public CCoeControl
    {
public: 
    
// constructor and destructor
    
    /**
     * C++ default constructor
     */
    CBCTestPaneContainer();
    
    /**
     * Destructor
     */
    virtual ~CBCTestPaneContainer();
    
    /**
     * Symbian 2nd constructor
     */
    void ConstructL( const TRect& aRect );
    
// new functions
    
    /**
    * Set component control, and container will own the control
    * @param aControl pointer to a control.
    */
    void SetControl( CCoeControl* aControl ); 
    
    /**
     * Delete control
     */
    void ResetControl();
    
    /**
     * Set pane case class.
     */
    void SetPaneCase( CBCTestPaneCase* aCase );

// from CCoeControl

    /**
     * Return count of component controls
     */
    TInt CountComponentControls() const;
    
    /**
     * Return pointer to component control specified by index
     * @param aIndex, a index to specify a component control
     */
    CCoeControl* ComponentControl( TInt aIndex ) const;

private: // from CCoeControl    
    
    /**
    * From CCoeControl, Draw.
    * Fills the window's rectangle.
    * @param aRect Region of the control to be (re)drawn.
    */
    void Draw( const TRect& aRect ) const;  
    
private: // data
    
    /**
     * Pointer to component control.
     * own
     */
    CCoeControl* iControl;
    
    /**
     * Pointer to case.
     * Not own
     */    
    CBCTestPaneCase* iPaneCase;  
    
    };    

#endif // C_BCTESTPANECONTAINER_H