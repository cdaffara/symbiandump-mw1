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
 * Description: declaration of functions providing string constants
 *
 */

#ifndef HOMESCREEN_DOMAIN_PS_KEYS_H
#define HOMESCREEN_DOMAIN_PS_KEYS_H


/*!
    \file homescreendomainpskeys.h
    \brief 
*/

/*!
    P&S Key path for all Home Screen information keys provided by this API.
*/
const char HsStatePSKeyPath[] = "/homescreen";

/*!
    P&S Key sub path to represent the state of the Home Screen. This property is updated when
    Home screen state is changed. Valid values are defined by the enum EPSHomescreenState.
    
    \see EPSHomescreenState
*/
const char HsStatePSKeySubPath[] = "state";

#if defined(__SYMBIAN32__) || defined(SYMBIAN) 
/*!
    Home Screen P&S category Uid for pure Symbian usage.
*/
const TUid KHsCategoryUid = { 0x20022F35 };

/*!
    P&S Key to represent the state of the Home Screen for pure Symbian usage.
    This property is updated when Home screen state is changed. Valid values
    are defined by the enum EPSHomescreenState.
*/
const TUint KHsCategoryStateKey = 0x1;
#endif //SYMBIAN
    
/*!
    Home screen state.
    As Homescreen and Application Library are in the same process
    EHomeScreenApplicationBackground and EHomeScreenApplicationForeground
    information considers both of them simultaneously.
*/
enum EPSHomescreenState
     {
     /**
     Invalid state. P&S key not initialized.
     */
     EHomeScreenInvalid = 0x0,
     /**
     Home Screen Application in background. 
     */
     EHomeScreenApplicationBackground = 0x1,
     /**
     Home Screen Application in foreground.
     */
     EHomeScreenApplicationForeground = 0x2,
     /**
     Home Screen in widget view.
     */
     EHomeScreenWidgetViewActive = 0x4,
     /**
     Home Screen in application library view.
     */
     EHomeScreenApplicationLibraryViewActive = 0x8,
     /**
     Home Screen in widget view in foreground.
     */
     EHomeScreenWidgetViewForeground = EHomeScreenApplicationForeground | EHomeScreenWidgetViewActive,
     /**
     Home Screen in application library view in foreground.
     */
     EHomeScreenApplicationLibraryViewForeground = EHomeScreenApplicationForeground | EHomeScreenApplicationLibraryViewActive 
     };      

/*!
    Home screen state.

    \deprecated EPSHsState
        is deprecated. Please use EPSHomescreenState instead.

    \see EPSHomescreenState
*/
enum EPSHsState
     {
     /**
     Invalid state. P&S key not initialized.
     */
     EHomeScreenInvalidState = 0,
     /**
     Home Screen inactive
     */
     EHomeScreenInactive = 1,
     /**
     Home Screen in idle state
     */
     EHomeScreenIdleState   = 2
     };

#endif //HOMESCREEN_DOMAIN_PS_KEYS_H
