/*
 ============================================================================
 Name		: t_oomtestappAppView.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __T_OOMTESTAPPAPPVIEW_h__
#define __T_OOMTESTAPPAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>

class Ct_oomtestappAppUi;

// CLASS DECLARATION
class Ct_oomtestappAppView : public CCoeControl
	{
public:
	// New methods

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Create a Ct_oomtestappAppView object, which will draw itself to aRect.
	 * @param aRect The rectangle this view will be drawn to.
	 * @return a pointer to the created instance of Ct_oomtestappAppView.
	 */
	static Ct_oomtestappAppView* NewL(const TRect& aRect, Ct_oomtestappAppUi& aAppUi);

	/**
	 * ~Ct_oomtestappAppView
	 * Virtual Destructor.
	 */
	virtual ~Ct_oomtestappAppView();

public:
	// Functions from base classes

	/**
	 * From CCoeControl, Draw
	 * Draw this Ct_oomtestappAppView to the screen.
	 * @param aRect the rectangle of this view that needs updating
	 */
	void Draw(const TRect& aRect) const;

	/**
	 * From CoeControl, SizeChanged.
	 * Called by framework when the view size is changed.
	 */
	virtual void SizeChanged();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 * Perform the second phase construction of a
	 * Ct_oomtestappAppView object.
	 * @param aRect The rectangle this view will be drawn to.
	 */
	void ConstructL(const TRect& aRect);

	/**
	 * Ct_oomtestappAppView.
	 * C++ default constructor.
	 */
	Ct_oomtestappAppView( Ct_oomtestappAppUi& aAppUi );

private:
    Ct_oomtestappAppUi& iAppUi;

    const CFont* iFont; // not own

	};

#endif // __T_OOMTESTAPPAPPVIEW_h__
// End of File
