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
* Description:  CSysApBatteryInfoController class declaration
*
*/


#ifndef SYSAPBATTERYINFOCONTROLLER_H
#define SYSAPBATTERYINFOCONTROLLER_H

#include <e32base.h>

class CBSUtil;
// class CBatteryPopupControl;

/**
 *  Controller for enhanced battery information.
 *
 *  @lib None.
 *  @since S60 5.0
 */
class CSysApBatteryInfoController : public CBase
    {

    /**
    * States for battery information.
    */
    enum TBatteryInfoState
        {
        /**
        * Battery capacity is below defined threshold. 
        * The battery information is displayed every time when
        * the battery capacity decreases.
        */
        EBatteryInfoBelowThreshold,
        
        /**
        * Battery capacity is above defined threshold.
        * The battery information is not shown
        */
        EBatteryInfoAboveThreshold,
        
        /**
        * Battery status is low or empty.
        * The battery information is not shown.
        */
        EBatteryInfoStatusLow
        };

public:    
    /**
    * Symbian two-phase controller.
    *
    * @since S60 5.0
    * @param aThreshold battery capacity percentage after which enhanced battery information
    *                   is displayed.
    */
    static CSysApBatteryInfoController* NewL( TInt aThreshold );

    /**
    * Destructor.
    */
    virtual ~CSysApBatteryInfoController();

    /**
     * Power menu has been displayed.
     * Displays the enhanced battery information.
     *
     * @since S60 5.0
     */
     void PowerMenuShownL();
     
     /**
     * Battery level indicator has been updated.
     * Displays the enhanced battery information if needed.
     *
     * @since S60 5.0
     */
     void BatteryLevelUpdatedL();
     
     /**
     * Battery status has been updated.
     * Stops displaying the battery information even if the battery level
     * is below the threshold value and continues to decrease.
     *
     * @since S60 5.0
     * @param aValue battery status
     *
     * @see KHWRMBatteryStatus
     */
     void BatteryStatusUpdated( const TInt aValue );

private:

    /**
    * C++ default constructor.
    */
    CSysApBatteryInfoController();

    /**
    * Constructor.
    */
    CSysApBatteryInfoController( TInt aThreshold );
    
    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();

    /**
    * Displays the battery pane popup.
    */
    void ShowBatteryPreviewPopupL( TInt aCapacity );

private: // data
    
    /**
    * Battery State utility.
    * Own.
    */
    CBSUtil* iBsUtil;
    
    /**
    * Battery capacity percentage which is currently shown.
    */
    TInt iCurrentCapacity;
    
    /**
    * Battery capacity percentage after which the enhanced battery 
    * information is shown for the first time.
    */
    TInt iThresholdCapacity;
    
    /**
    * Battery info state.
    */
    TBatteryInfoState iBatteryInfoState;
    
    /**
    * Battery popup control.
    * Own.
    */
    // CBatteryPopupControl* iBatteryPopup;
    };


#endif // SYSAPBATTERYINFOCONTROLLER_H
