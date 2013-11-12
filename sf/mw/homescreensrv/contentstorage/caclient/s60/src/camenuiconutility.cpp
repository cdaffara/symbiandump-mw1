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

// conversion
#include <XQConversions>
// apparc
#include <apparc.h>
#include <apgcli.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaidpartner.h>
#else
#include <apaid.h>
#endif
// cfbsbitmap
#include <apgicnfl.h>

#include <rsatsession.h>
#include <tsaticoninfo.h>
#include <rsatservice.h>// MCL

#include <e32property.h>
#include <bitdev.h>

#include <AknIconUtils.h>


#include <HbIcon>

#include "ca2internalCRkeys.h"
#include "camenuiconutility.h"
#include "caentry.h"
#include "caquery.h"
#include "caservice.h"
#include "cadef.h"

const char appUidAttributeName[] = "application:uid";

LOCAL_C void getIconFromFile(HbIcon& icon, const QSizeF &size)
{
    if ((icon.iconName().contains(QChar('.')) ||
        (icon.iconName().contains(QChar('/'))) ||
        (icon.iconName().contains(QChar('\\'))) )
        && !icon.isNull() && icon.size().isValid()) {
        icon.setSize(size);
        icon = HbIcon(QIcon(icon.pixmap()));
    }    
}

/*!
 Convert from bitmap and mask to qpixmap
 \param input bitmap.
 \param pixmap to prepare.
 \retval void.
 */
LOCAL_C void fromBitmapAndMaskToPixmap(CFbsBitmap* fbsBitmap,
        CFbsBitmap* fbsMask, QPixmap& pixmap)
{
    pixmap = pixmap.fromSymbianCFbsBitmap(fbsBitmap);
    QPixmap mask;
    mask = mask.fromSymbianCFbsBitmap(fbsMask);
    pixmap.setAlphaChannel(mask);
}

/*!
 Get icon from entry.
 \param entry const reference to CaEntry.
 \retval icon.
 */
LOCAL_C HbIcon getIconFromEntry(const CaEntry& entry, const QSizeF &size)
{
    HbIcon icon;
    QString skinId(entry.iconDescription().skinId());
    if (!skinId.isEmpty()) {
        icon = HbIcon(skinId);
        getIconFromFile(icon, size);
    }
    
    if (icon.isNull() || !(icon.size().isValid())) {
        QString fileName(entry.iconDescription().filename());
        if (!fileName.isEmpty()) {
            icon = HbIcon(fileName);
            getIconFromFile(icon, size);
        }
    }
    return icon;
}

/*!
 Get icon from apparc.
 \param entry const reference to CaEntry.
 \param sie const reference to icon size.
 \retval icon.
 */
LOCAL_C HbIcon getIconFromApparcL(int uidValue, const QSizeF &size)
{
    HbIcon icon;
    
    RApaLsSession apaLsSession;
    CleanupClosePushL(apaLsSession);
    User::LeaveIfError(apaLsSession.Connect());

    TUid uid = uid.Uid(uidValue);
    CApaAppServiceInfoArray* skinArray(NULL);
    TRAPD( err, skinArray =
        apaLsSession.GetAppServiceOpaqueDataLC(uid, TUid::Uid(0x2002DCF3));
        if (err == KErrNone && skinArray ) {
            TArray<TApaAppServiceInfo> tmpArray( skinArray->Array() );
            if ( tmpArray.Count() ) {
                TPtrC8 opaque(tmpArray[0].OpaqueData());
                const TPtrC16 iconName((TText16*) opaque.Ptr(),(opaque.Length()+1)>>1);
                icon = HbIcon( XQConversions:: s60DescToQString( iconName ) );
            }
        }
        CleanupStack::PopAndDestroy(skinArray);
        );
    
    if (icon.isNull() || !(icon.size().isValid())) {
        TSize iconSize(size.width(), size.height());
        CApaMaskedBitmap* apaMaskedBitmap = CApaMaskedBitmap::NewLC();
        TInt err = apaLsSession.GetAppIcon(uid,iconSize,*apaMaskedBitmap);
        
        TInt iconsCount(0);
        apaLsSession.NumberOfOwnDefinedIcons(uid, iconsCount);
        
        QPixmap pixmap;
        if ((err == KErrNone) && (iconsCount > 0)) {
            fromBitmapAndMaskToPixmap(apaMaskedBitmap,
                    apaMaskedBitmap->Mask(), pixmap);
    
            pixmap = pixmap.scaled(size.toSize(), 
                Qt::KeepAspectRatioByExpanding);
            icon = HbIcon(QIcon(pixmap));
        } else {
            HBufC* fileNameFromApparc = NULL;
            TInt err2 = apaLsSession.GetAppIcon(uid,fileNameFromApparc);
            CleanupStack::PushL(fileNameFromApparc);
            if (err2 == KErrNone) {
                QString fileName = XQConversions::s60DescToQString(
                        fileNameFromApparc->Des());
                if (fileName.endsWith(QLatin1String(".mif"),
                        Qt::CaseInsensitive) ||
                    fileName.endsWith(QLatin1String(".mbm"),
                        Qt::CaseInsensitive)) {
                    icon = HbIcon(QIcon(fileName));
                    // Icon should be valid.
                    icon.setSize(QSizeF(0.0, 0.0));
                } else {
                    icon = HbIcon(fileName);
                }
            }
            CleanupStack::PopAndDestroy(fileNameFromApparc);
        }
        CleanupStack::PopAndDestroy(apaMaskedBitmap);
    }
    CleanupStack::PopAndDestroy(&apaLsSession);
    return icon;
}

/*!
 Get default icon from theme.
 \param entry const reference to CaEntry.
 \retval icon.
 */
LOCAL_C HbIcon getDefaultIcon(const CaEntry& entry)
{
    HbIcon icon;
    
    if (entry.entryTypeName() == 
            XQConversions::s60DescToQString(KCaTypeApp)) {
        icon = HbIcon("qtg_large_application");
    } else if (entry.entryTypeName() ==
            XQConversions::s60DescToQString(KCaTypeFolder) ||
            entry.entryTypeName() == 
            XQConversions::s60DescToQString(KCaTypeCollection) ||
            entry.entryTypeName() == 
            XQConversions::s60DescToQString(KCaTypeCollectionDownload)) {
        icon = HbIcon("qtg_large_applications_user");
    } else if (entry.entryTypeName() ==
        XQConversions::s60DescToQString(KCaTypeUrl)) {
        icon = HbIcon("qtg_large_application");
    } else if (entry.entryTypeName() ==
        XQConversions::s60DescToQString(KCaTypeWidget)) {
        icon = HbIcon("qtg_large_application");
    } else if (entry.entryTypeName() ==
        XQConversions::s60DescToQString(KCaTypeTemplatedApp)) {
        icon = HbIcon("qtg_large_application");
    } else if (entry.entryTypeName() ==
        XQConversions::s60DescToQString(KCaTypePackage)) {
        QString type = entry.attribute(XQConversions::s60DescToQString(KCaAttrAppType));
        if (type == 
                XQConversions::s60DescToQString(KCaAttrAppTypeValueJava)) {
            icon = HbIcon("qtg_large_java");
        } else if (type == 
                XQConversions::s60DescToQString(KCaAttrAppTypeValueCWRT)) {
            icon = HbIcon("qtg_large_widget");
        } else {
            icon = HbIcon("qtg_large_sisx");
        }        
    }
    
    return icon;
}

/*!
 Get entry by uid.
 \param uid application uid.
 \retval entry.
 */
LOCAL_C CaEntry getEntryByUid(int uid)
{
    CaEntry entry;
    CaQuery query;
    query.setAttribute(QString("application:uid"), QString::number(uid));
    QList< QSharedPointer<CaEntry> > entries = CaService::instance()->getEntries(query);
    if (!entries.isEmpty()) {
        entry = *(entries.first());
    }
    return entry;
}

/*!
 Get icon when uid is defined.
 \param entry const reference to CaEntry.
 \param sie const reference to icon size.
 \retval icon.
 */
HbIcon CaMenuIconUtility::getApplicationIcon(int uid,
        const QSizeF &size)
{
    HbIcon icon;
    CaEntry entry;
    entry = getEntryByUid(uid);
    icon = getEntryIcon(entry, size);
    return icon;
}


#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (Icon can be created only from SIM)
/*!
 Get bitmap.
 \param aIconEF icon information array
 \retval bitmap.
 */
LOCAL_C CFbsBitmap* getSatBitmapL()
    {
    RSatSession satSession;
    CleanupClosePushL(satSession);
    RSatService satIcon;
    CleanupClosePushL(satIcon);

    satSession.ConnectL();
    satIcon.OpenL(satSession);

    
    TInt iconId(KErrNone);
    RProperty::Get(KCRUidCa, KCaSatUIIconId, iconId);
    
    CFbsBitmap* bitmap(NULL);

    if (iconId != KErrNone) {
        RIconEf iconEf;
        satIcon.GetIconInfoL(TUint8(iconId), iconEf);
        CleanupClosePushL(iconEf);

        TInt selectedIconIndex(KErrNotFound);
        TSize selectedIconSize(0, 0);
        
        for (TInt i = 0; i < iconEf.Count(); ++i) {
            if ((iconEf[i].IconSize().iHeight * iconEf[i].IconSize().iWidth)
                >= (selectedIconSize.iHeight * selectedIconSize.iWidth))
                if (bitmap) {
                    delete bitmap;
                    bitmap = NULL;
                }
            // test and select index of icon which is not too big
            bitmap = satIcon.GetIconL( iconEf[ i ] );
            if (bitmap) //!bitmap if iIcon is too big
            {
                selectedIconSize = iconEf[i].IconSize();
                selectedIconIndex = i;
            }
        }
        if (selectedIconIndex != KErrNotFound) {
            if (bitmap) {
                delete bitmap;
                bitmap = NULL;
            }
            bitmap = satIcon.GetIconL( iconEf[ selectedIconIndex ] );
        }
        CleanupStack::PopAndDestroy(&iconEf);
    }
    
    CleanupStack::PopAndDestroy(&satIcon);
    CleanupStack::PopAndDestroy(&satSession);
    
    return bitmap;
}
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT



#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (Icon can be created only from SIM)
/*!
 Get bitmap.
 \param aIconEF icon information array
 \retval bitmap.
 */
LOCAL_C CFbsBitmap* getSatMaskL(CFbsBitmap* aBitmap)
    {
    CFbsBitmap* mask(new (ELeave) CFbsBitmap);
    CleanupStack::PushL(mask);

    User::LeaveIfError(mask->Create(aBitmap->SizeInPixels(), EGray256));

    CFbsBitmapDevice* maskDevice = CFbsBitmapDevice::NewL(mask);
    CleanupStack::PushL(maskDevice);
    CFbsBitGc* maskGc;
    User::LeaveIfError(maskDevice->CreateContext(maskGc));
    CleanupStack::PushL(maskGc);
    maskGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
    maskGc->SetDrawMode(CGraphicsContext::EDrawModePEN);
    maskGc->SetBrushColor(KRgbBlack);
    maskGc->Clear();
    maskGc->SetBrushColor(KRgbWhite);
    maskGc->DrawRect(TRect(TPoint(), aBitmap->SizeInPixels()));
    CleanupStack::PopAndDestroy(maskGc);
    CleanupStack::PopAndDestroy(maskDevice);
    CleanupStack::Pop(mask);
    return mask;
}
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT


#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (Icon can be created only from SIM)
/*!
 Get icon from entry.
 \param entry const reference to CaEntry.
 \retval icon.
 */
LOCAL_C HbIcon getSatIconL()
{
    HbIcon icon;
    QPixmap pixmap;
    CFbsBitmap* bitmap(NULL);
    CFbsBitmap* mask(NULL);
    bitmap = getSatBitmapL();
    if(bitmap)
    {
        mask = getSatMaskL(bitmap);
        fromBitmapAndMaskToPixmap(bitmap, mask, pixmap);
        icon = HbIcon(QIcon(pixmap));
    }
   

    delete mask;
    delete bitmap;
    return icon;

}
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT


/*!
 Get icon.
 \param entry const reference to CaEntry.
 \param sie const reference to icon size.
 \retval icon.
 */
HbIcon CaMenuIconUtility::getEntryIcon(const CaEntry& entry, const QSizeF &size)
{
    HbIcon icon;

    icon = getIconFromEntry(entry, size);
    
    if ((icon.isNull() || !(icon.size().isValid())) &&
        entry.attribute(appUidAttributeName).toInt() == KSatUid.iUid) {
        TRAP_IGNORE(icon = getSatIconL());
    }
    
    if (icon.isNull() || !(icon.size().isValid())) {
        QString uidString(entry.attribute(appUidAttributeName));
        bool uidOk(false);
        int uidValue = uidString.toInt(&uidOk);
        if (!uidOk) {
            uidString = entry.iconDescription().applicationId();
            uidValue = uidString.toInt(&uidOk);
        }
        if (uidOk) {
            TRAP_IGNORE(icon = getIconFromApparcL(uidValue, size));
        }
    }

    if (icon.isNull() || !(icon.size().isValid())) {
        icon = getDefaultIcon(entry);
    }

    if (entry.entryTypeName() == XQConversions::s60DescToQString(
        KCaTypeWidget)) {
        icon.addProportionalBadge(Qt::AlignBottom | Qt::AlignRight,
            HbIcon("qtg_small_homescreen"), QSizeF(0.5, 0.5));
    }
    return icon;
}



