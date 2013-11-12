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
* Description:  Homescreen widget preference service.
*
*/


#ifndef HSIWIDGETPREFERENCESERVICE_H
#define HSIWIDGETPREFERENCESERVICE_H

class QString;
class HsWidget;

/*DEPRECATED
 * @ingroup group_hswidgetmodel group_widget_api_candidates
 * @brief Preference store for widget.
 *
 * The preference service is a key/value store with both the key and
 * the value being strings. The widget may store data to be reused
 * at a later date, such as a username, settings for the widget, persistent data,
 * and so on. The data is stored on the device, for example on its hard drive,
 * and can be read back at any time.
 *
 * @lib ?library
 * @since S60 ?S60_version
 */
class IHsWidgetPreferenceService
{
public:

	/**
     * Save a widget preference.
	 * This method will store the given value in the widget
	 * in the widget preference store for the given key.
	 *
	 * The preference store is unique to this widget and any values
	 * stored in it are persisted if the widget is reloaded or closed
	 * and started again.
	 *
	 * If a value for the given key already exists, it is silently
	 * overwritten. Storing an empty string for the given key will
	 * create an entry for the given key with an empty string as its value.
	 *
	 * @param widget HsWidget instance.
     * @param key The preference to get from the preference store.
     * @param value The value of the preference.
	 * @return True on success, false otherwise.
	 */
	virtual bool setPreferenceForKey(HsWidget *widget, const QString &key, const QString &value)=0;

	/**
	 * Get a preference from the widget preference store.
	 * Get the value stored for the given key in the widget preference store.
	 * If there is no value for the given key, an empty string is returned.
	 *
	 * @param widget HsWidget instance.
     * @param key The preference to get from the preference store.
     * @param value The value of the preference.
	 * @return True on success, false otherwise.
	 */
	virtual bool preferenceForKey(HsWidget *widget, const QString &key, QString &value)=0;

};

Q_DECLARE_METATYPE(IHsWidgetPreferenceService*)
#endif
