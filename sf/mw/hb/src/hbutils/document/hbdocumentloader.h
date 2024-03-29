/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbUtils module of the UI Extensions for Mobile.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at developer.feedback@nokia.com.
**
****************************************************************************/

#ifndef HBDOCUMENTLOADER_H
#define HBDOCUMENTLOADER_H

#include <hbglobal.h>

#include <QList>
#include <QString>
#include <QObject>

QT_BEGIN_NAMESPACE
class QObject;
class QGraphicsWidget;
class QIODevice;
QT_END_NAMESPACE
class HbMainWindow;
class HbDocumentLoaderPrivate;

class HB_UTILS_EXPORT HbDocumentLoader
{
public:
    HbDocumentLoader();
    HbDocumentLoader(const HbMainWindow *window);
    virtual ~HbDocumentLoader();

    QObjectList load( const QString &fileName, const QString &section , bool *ok = 0 );
    QObjectList load( QIODevice *device, const QString &section, bool *ok = 0 );

    QObjectList load( const QString &fileName, bool *ok = 0 );
    QObjectList load( QIODevice *device, bool *ok = 0 );
    
    bool createBinary( QIODevice *srcDevice, QIODevice *dstDevice );

    QGraphicsWidget *findWidget(const QString &name) const;
    QObject *findObject(const QString &name) const;
    
    bool setObjectTree( QObjectList roots );
    
    void reset();
    
    static QString version();
	
protected:
    virtual QObject *createObject(const QString& type, const QString &name);

private:
    HbDocumentLoaderPrivate * const d_ptr;

    Q_DISABLE_COPY(HbDocumentLoader)
    Q_DECLARE_PRIVATE_D(d_ptr, HbDocumentLoader)
};

#endif // HBDOCUMENTLOADER_H
