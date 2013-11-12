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
* Description:  HsWidget provider manifest.
*
*/


#ifndef HSWIDGETPROVIDERMANIFEST_H
#define HSWIDGETPROVIDERMANIFEST_H

#include <QObject>
#include <hswidgetmodel_global.h>
#include <hsiwidgetprovider.h>


class HsWidgetProviderManifestPrivate;



class HSWIDGETMODEL_EXPORT HsWidgetProviderManifest : public QObject
{
    Q_OBJECT

public:

    /**
     * Constructor. Loads widgets as widget tokens from the given
     * manifest file.
     * @since S60 ?S60_version.     
     * @param aParent Parent object.
     */
    HsWidgetProviderManifest(QObject *parent = 0);
    
    /**
     * Destructor.
     * @since S60 ?S60_version.
     */
    virtual ~HsWidgetProviderManifest();


public:

    /**
     * Returns loaded widgets as widget tokens.
     * @since S60 ?S60_version.
     * @return Loaded widgets as tokens.
     */
    QList<HsWidgetToken> widgets() const;
	
    /**
     * Tells if the widget tokens must be asked from plugin
     * instead of manifest file.
     * @since S60 ?S60_version.
     * @return True if loading required, false otherwise.
     */
    bool loadOnQuery() const;

    /**
     * Loads widgets as widget tokens from a manifest file.
     * @since S60 ?S60_version.
     * @param aFileName Manifest file name.
     * @return true if load was succesfull, false otherwise
     */
	bool loadFromXml(const QString &fileName);

private:

	Q_DISABLE_COPY(HsWidgetProviderManifest)

    HsWidgetProviderManifestPrivate *mD;
    friend class HsWidgetProviderManifestPrivate;

};




#endif
