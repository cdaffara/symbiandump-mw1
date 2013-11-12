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
* Description:  Preference store interface.
*
*/

#ifndef HSIPREFERENCESTORE_H
#define HSIPREFERENCESTORE_H

class QString;


/*DEPRECATED
    \class IHsPreferenceStore
    \brief Interface class for preference store.

    The preference store is a key/value store with both the key and
    the value being strings. The widget may store data to be reused
    at a later date, such as a username, settings for the widget, persistent data,
    and so on. The data is stored on the device, for example on its hard drive,
    and can be read back at any time.
*/
class IHsPreferenceStore
{

public:

/*!
    Virtual destructor.
*/
    virtual ~IHsPreferenceStore() {};

/*!
    Save a widget preference.
    This method will store the given value in the widget
    in the widget preference store for the given key.

    The preference store is unique to this widget and any values
    stored in it are persisted if the widget is reloaded or closed
    and started again.

    If a value for the given key already exists, it is silently
    overwritten. Storing an empty string for the given key will
    create an entry for the given key with an empty string as its value.

    \a id HsWidget's unique identifier
    \a key The string to save the preference under.
    \a value The value of the preference.     
*/
    virtual bool setPreferenceForKey(int id, const QString &key, const QString &value) = 0;

/*!
    Get a preference from the widget preference store.
    Get the value stored for the given key in the widget preference store.
    If there is no value for the given key, an empty string is returned.

    \a id HsWidget's unique identifier
    \a key The preference to get from the preference store.    
    \a value The value of the preference is written to this object.
*/
    virtual bool preferenceForKey(int id, const QString &key, QString &value) = 0;

};

#endif
