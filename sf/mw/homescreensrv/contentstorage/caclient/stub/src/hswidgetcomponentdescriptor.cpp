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
* Description:
*
*/


#include "hswidgetcomponentdescriptor.h"


HsWidgetComponentDescriptor::HsWidgetComponentDescriptor():installationPath(),uri(),title(),
	description(),iconUri(),hidden(),serviceXml(),version(),library(),translationFilename(),uid(0),previewImage()
{}

HsWidgetComponentDescriptor::HsWidgetComponentDescriptor(const HsWidgetComponentDescriptor& copyFrom) {
    this->installationPath = copyFrom.installationPath;
    this->uri = copyFrom.uri;
    this->title = copyFrom.title;
    this->description = copyFrom.description;
    this->iconUri = copyFrom.iconUri;
    this->hidden = copyFrom.hidden;
    this->serviceXml = copyFrom.serviceXml;
    this->version = copyFrom.version;
    this->library = copyFrom.library;
    this->translationFilename = copyFrom.translationFilename;
    this->uid = copyFrom.uid;
    this->previewImage = copyFrom.previewImage;

}


bool HsWidgetComponentDescriptor::isValid()
{
    bool ret(false);
    if (version == "1.0") {
        ret = !(uri.isEmpty() || serviceXml.isEmpty() || title.isEmpty()); 
    } else if (version.isEmpty()) {
        ret = !(uri.isEmpty() || title.isEmpty()); 
    }
    return ret;
}

