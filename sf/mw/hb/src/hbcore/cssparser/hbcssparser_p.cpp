/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbCore module of the UI Extensions for Mobile.
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

#include "hbcssparser_p.h"

#include <new>
#include <qdebug.h>
#include <qcolor.h>
#include <qfont.h>
#include <qfileinfo.h>
#include <qfontmetrics.h>
#include <qbrush.h>
#include <qimagereader.h>
#include <qgraphicswidget.h>

//QT_BEGIN_NAMESPACE

//#define CSSSTACKS_DEBUG 

#include "hbcssscanner_p.cpp"
#include "hbmemoryutils_p.h"
#include "hblayeredstyleloader_p.h"
#include "hbthemeindex_p.h"

using namespace HbCss;

const QString GLOBAL_CSS_SELECTOR = "*";

struct HbCssKnownValue
{
    const char *name;
    quint64 id;
};

static const HbCssKnownValue properties[NumProperties - 1] = {
    { "alignment", Property_Alignment },
    { "anchor-direction", Property_AnchorDirection },
    { "aspect-ratio", Property_AspectRatio },
    { "border-width", Property_BorderWidth },
    { "border-width-bottom", Property_BorderWidthBottom },
    { "border-width-left", Property_BorderWidthLeft },
    { "border-width-right", Property_BorderWidthRight },
    { "border-width-top", Property_BorderWidthTop },
    { "bottom", Property_Bottom },
    { "center-horizontal", Property_CenterHorizontal },
    { "center-vertical", Property_CenterVertical },
    { "color", Property_Color },
    { "fixed-height", Property_FixedHeight },
    { "fixed-length", Property_FixedLength },
    { "fixed-size", Property_FixedSize },
    { "fixed-width", Property_FixedWidth },
    { "font", Property_Font },
    { "font-family", Property_FontFamily },
    { "font-size", Property_FontSize },
    { "font-style", Property_FontStyle },
    { "font-variant", Property_FontVariant },
    { "font-weight", Property_FontWeight },
    { "layout", Property_Layout },
    { "layout-direction", Property_LayoutDirection },
    { "left", Property_Left },
    { "max-height", Property_MaximumHeight },
    { "max-length", Property_MaximumLength },
    { "max-size", Property_MaximumSize },
    { "max-width", Property_MaximumWidth },
    { "min-height", Property_MinimumHeight },
    { "min-length", Property_MinimumLength },
    { "min-size", Property_MinimumSize },
    { "min-width", Property_MinimumWidth },
    { "pref-height", Property_PreferredHeight },
    { "pref-length", Property_PreferredLength },
    { "pref-size", Property_PreferredSize },
    { "pref-width", Property_PreferredWidth },
    { "right", Property_Right },
    { "section", Property_Section },
    { "size-policy", Property_SizePolicy },
    { "size-policy-horizontal", Property_SizePolicyHorizontal },
    { "size-policy-vertical", Property_SizePolicyVertical },
    { "text-align", Property_TextAlignment },
    { "text-decoration", Property_TextDecoration },
    { "text-height", Property_TextHeight },
    { "text-line-count-max", Property_TextLineCountMax },
    { "text-line-count-min", Property_TextLineCountMin },
    { "text-transform", Property_TextTransform },
    { "text-wrap-mode", Property_TextWrapMode },
    { "top", Property_Top },
    { "zvalue", Property_ZValue }
};

static const HbCssKnownValue values[NumKnownValues - 1] = {
    { "bold", Value_Bold },
    { "bottom", Value_Bottom },
    { "center", Value_Center },
    { "digital", Value_Digital },
    { "expanding", Value_Expanding },
    { "fixed", Value_Fixed },
    { "ignore", Value_Ignore },
    { "ignored", Value_Ignored },
    { "italic", Value_Italic },
    { "keep", Value_Keep },
    { "keep-expand", Value_KeepExpand },
    { "left", Value_Left },
    { "left-to-right", Value_LeftToRight },
    { "line-through", Value_LineThrough },
    { "lowercase", Value_Lowercase },
    { "maximum", Value_Maximum },
    { "minimum", Value_Minimum },
    { "minimum-expanding", Value_MinimumExpanding },
    { "negative", Value_Negative },
    { "no-wrap", Value_NoWrap },
    { "none", Value_None },
    { "normal", Value_Normal },
    { "oblique", Value_Oblique },
    { "overline", Value_Overline },
    { "parent", Value_Parent },
    { "positive", Value_Positive },
    { "preferred", Value_Preferred },
    { "primary", Value_Primary },
    { "primary-small", Value_PrimarySmall },
    { "right", Value_Right },
    { "right-to-left", Value_RightToLeft },
    { "secondary", Value_Secondary },
    { "small-caps", Value_SmallCaps },
    { "title", Value_Title },
    { "top", Value_Top },
    { "transparent", Value_Transparent },
    { "underline", Value_Underline },
    { "uppercase", Value_Uppercase },
    { "word-wrap", Value_WordWrap },
    { "wrap-anywhere", Value_WrapAnywhere }
};

static const HbCssKnownValue pseudos[NumPseudos - 1] = {
    { "landscape", PseudoClass_Landscape },
    { "left-to-right", PseudoClass_LeftToRight },
    { "portrait", PseudoClass_Portrait },
    { "right-to-left", PseudoClass_RightToLeft }
};

inline bool operator<(const QString &name, const HbCssKnownValue &prop)
{
    return QString::compare(name, QLatin1String(prop.name), Qt::CaseInsensitive) < 0;
}

inline bool operator<(const HbCssKnownValue &prop, const QString &name)
{
    return QString::compare(QLatin1String(prop.name), name, Qt::CaseInsensitive) < 0;
}

static quint64 findKnownValue(const QString &name, const HbCssKnownValue *start, int numValues)
{
    const HbCssKnownValue *end = &start[numValues - 1];
    const HbCssKnownValue *prop = qBinaryFind(start, end, name);
    if (prop == end)
        return 0;
    return prop->id;
}

#ifndef HB_BIN_CSS
///////////////////////////////////////////////////////////////////////////////
// Value Extractor
ValueExtractor::ValueExtractor(const HbVector<Declaration> &decls, const HbDeviceProfile &profile)
: declarations(decls), currentProfile(profile)
{
}
ValueExtractor::ValueExtractor(
    const HbVector<Declaration> &decls, 
    const QHash<QString, 
    HbCss::Declaration> &varDeclarations,
    const HbDeviceProfile &profile)
: declarations(decls), variableDeclarationsHash(varDeclarations), currentProfile(profile)
{
}

ValueExtractor::ValueExtractor(
    const HbVector<Declaration> &varDecls, 
    bool isVariable, 
    const HbDeviceProfile &profile)
: variableDeclarations(varDecls), currentProfile(profile)
{
    Q_UNUSED(isVariable)
    // Initialize to some profile.
    if ( currentProfile.isNull() ) {
        currentProfile = HbDeviceProfile::current();
    }
}

ValueExtractor::ValueExtractor(
    const QHash<QString, 
    HbCss::Declaration> &varDecls, 
    bool isVariable, 
    const HbDeviceProfile &profile)
: variableDeclarationsHash(varDecls), currentProfile(profile)
{
    Q_UNUSED(isVariable)
    // Initialize to some profile.
    if ( currentProfile.isNull() ) {
        currentProfile = HbDeviceProfile::current();
    }
}

qreal ValueExtractor::asReal(const Value& v, bool *ok)
{
    QString s = v.variant.toString();
    s.reserve(s.length());

    if (v.type == Value::Expression || v.type == Value::ExpressionNegative) {
        qreal factor = (v.type == Value::Expression) ? 1.0 : -1.0;
        qreal value = 0;
        extractExpressionValue(s, value);
        return factor * value;
    }
    return asReal(s, v.type, ok);
}

qreal ValueExtractor::asReal(QString &s, Value::Type type, bool *ok)
{
    if (ok) {
        *ok = true;
    }

    if (type == Value::Variable || type == Value::VariableNegative) {
        qreal factor = (type == Value::Variable) ? 1.0 : -1.0;
        HbVector<HbCss::Value> values;
        if (extractVariableValue(s, values))
            return factor * asReal(values.first(), ok);
        else
            if (ok) {
                *ok = false;
            }
            return 0;
    } else if (type == Value::Percentage) {
        qreal result = s.toDouble(ok) /100.0;
        if (ok && !(*ok)) {
            return 0;
        } else {
            return result;
        }
    }

    enum { None, Px, Un, Mm } unit = None;
    if (s.endsWith(QLatin1String("un"), Qt::CaseInsensitive)) {
        unit = Un;
    } else if (s.endsWith(QLatin1String("px"), Qt::CaseInsensitive)) {
        unit = Px;
    } else if (s.endsWith(QLatin1String("mm"), Qt::CaseInsensitive)) {
        unit = Mm;
    }

    if (unit != None) {
        // Assuming all unit identifiers have two characters
        s.chop(2);
    }

    qreal result = s.toDouble(ok);
    if (ok && !(*ok)) {
        return 0;
    }

    if (unit == Un) {
        result = currentProfile.unitValue() * result;
    } else if (unit == Mm) {
        result = currentProfile.ppmValue() * result;
    } // else -> already in pixels
    return result;
}

qreal ValueExtractor::asReal(const Declaration &decl, bool *ok)
{
    if (decl.values.count() < 1) {
        if (ok) {
            *ok = false;
        }
        return 0;
    }
    return asReal(decl.values.first(), ok);
}

bool ValueExtractor::asReals(const Declaration &decl, qreal *m)
{
    bool ok = true;
    int i;
    for (i = 0; i < qMin(decl.values.count(), 4); i++) {
        m[i] = asReal(decl.values[i], &ok);
        if (!ok) {
            return false;
        }
    }

    if (i == 0) m[0] = m[1] = m[2] = m[3] = 0;
    else if (i == 1) m[3] = m[2] = m[1] = m[0];
    else if (i == 2) m[2] = m[0], m[3] = m[1];
    else if (i == 3) m[3] = m[1];

    return true;
}

static QSizePolicy::Policy parseSizePolicy(const Value& v)
{
    QSizePolicy::Policy pol(QSizePolicy::Preferred);
    switch (v.variant.toInt())
        {
        case Value_Fixed: pol = QSizePolicy::Fixed; break;
        case Value_Minimum: pol = QSizePolicy::Minimum; break;
        case Value_Maximum: pol = QSizePolicy::Maximum; break;
        case Value_Preferred: pol = QSizePolicy::Preferred; break;
        case Value_Expanding: pol = QSizePolicy::Expanding; break;
        case Value_MinimumExpanding: pol = QSizePolicy::MinimumExpanding; break;
        case Value_Ignored: pol = QSizePolicy::Ignored; break;
        default: break;
        }
    return pol;
}

static HbCss::LayoutDirection parseLayoutDirectionValue(const Value v)
{
    HbCss::LayoutDirection retVal(HbCss::LayoutDirection_Parent); // Parent as default
    if(v.type == Value::KnownIdentifier) {
        switch(v.variant.toInt()) {
        case Value_RightToLeft:
            retVal = HbCss::LayoutDirection_RightToLeft;
            break;
        case Value_LeftToRight:
            retVal = HbCss::LayoutDirection_LeftToRight;
            break;
        case Value_Parent:
        default:
            break;
        }
    }
    return retVal;
}

static Qt::AspectRatioMode parseAspectRatioMode(const Value& v)
{
    Qt::AspectRatioMode mode = Qt::KeepAspectRatio;
    switch (v.variant.toInt()) {
    case Value_Ignore:
        mode = Qt::IgnoreAspectRatio;
        break;
    case Value_KeepExpand:
        mode = Qt::KeepAspectRatioByExpanding;
        break;
    case Value_Keep:
    default:
        break;
    }
    return mode;
}

static HbAnchor::Direction parseAnchorDirection(const Value& v)
{
    HbAnchor::Direction dir = HbAnchor::Positive;
    switch (v.variant.toInt()) {
    case Value_Negative:
        dir = HbAnchor::Negative;
        break;
    case Value_Positive:
    default:
        break;
    }
    return dir;
}

static Qt::Alignment parseAlignment(const Declaration &decl)
{
    if (decl.values.isEmpty() || decl.values.count() > 2)
        return Qt::AlignLeft | Qt::AlignTop;

    Qt::Alignment a[2] = { 0, 0 };
    for (int i = 0; i < qMin(2, decl.values.count()); i++) {
        if (decl.values.at(i).type != Value::KnownIdentifier)
            break;
        switch (decl.values.at(i).variant.toInt()) {
        case Value_Left: a[i] = Qt::AlignLeft; break;
        case Value_Right: a[i] = Qt::AlignRight; break;
        case Value_Top: a[i] = Qt::AlignTop; break;
        case Value_Bottom: a[i] = Qt::AlignBottom; break;
        case Value_Center: a[i] = Qt::AlignCenter; break;
        default: break;
        }
    }

    if (a[0] == Qt::AlignCenter && a[1] != 0 && a[1] != Qt::AlignCenter)
        a[0] = (a[1] == Qt::AlignLeft || 
            a[1] == Qt::AlignRight) ? Qt::AlignVCenter : Qt::AlignHCenter;
    if ((a[1] == 0 || a[1] == Qt::AlignCenter) && a[0] != Qt::AlignCenter)
        a[1] = (a[0] == Qt::AlignLeft || 
            a[0] == Qt::AlignRight) ? Qt::AlignVCenter : Qt::AlignHCenter;
    return a[0] | a[1];
}

static Hb::TextWrapping parseWrapMode(const Value v)
{
    Hb::TextWrapping mode(Hb::TextNoWrap);
    if (v.type == Value::KnownIdentifier) {
        switch(v.variant.toInt()) {
        case Value_WordWrap: mode = Hb::TextWordWrap; break;
        case Value_WrapAnywhere: mode = Hb::TextWrapAnywhere; break;
        case Value_NoWrap: // fall-through
        default: break;
        }
    }
    return mode;
}

static bool setFontSizeFromValue(Value value, QFont &font)
{
    if (value.type != Value::Length)
        return false;

    bool valid = false;
    QString s = value.variant.toString();
    if (s.endsWith(QLatin1String("pt"), Qt::CaseInsensitive)) {
        s.chop(2);
        value.variant = s;
        if (value.variant.convert(HbVariant::Double)) {
            font.setPointSizeF(value.variant.toDouble());
            valid = true;
        }
    } else if (s.endsWith(QLatin1String("px"), Qt::CaseInsensitive)) {
        s.chop(2);
        value.variant = s;
        if (value.variant.convert(HbVariant::Int)) {
            font.setPixelSize(value.variant.toInt());
            valid = true;
        }
    }
    return valid;
}

static bool setFontStyleFromValue(const Value &value, QFont &font)
{
    if (value.type != Value::KnownIdentifier)
        return false ;
    switch (value.variant.toInt()) {
        case Value_Normal: font.setStyle(QFont::StyleNormal); return true;
        case Value_Italic: font.setStyle(QFont::StyleItalic); return true;
        case Value_Oblique: font.setStyle(QFont::StyleOblique); return true;
        default: break;
    }
    return false;
}

static bool setFontWeightFromValue(const Value &value, QFont &font)
{
    if (value.type == Value::KnownIdentifier) {
        switch (value.variant.toInt()) {
            case Value_Normal: font.setWeight(QFont::Normal); return true;
            case Value_Bold: font.setWeight(QFont::Bold); return true;
            default: break;
        }
        return false;
    }
    if (value.type != Value::Number)
        return false;
    font.setWeight(qMin(value.variant.toInt() / 8, 99));
    return true;
}

static bool setFontFamilyFromValues(const Declaration &decl, QFont &font)
{
    QString family;
    for (int i = 0; i < decl.values.count(); ++i) {
        const Value &v = decl.values.at(i);
        if (v.type == Value::TermOperatorComma)
            break;
        const QString str = v.variant.toString();
        if (str.isEmpty())
            break;
        family += str;
        family += QLatin1Char(' ');
    }
    family = family.simplified();
    if (family.isEmpty())
        return false;
    font.setFamily(family);
    return true;
}

static void setTextDecorationFromValues(const Declaration &decl, QFont &font)
{
    for (int i = 0; i < decl.values.count(); ++i) {
        if (decl.values.at(i).type != Value::KnownIdentifier)
            continue;
        switch (decl.values.at(i).variant.toInt()) {
            case Value_Underline: font.setUnderline(true); break;
            case Value_Overline: font.setOverline(true); break;
            case Value_LineThrough: font.setStrikeOut(true); break;
            case Value_None:
                font.setUnderline(false);
                font.setOverline(false);
                font.setStrikeOut(false);
                break;
            default: break;
        }
    }
}

static void parseShorthandFontProperty(const Declaration &decl, QFont &font)
{
    font.setStyle(QFont::StyleNormal);
    font.setWeight(QFont::Normal);

    int i = 0;
    while (i < decl.values.count()) {
        if (setFontStyleFromValue(decl.values.at(i), font)
            || setFontWeightFromValue(decl.values.at(i), font))
            ++i;
        else
            break;
    }

    if (i < decl.values.count()) {
        setFontSizeFromValue(decl.values.at(i), font);
        ++i;
    }

    if (i < decl.values.count()) {
        QString fam = decl.values.at(i).variant.toString();
        if (!fam.isEmpty())
            font.setFamily(fam);
    }
}

static void setFontVariantFromValue(const Value &value, HbFontSpec &fontSpec, QFont &font )
{
    // Sets font variants. Some set the fontspec and some the HbFontSpec
    HbFontSpec::Role role( HbFontSpec::Undefined );
    if (value.type == Value::KnownIdentifier) {
        switch (value.variant.toInt()) {
            case Value_Normal: font.setCapitalization(QFont::MixedCase); break;
            case Value_SmallCaps: font.setCapitalization(QFont::SmallCaps); break;
            case Value_Primary: role = HbFontSpec::Primary; break;
            case Value_Secondary: role = HbFontSpec::Secondary; break;
            case Value_Title: role = HbFontSpec::Title; break;
            case Value_PrimarySmall: role = HbFontSpec::PrimarySmall; break;
            case Value_Digital: role = HbFontSpec::Digital; break;
            default: break;
        }
    }
    if (role != HbFontSpec::Undefined) {
        fontSpec.setRole( role );
    }
}

static void setTextTransformFromValue(const Value &value, QFont &font)
{
    if (value.type == Value::KnownIdentifier) {
        switch (value.variant.toInt()) {
            case Value_None: font.setCapitalization(QFont::MixedCase); break;
            case Value_Uppercase: font.setCapitalization(QFont::AllUppercase); break;
            case Value_Lowercase: font.setCapitalization(QFont::AllLowercase); break;
            default: break;
        }
    }
}

bool ValueExtractor::extractKnownProperties(KnownProperties &prop)
{
    KnownPropertyFlags flags(0);
    bool hit = false;
    bool tphSet = false;

    // Initialize border prop to zero
    prop.mBorderWidths[TopEdge] = 0.0;
    prop.mBorderWidths[RightEdge] = 0.0;
    prop.mBorderWidths[BottomEdge] = 0.0;
    prop.mBorderWidths[LeftEdge] = 0.0;

    // Initialize font prop
    prop.mFont = QFont();
    prop.mFontSpec = HbFontSpec();

    for (int i = 0; i < declarations.count(); i++) {
        const Declaration &decl = declarations.at(i);
        switch (decl.propertyId) {
        case Property_MinimumWidth: prop.mMinW = asReal(decl); flags|=ExtractedMinW; break;
        case Property_MinimumLength: // fall-through
        case Property_MinimumHeight: prop.mMinH = asReal(decl); flags|=ExtractedMinH; break;
        case Property_MaximumWidth: prop.mMaxW = asReal(decl); flags|=ExtractedMaxW; break;
        case Property_MaximumLength: // fall-through
        case Property_MaximumHeight: prop.mMaxH = asReal(decl); flags|=ExtractedMaxH; break;
        case Property_PreferredWidth: prop.mPrefW = asReal(decl); flags|=ExtractedPrefW; break;
        case Property_PreferredLength: // fall-through
        case Property_PreferredHeight: prop.mPrefH = asReal(decl); flags|=ExtractedPrefH; break;
        case Property_FixedWidth:
            prop.mPrefW = asReal(decl); flags|=ExtractedPrefW;
            prop.mSizePolicy.setHorizontalPolicy(QSizePolicy::Fixed); flags|=ExtractedPolHor;
            break;
        case Property_FixedLength: // fall-through
        case Property_FixedHeight:
            prop.mPrefH = asReal(decl); flags|=ExtractedPrefH;
            prop.mSizePolicy.setVerticalPolicy(QSizePolicy::Fixed); flags|=ExtractedPolVer;
            if (!tphSet) { 
                // legacy support. deprecated.
                prop.mFontSpec.setTextHeight(asReal(decl));
            }
            break;
        case Property_SizePolicy:
            prop.mSizePolicy.setHorizontalPolicy(parseSizePolicy(decl.values.at(0)));
            if (decl.values.count() > 1) {
                prop.mSizePolicy.setVerticalPolicy(parseSizePolicy(decl.values.at(1)));
            } else {
                prop.mSizePolicy.setVerticalPolicy(prop.mSizePolicy.horizontalPolicy());
            }
            flags|=ExtractedPolHor;
            flags|=ExtractedPolVer;
            break;
        case Property_SizePolicyHorizontal:
            prop.mSizePolicy.setHorizontalPolicy(parseSizePolicy(decl.values.at(0)));
            flags|=ExtractedPolHor;
            break;
        case Property_SizePolicyVertical:
            prop.mSizePolicy.setVerticalPolicy(parseSizePolicy(decl.values.at(0)));
            flags|=ExtractedPolVer;
            break;
        case Property_MinimumSize:
            prop.mMinW = asReal(decl.values.at(0));
            prop.mMinH = (decl.values.count() > 1) ? asReal(decl.values.at(1)) : prop.mMinW;
            flags|=ExtractedMinW;
            flags|=ExtractedMinH;
            break;
        case Property_MaximumSize:
            prop.mMaxW = asReal(decl.values.at(0));
            prop.mMaxH = (decl.values.count() > 1) ? asReal(decl.values.at(1)) : prop.mMaxW;
            flags|=ExtractedMaxW;
            flags|=ExtractedMaxH;
            break;
        case Property_PreferredSize:
            prop.mPrefW = asReal(decl.values.at(0));
            prop.mPrefH = (decl.values.count() > 1) ? asReal(decl.values.at(1)) : prop.mPrefW;
            flags|=ExtractedPrefW;
            flags|=ExtractedPrefH;
            break;
        case Property_FixedSize:
            prop.mPrefW = asReal(decl.values.at(0));
            prop.mPrefH = (decl.values.count() > 1) ? asReal(decl.values.at(1)) : prop.mPrefW;
            prop.mSizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
            prop.mSizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
            flags|=ExtractedPrefW;
            flags|=ExtractedPrefH;
            flags|=ExtractedPolHor;
            flags|=ExtractedPolVer;
            break;

        case Property_Left: prop.mLeft = asReal(decl); flags|=ExtractedLeft; break;
        case Property_Top: prop.mTop = asReal(decl); flags|=ExtractedTop; break;
        case Property_Right: prop.mRight = asReal(decl); flags|=ExtractedRight; break;
        case Property_Bottom: prop.mBottom = asReal(decl); flags|=ExtractedBottom; break;
        case Property_CenterHorizontal: 
            prop.mCenterH = asReal(decl); flags|=ExtractedCenterH; break;
        case Property_CenterVertical: 
            prop.mCenterV = asReal(decl); flags|=ExtractedCenterV; break;

        case Property_LayoutDirection: // fall-through
            prop.mLayoutDir = parseLayoutDirectionValue(decl.values.at(0));
            flags|=ExtractedLayoutDir;
            break;

        case Property_Alignment: 
        case Property_TextAlignment: 
            prop.mAlignment = parseAlignment(decl); flags|=ExtractedAlignment; break;
        case Property_TextLineCountMin: 
            prop.mMinLines = decl.values.first().variant.toInt(); flags|=ExtractedMinLines; break;
        case Property_TextLineCountMax: 
            prop.mMaxLines = decl.values.first().variant.toInt(); flags|=ExtractedMaxLines; break;
        case Property_TextWrapMode: 
            prop.mTextWrapMode = parseWrapMode(decl.values.at(0)); flags|=ExtractedWrapMode; break;
        case Property_ZValue: prop.mZ = 
            asReal(decl); flags|=ExtractedZValue; break;

        case Property_BorderWidthBottom: 
            prop.mBorderWidths[BottomEdge] = asReal(decl); flags|=ExtractedBorderWidths; break;
        case Property_BorderWidthLeft: 
            prop.mBorderWidths[LeftEdge] = asReal(decl); flags|=ExtractedBorderWidths; break;
        case Property_BorderWidthRight: 
            prop.mBorderWidths[RightEdge] = asReal(decl); flags|=ExtractedBorderWidths; break;
        case Property_BorderWidthTop: 
            prop.mBorderWidths[TopEdge] = asReal(decl); flags|=ExtractedBorderWidths; break;
        case Property_BorderWidth: 
            asReals(decl, prop.mBorderWidths); flags|=ExtractedBorderWidths; break;

        case Property_AspectRatio:
            prop.mAspectRatioMode = parseAspectRatioMode(decl.values.at(0));
            flags|=ExtractedAspectRatioMode;
            break;

        case Property_FontSize: setFontSizeFromValue(decl.values.at(0), prop.mFont); break;
        case Property_FontStyle: setFontStyleFromValue(decl.values.at(0), prop.mFont); break;
        case Property_FontWeight: setFontWeightFromValue(decl.values.at(0), prop.mFont); break;
        case Property_FontFamily: setFontFamilyFromValues(decl, prop.mFont); break;
        case Property_TextDecoration: setTextDecorationFromValues(decl, prop.mFont); break;
        case Property_Font: parseShorthandFontProperty(decl, prop.mFont); break;
        case Property_FontVariant: 
            setFontVariantFromValue(decl.values.at(0), prop.mFontSpec, prop.mFont); break;
        case Property_TextTransform: 
            setTextTransformFromValue(decl.values.at(0), prop.mFont); break;
        case Property_TextHeight: 
            tphSet = true; prop.mFontSpec.setTextHeight(asReal(decl)); break;

        case Property_AnchorDirection:
            prop.mAnchorDir = parseAnchorDirection(decl.values.at(0));
            flags|=ExtractedAnchorDir;
            break;
        default: continue;
        }
        hit = true;
    }

    if (prop.mFont != QFont()) {
        flags |= ExtractedFont;
    }
    if (!prop.mFontSpec.isNull()) {
        flags |= ExtractedFontSpec;
    }

    prop.mFlags = flags;
    return hit;
}

static QColor parseColorValue(Value v)
{
    if (v.type == Value::Identifier || v.type == Value::String || v.type == Value::Color)
        return v.variant.toColor();

    if (v.type == Value::KnownIdentifier && v.variant.toInt() == Value_Transparent)
        return Qt::transparent;

    if (v.type != Value::Function)
        return QColor();

    QStringList lst = v.variant.toStringList();
    if (lst.count() != 2)
        return QColor();

    bool rgb = lst.at(0).startsWith(QLatin1String("rgb"));

    Parser p(lst.at(1));
    if (!p.testExpr())
        return QColor();

    HbVector<Value> colorDigits(v.memoryType);
    if (!p.parseExpr(&colorDigits))
        return QColor();

    for (int i = 0; i < qMin(colorDigits.count(), 7); i += 2) {
        if (colorDigits.at(i).type == Value::Percentage) {
            colorDigits[i].variant = colorDigits.at(i).variant.toDouble() * 255. / 100.;
            colorDigits[i].type = Value::Number;
        } else if (colorDigits.at(i).type != Value::Number) {
            return QColor();
        }
    }

    int v1 = colorDigits.at(0).variant.toInt();
    int v2 = colorDigits.at(2).variant.toInt();
    int v3 = colorDigits.at(4).variant.toInt();
    int alpha = colorDigits.count() >= 7 ? colorDigits.at(6).variant.toInt() : 255;

    return rgb ? QColor::fromRgb(v1, v2, v3, alpha)
               : QColor::fromHsv(v1, v2, v3, alpha);
}

bool ValueExtractor::extractVariableValue(
    const QString& variableName, 
    HbVector<HbCss::Value>& values) const
{
    bool variableFound = false;
    if ( !variableDeclarationsHash.isEmpty() ) {
        values = variableDeclarationsHash.value(variableName).values;
        if ( !values.isEmpty() ) {
            variableFound = true;
        }
    } else {
        const int variableCount = variableDeclarations.count();
        for (int i=variableCount-1; i>=0; i--) {
            if (variableDeclarations.at(i).property == variableName ) {
                values = variableDeclarations.at(i).values;
                variableFound = true;
                break;
            }
        }    
    }
    return variableFound;
}

bool ValueExtractor::extractVariableValue(const QString& variableName, qreal& value)
{
    bool variableFound = false;
    HbVector<HbCss::Value> values;
    if (extractVariableValue(variableName, values)) {
        value = asReal(values.first());
        variableFound = true;
    }
    return variableFound;    
}

bool ValueExtractor::extractVariableValue( const QString& variableName, HbCss::Value &val ) const
{
    HbVector<HbCss::Value> values;
    bool variableFound = extractVariableValue( variableName, values );

    //for variable cascading support
    if ( variableFound ) {
        val = values.first();
        if ( val.type == Value::Variable ){
            variableFound = extractVariableValue ( val.variant.toString (), val );
        }
    } else {
        HbLayeredStyleLoader *styleLoader = 
            HbLayeredStyleLoader::getStack(HbLayeredStyleLoader::Concern_Colors);
        if (styleLoader) {
            variableFound = styleLoader->findInDefaultVariables(variableName, val);
        }
    }
    return variableFound;
}

bool ValueExtractor::extractExpressionValue(QString &expression, qreal &value)
{
    // todo: invalid global variables are not checked because asReal() doesn't check them
    value = 0;
    const QChar SPACE(' ');
    const QChar LPARENTHESIS('(');
    const QChar RPARENTHESIS(')');
    const QChar MINUS('-');
    const QChar PLUS('+');
    const QChar STAR('*');
    const QChar SLASH('/');

    int position = 0;
    int begin = -1;
    // + and - are level 0, * and / in level 1, unary - is level 2
    // every parenthesis add 10 to (base) precedence level
    int precedenceLevel = 0; 
    bool parseVariable = false;
    bool endMark = false;
    int operatorCount = 1;//there can only be 2 sequental operators if the latter one is unary '-'
    while (position < expression.size()) {
        endMark = false;
        if (expression.at(position) == SPACE) {
            endMark = true;
        } else if ((expression.at(position) == LPARENTHESIS) && !parseVariable) {
            precedenceLevel+=10;
            position++;
            operatorCount = 1;
            continue;
        } else if (expression.at(position) == RPARENTHESIS) {
            if (parseVariable) {
                parseVariable = false;
                operatorCount = 0;
                position++;
                continue;
            }
            precedenceLevel-=10;
            operatorCount = 0;
            endMark = true;
        } else if ((expression.at(position) == MINUS) && !parseVariable) {
            endMark = true;
        } else if ((expression.at(position) == PLUS) ||
                   (expression.at(position) == STAR) ||
                   (expression.at(position) == SLASH)) {
            endMark = true;
        }

        if (endMark) {
            if (begin >= 0) {
                // parse value
                QString valueString = expression.mid(begin, position - begin);
                qreal val = 0;
                if (valueString.startsWith("var(") && valueString.endsWith(")")) {
                    // remove var( and last )
                    QString variableString = valueString.mid(4, valueString.size()-5);
                    if (!extractVariableValue(variableString, val)) {
                        expressionValues.clear();
                        return false;
                    }
                } else {
                    bool real_ok = true;
                    val = asReal(valueString, Value::String, &real_ok);
                    if (!real_ok) {
                        expressionValues.clear();
                        return false;
                    }
                }
                expressionValues.append(ExpressionValue(ExpressionValue::None, 0, val));
                operatorCount = 0;
            }
            begin = -1;
            if (expression.at(position) == MINUS) {
                if (operatorCount == 1) {
                    expressionValues.append(
                        ExpressionValue(ExpressionValue::UnaryMinus,precedenceLevel+2,0));
                } else if (operatorCount > 1) {
                    expressionValues.clear();
                    return false;
                } else {
                    expressionValues.append(
                        ExpressionValue(ExpressionValue::Minus,precedenceLevel,0));
                }
                operatorCount++;
            } else if (expression.at(position) == PLUS) {
                if (operatorCount > 0) {
                    expressionValues.clear();
                    return false;
                }
                expressionValues.append(
                    ExpressionValue(ExpressionValue::Plus,precedenceLevel,0));
                operatorCount++;
            } else if (expression.at(position) == STAR) {
                if (operatorCount > 0) {
                    expressionValues.clear();
                    return false;
                }
                expressionValues.append(
                    ExpressionValue(ExpressionValue::Star,precedenceLevel+1,0));
                operatorCount++;
            } else if (expression.at(position) == SLASH) {
                if (operatorCount > 0) {
                    expressionValues.clear();
                    return false;
                }
                expressionValues.append(
                    ExpressionValue(ExpressionValue::Slash,precedenceLevel+1,0));
                operatorCount++;
            }
            position++;
            continue;
        }

        if (begin == -1) {
            begin = position;
        }

        // flag variable parsing (variable syntax contains parenthesis)
        if ((expression.at(position) == QChar('v')) && !parseVariable) {
            parseVariable = true;
            position++;
            continue;
        }
        position++;
    }

    // check for unmatching parentheses
    if (precedenceLevel != 0) {
        expressionValues.clear();
        return false;
    }

    // parse last value
    if (begin >= 0) {
        QString valueString = expression.mid(begin, position - begin);
        qreal val = 0;
        if (valueString.startsWith("var(") && valueString.endsWith(")")) {
            // remove var( and last )
            QString variableString = valueString.mid(4, valueString.size()-5);
            if (!extractVariableValue(variableString, val)) {
                expressionValues.clear();
                return false;
            }
        } else {
            bool real_ok = true;
            val = asReal(valueString, Value::String, &real_ok);
            if (!real_ok) {
                expressionValues.clear();
                return false;
            }
        }
        expressionValues.append(ExpressionValue(ExpressionValue::None, 0, val));
    }

    if(expressionValues.isEmpty()) {
        expressionValues.clear();
        return false;
    }
        
    // if last value is operator, fail
    if (expressionValues[expressionValues.size()-1].mToken != ExpressionValue::None) {
        expressionValues.clear();
        return false;
    }


    while (expressionValues.size() > 1) { // we have an answer when size = 1
        int maxPrecedence = -1;
        int calculateIndex = -1;
        for (int i = 0; i < expressionValues.size(); i++) {
            if ((expressionValues[i].mToken != ExpressionValue::None) &&
                (expressionValues[i].mPrecedence > maxPrecedence)) {
                maxPrecedence = expressionValues[i].mPrecedence;
                calculateIndex = i; // contains operator with highest precedence
            }
        }
        qreal answer = 0;

        if(calculateIndex < 0){
            return false;
        }

        switch (expressionValues[calculateIndex].mToken) {
            case ExpressionValue::Minus:
                answer = expressionValues[calculateIndex-1].mValue -
                         expressionValues[calculateIndex+1].mValue;
                break;
            case ExpressionValue::Plus:
                answer = expressionValues[calculateIndex-1].mValue +
                         expressionValues[calculateIndex+1].mValue;
                break;
            case ExpressionValue::Star:
                answer = expressionValues[calculateIndex-1].mValue *
                         expressionValues[calculateIndex+1].mValue;
                break;
            case ExpressionValue::Slash:
                if (expressionValues[calculateIndex+1].mValue == 0) {
                    expressionValues.clear();
                    return false;
                }
                answer = expressionValues[calculateIndex-1].mValue /
                         expressionValues[calculateIndex+1].mValue;
                break;
            default:
                break;
        }
        if (expressionValues[calculateIndex].mToken == ExpressionValue::UnaryMinus) {
            expressionValues[calculateIndex+1].mValue = 
                -expressionValues[calculateIndex+1].mValue;
            expressionValues.removeAt(calculateIndex);
        } else {
            expressionValues[calculateIndex-1].mValue = answer;
            expressionValues.removeAt(calculateIndex+1);
            expressionValues.removeAt(calculateIndex);
        }
    }

    value = expressionValues[0].mValue;
    expressionValues.clear();

    return true;    
}


bool ValueExtractor::extractCustomProperties( const QList<QString> &keys, QList<QVariant> &values )
{
    if ( keys.count() != values.count() ) {
        return false;
    }
    for ( int i = 0; i < declarations.count(); i++ ) {
        for( int j = 0; j < keys.count(); j++ ) {
            if (declarations[i].property == keys[j] ) {
                Value val = declarations[i].values.last();
                switch (val.type) {
                    case Value::Length:
                    case Value::Variable:
                    case Value::VariableNegative:
                    case Value::Expression:
                    case Value::ExpressionNegative:
                    case Value::Percentage:
                        values[j] = asReal(val);
                        break;
                    case Value::KnownIdentifier:
                        values[j] = (QString)val.original;
                        break;
                    default:
                        values[j] = val.variant;
                        break;
                }
                break;
            }
        }
    }
    return true;
}

bool ValueExtractor::extractLayout(QString &layoutName, QString &sectionName)
{
    QString tempSectionName;
    bool hit = false;
    for (int i = 0; i < declarations.count(); ++i) {
        const Declaration &decl = declarations.at(i);
        if ( decl.propertyId == Property_Layout ) {
            if( decl.values.count() == 1 ) {
                layoutName = decl.values.at(0).variant.toString();
                hit = true; 
            }
        }
        else if ( decl.propertyId == Property_Section ) {
            if (decl.values.count() == 1 ) {
                tempSectionName = decl.values.at(0).variant.toString();
                //  a section without a layout doesn't count as a hit
            }
        }
    }
    if(hit)
        sectionName = tempSectionName;
    return hit;
}

bool ValueExtractor::extractColor( QColor &color ) const
{
    bool hit = false;
    const int declarationsCount = declarations.count();
    for ( int i = 0; i < declarationsCount; ++i ) {
        const Declaration &decl = declarations.at(i);
        switch(decl.propertyId) {
        case Property_Color:
            {
            HbCss::Value value;
            if ( decl.values.at(0).type == Value::Variable ) {
                const QString variableName = decl.values.at(0).variant.toString();
                HbThemeIndexResource resource(variableName);
                if (resource.isValid()) {
                    // Color value coming from index
                    color = resource.colorValue();
                } else {
                    // Color value coming from custom css
                    extractVariableValue( variableName, value );
                    color = parseColorValue(value);
                }
            } else {
                value = decl.values.at(0);
                color = parseColorValue(value);
            }
            hit = true;
            break;
            }
        default:
            break;
        }
    }
    return hit;
}
#endif


///////////////////////////////////////////////////////////////////////////////
// Selector
int Selector::specificity() const
{
    int val = 0;
    for (int i = 0; i < basicSelectors.count(); ++i) {
        const BasicSelector &sel = basicSelectors.at(i);
        if (!sel.elementName.isEmpty())
            val += 1;

        val += (sel.pseudos.count() + sel.attributeSelectors.count()) * 0x10;
        val += sel.ids.count() * 0x100;
    }
    return val;
}

QString Selector::pseudoElement() const
{
    const BasicSelector& bs = basicSelectors.last();
    if (!bs.pseudos.isEmpty() && bs.pseudos.first().type == PseudoClass_Unknown)
        return bs.pseudos.first().name;
    return QString();
}

quint64 Selector::pseudoClass(quint64 *negated) const
{
    const BasicSelector& bs = basicSelectors.last();
    if (bs.pseudos.isEmpty())
        return PseudoClass_Unspecified;
    quint64 pc = PseudoClass_Unknown;
    for (int i = !pseudoElement().isEmpty(); i < bs.pseudos.count(); i++) {
        const Pseudo &pseudo = bs.pseudos.at(i);
        if (pseudo.type == PseudoClass_Unknown)
            return PseudoClass_Unknown;
        if (!pseudo.negated)
            pc |= pseudo.type;
        else if (negated)
            *negated |= pseudo.type;
    }
    return pc;
}

///////////////////////////////////////////////////////////////////////////////
// StyleSelector
StyleSelector::StyleSelector()
{
}

StyleSelector::StyleSelector(const StyleSelector &copy)
{
    medium = copy.medium;
    QVectorIterator<StyleSheet*> iter(copy.styleSheets);
    while(iter.hasNext()){
        StyleSheet *newSheet = HbMemoryUtils::create<HbCss::StyleSheet>(*(iter.next()),
                                                                    HbMemoryManager::HeapMemory);
        addStyleSheet(newSheet);
    }
}

StyleSelector::~StyleSelector()
{
    for(int i=0; i<styleSheets.count(); i++){
        HbMemoryUtils::release<HbCss::StyleSheet>(styleSheets.at(i));
    }
    styleSheets.clear();
    widgetSheets.clear();
}

int StyleSelector::selectorMatches(
    const Selector &selector, NodePtr node, bool nameCheckNeeded) const
{
    if (selector.basicSelectors.isEmpty()) {
        return -1;
    }

    if (selector.basicSelectors.first().relationToNext == BasicSelector::NoRelation) {
        if (selector.basicSelectors.count() != 1) {
            return -1;
        }
        return basicSelectorMatches(selector.basicSelectors.first(), node, nameCheckNeeded);
    }

    if (selector.basicSelectors.count() <= 1) {
        return -1;
    }

    int i = selector.basicSelectors.count() - 1;
    int matchLevel(-1);
    int firstMatchLevel(-1);

    BasicSelector sel = selector.basicSelectors.at(i);
    bool firstLoop = true;
    do {
        matchLevel = basicSelectorMatches(sel, node, (nameCheckNeeded || !firstLoop));
        if (firstLoop) {
            firstMatchLevel = matchLevel;
        }
        if (matchLevel < 0) {
            if (sel.relationToNext == BasicSelector::MatchNextSelectorIfParent
                || i == selector.basicSelectors.count() - 1) { // first element must always match!
                break;
            }
        }

        if (matchLevel >= 0 || sel.relationToNext != BasicSelector::MatchNextSelectorIfAncestor)
            --i;

        if (i < 0) {
            break;
        }

        sel = selector.basicSelectors.at(i);
        if (sel.relationToNext == BasicSelector::MatchNextSelectorIfAncestor
            || sel.relationToNext == BasicSelector::MatchNextSelectorIfParent) {
            node = parentNode(node);
        } else if (sel.relationToNext == BasicSelector::MatchNextSelectorIfPreceeds) {
            node = previousSiblingNode(node);
        }
        if (isNullNode(node)) {
            matchLevel = -1;
            break;
        }
        firstLoop = false;
   } while (i >= 0 && (matchLevel >= 0 || 
       sel.relationToNext == BasicSelector::MatchNextSelectorIfAncestor));

    return (matchLevel < 0) ? -1 : firstMatchLevel;
}

int StyleSelector::inheritanceDepth(NodePtr node, HbString &elementName) const
{
    if (elementName == GLOBAL_CSS_SELECTOR)
        return 0;

    const uint nameHash = qHash(elementName.constData());
    static QHash<uint, int> depths;
    if (depths.contains(nameHash)) {
        return depths[nameHash];
    } else {
        int result = nodeNameEquals(node, elementName);
        depths[nameHash] = result;
        return result;
    }
}

const uint CLASS_HASH = qHash(QString("class"));

int StyleSelector::basicSelectorMatches(
    const BasicSelector &sel, NodePtr node, bool nameCheckNeeded) const
{
    int matchLevel = 0;
    HbString elementName(HbMemoryManager::HeapMemory);

    if (!sel.attributeSelectors.isEmpty()) {
        if (!hasAttributes(node))
            return -1;

        for (int i = 0; i < sel.attributeSelectors.count(); ++i) {
            const AttributeSelector &a = sel.attributeSelectors.at(i);
            if (a.nameHash == CLASS_HASH) {
                elementName = a.value;
            }
            if (!attributeMatches(node, a)) {
                return -1;
            }
        }
    }
    if ( elementName.isEmpty() ) {
        elementName = sel.elementName;
    }

    if (!elementName.isEmpty()) {
        matchLevel = nameCheckNeeded 
            ? nodeNameEquals(node, elementName)
            : inheritanceDepth(node, elementName);
        if ( matchLevel < 0 ) {
            return -1;
        }
    }

    if (!sel.ids.isEmpty() && sel.ids != nodeIds(node)) {
        return -1;
    }

    return matchLevel;
}

static inline bool qcss_selectorStyleRuleLessThan(
    const QPair<int, HbCss::StyleRule> &lhs, const QPair<int, HbCss::StyleRule> &rhs)
{
    return lhs.first < rhs.first;
}

static inline bool qcss_selectorDeclarationLessThan(
    const QPair<int, HbCss::Declaration> &lhs, const QPair<int, HbCss::Declaration> &rhs)
{
    return lhs.first < rhs.first;
}

void StyleSelector::matchRules(
    NodePtr node, const HbVector<StyleRule> &rules, StyleSheetOrigin origin,
    int depth, QVector<WeightedRule> *weightedRules, bool nameCheckNeeded) const
{
    for (int i = 0; i < rules.count(); ++i) {
        const StyleRule &rule = rules.at(i);
        for (int j = 0; j < rule.selectors.count(); ++j) {
            const Selector& selector = rule.selectors.at(j);
            int matchLevel = selectorMatches(selector, node, nameCheckNeeded);
            if ( matchLevel >= 0 ) {
                WeightedRule wRule;
                wRule.first = selector.specificity()
                    + 0x1000* matchLevel
                    + (origin == StyleSheetOrigin_Inline)*0x10000*depth;
                wRule.second.selectors.append(selector);
                wRule.second.declarations = rule.declarations;
#ifdef HB_CSS_INSPECTOR
                wRule.second.owningStyleSheet = rule.owningStyleSheet;
#endif
                weightedRules->append(wRule);
            }
        }
    }
}

// Returns style rules that are in ascending order of specificity
// Each of the StyleRule returned will contain exactly one Selector
HbVector<StyleRule> StyleSelector::styleRulesForNode(
    NodePtr node, const Qt::Orientation orientation) const
{
    HbVector<StyleRule> rules;
    if (styleSheets.isEmpty())
        return rules;

    QVector<WeightedRule> weightedRules = weightedStyleRulesForNode(node, orientation);

    qStableSort(weightedRules.begin(), weightedRules.end(), qcss_selectorStyleRuleLessThan);

    for (int j = 0; j < weightedRules.count(); j++)
        rules += weightedRules.at(j).second;

    return rules;
}


// Returns style rules and specificity values (unordered)
QVector<WeightedRule> StyleSelector::weightedStyleRulesForNode(
    NodePtr node, const Qt::Orientation orientation) const
{
    initNode(node);
    QVector<WeightedRule> weightedRules; // (spec, rule) that will be sorted below

    // Generate inheritance list (reverse order)
    QStringList classNames;
    QGraphicsWidget *widgetPtr = static_cast<QGraphicsWidget*> (node.ptr);
    const QMetaObject *metaObject = widgetPtr->metaObject();
    do {
        const QString className = metaObject->className();
        classNames << className;
        metaObject = metaObject->superClass();
    } while (metaObject != 0);
    classNames << GLOBAL_CSS_SELECTOR;

    // Iterate backwards through list to append most-derived classes last
    int count = classNames.count();
    bool firstLoop = true;
    while(count--){
        const QString &className = classNames.at(count);
        uint classNameHash = qHash(className);
        QVectorIterator<StyleSheet*> iter(widgetSheets[classNameHash]);
        while (iter.hasNext()) {
            const StyleSheet *styleSheet = iter.next();
            if(styleSheet) {
                WidgetStyleRules* widgetStack = styleSheet->widgetStack(classNameHash);
                if (widgetStack) {
                    matchRules(node, widgetStack->styleRules, styleSheet->origin, 
                                styleSheet->depth, &weightedRules, false);
                    // Append orientation-specific rules
                    if (orientation == Qt::Vertical) {
                        matchRules(node, widgetStack->portraitRules, styleSheet->origin, 
                                    styleSheet->depth, &weightedRules, false);
                    }else if (orientation == Qt::Horizontal) {
                        matchRules(node, widgetStack->landscapeRules, styleSheet->origin, 
                                    styleSheet->depth, &weightedRules, false);
                    }
                }
                if (firstLoop && !medium.isEmpty()) { // Media rules are only added to global widget stack
                    int mediaRuleCount = styleSheet->mediaRules.count();
                    for (int i = 0; i < mediaRuleCount; ++i) {
                        if (styleSheet->mediaRules.at(i).media.contains(
                                HbString(medium, HbMemoryManager::HeapMemory),
                                Qt::CaseInsensitive)) {
                            matchRules(node, styleSheet->mediaRules.at(i).styleRules, 
                                styleSheet->origin, styleSheet->depth, &weightedRules);
                        }
                    }
                }// End medium.isEmpty loop
            }// End styleSheet
        }
        firstLoop = false;
    }
    cleanupNode(node);
    return weightedRules;
}

bool StyleSelector::hasOrientationSpecificStyleRules(NodePtr node) const
{
    // Generate inheritance list (reverse order)
    QStringList classNames;
    QGraphicsWidget *widgetPtr = static_cast<QGraphicsWidget*> (node.ptr);
    const QMetaObject *metaObject = widgetPtr->metaObject();
    do {
        const QString className = metaObject->className();
        classNames << className;
        metaObject = metaObject->superClass();
    } while (metaObject != 0);
    classNames << GLOBAL_CSS_SELECTOR;

    int count = classNames.count();
    while (count--) {
        const QString &className = classNames.at(count);
        uint classNameHash = qHash(className);
        QVectorIterator<StyleSheet*> iter(widgetSheets[classNameHash]);
        while (iter.hasNext()) {
            const StyleSheet *styleSheet = iter.next();
            if (styleSheet) {
                WidgetStyleRules* widgetStack = styleSheet->widgetStack(classNameHash);
                if (widgetStack) {
                    if (widgetStack->portraitRules.count() ||
                            widgetStack->landscapeRules.count()) {
                        return true;
                    }
                }
            }// End styleSheet
        }
    }
    return false;
}



// Returns declarations and specificity values (unordered)
QVector<WeightedDeclaration> StyleSelector::weightedDeclarationsForNode(
    NodePtr node, 
    const Qt::Orientation orientation,
    const char *extraPseudo) const
{
    QVector<WeightedDeclaration> decls;
    QVector<WeightedRule> rules = weightedStyleRulesForNode(node, orientation);
    for (int i = 0; i < rules.count(); i++) {
        const Selector& selector = rules.at(i).second.selectors.at(0);
        const QString pseudoElement = selector.pseudoElement();

        bool pseudoElementMatches = (extraPseudo && pseudoElement == QLatin1String(extraPseudo));

        // skip rules with non-matching pseudo elements
        if (!pseudoElement.isEmpty() && !pseudoElementMatches)
            continue;

        quint64 pseudoClass = selector.pseudoClass();
        bool pseudoClassIsValid = 
            pseudoClass == PseudoClass_Unspecified
            || pseudoClass == PseudoClass_Landscape 
            || pseudoClass == PseudoClass_Portrait;

        if (pseudoClassIsValid || pseudoElementMatches) {
            HbVector<Declaration> ruleDecls = rules.at(i).second.declarations;
            for (int j=0; j<ruleDecls.count(); j++) {
                WeightedDeclaration wDecl;
                wDecl.first = rules.at(i).first;
                wDecl.second = ruleDecls.at(j);
                decls.append(wDecl);
            }
        }
    }
    return decls;
}

// for qtexthtmlparser which requires just the declarations with Enabled state
// and without pseudo elements
HbVector<Declaration> StyleSelector::declarationsForNode(
    NodePtr node, 
    const Qt::Orientation orientation,
    const char *extraPseudo) const
{
    HbVector<Declaration> decls;
    if (styleSheets.isEmpty())
        return decls;

    QVector<WeightedDeclaration> weightedDecls = 
        weightedDeclarationsForNode(node, orientation, extraPseudo);

    qStableSort(weightedDecls.begin(), weightedDecls.end(), qcss_selectorDeclarationLessThan);

    for (int j = 0; j < weightedDecls.count(); j++)
        decls += weightedDecls.at(j).second;

    return decls;
}

void StyleSelector::variableRuleSets(QHash<QString, HbCss::Declaration> *variables) const 
{
    HbVector<Declaration> decls;
    const int styleSheetsCount = styleSheets.count();
    for (int i=0; i<styleSheetsCount; i++) {
        const StyleSheet *styleSheet = styleSheets.at(i);
        const int variableRuleCount = styleSheet->variableRules.count();
        for (int j=0; j<variableRuleCount; j++) {
            decls = styleSheet->variableRules.at(j).declarations;
            const int declsCount = decls.count();
            for (int k=0; k<declsCount; k++) {
                variables->insert(decls.at(k).property, decls.at(k));
            }
        }
    }
}

void StyleSelector::addStyleSheet( StyleSheet* styleSheet )
{
    styleSheets.append(styleSheet);
    foreach (const HbCss::WidgetStyleRules &wsr, styleSheet->widgetRules) {
        widgetSheets[wsr.classNameHash].append(styleSheet);
    }
}

void StyleSelector::removeStyleSheet( StyleSheet* styleSheet )
{
    styleSheets.remove(styleSheets.indexOf(styleSheet));
    QHash<uint, QVector<HbCss::StyleSheet*> >::iterator iter = widgetSheets.begin();
    while (iter != widgetSheets.end()) {
        int index = iter.value().indexOf(styleSheet);
        if (index != -1) {
            iter.value().remove(index);
        }        
        ++iter;
    }
    HbMemoryUtils::release<HbCss::StyleSheet>(styleSheet);
}



static inline bool isHexDigit(const char c)
{
    return (c >= '0' && c <= '9')
           || (c >= 'a' && c <= 'f')
           || (c >= 'A' && c <= 'F')
           ;
}

QString Scanner::preprocess(const QString &input, bool *hasEscapeSequences)
{
    QString output = input;

    if (hasEscapeSequences)
        *hasEscapeSequences = false;

    int i = 0;
    while (i < output.size()) {
        if (output.at(i) == QLatin1Char('\\')) {

            ++i;
            // test for unicode hex escape
            int hexCount = 0;
            const int hexStart = i;
            while (i < output.size()
                   && isHexDigit(output.at(i).toLatin1())
                   && hexCount < 7) {
                ++hexCount;
                ++i;
            }
            if (hexCount == 0) {
                if (hasEscapeSequences)
                    *hasEscapeSequences = true;
                continue;
            }

            hexCount = qMin(hexCount, 6);
            bool ok = false;
            ushort code = output.mid(hexStart, hexCount).toUShort(&ok, 16);
            if (ok) {
                output.replace(hexStart - 1, hexCount + 1, QChar(code));
                i = hexStart;
            } else {
                i = hexStart;
            }
        } else {
            ++i;
        }
    }
    return output;
}

void Scanner::scan(const QString &preprocessedInput, QVector<Symbol> *symbols)
{
    HbCssScanner_Generated scanner(preprocessedInput);
    Symbol sym;
    int tok = scanner.lex();
    while (tok != -1) {
        sym.token = static_cast<HbCss::TokenType>(tok);
        sym.text = scanner.input;
        sym.start = scanner.lexemStart;
        sym.len = scanner.lexemLength;
        symbols->append(sym);
        tok = scanner.lex();
    }
}

QString Symbol::lexem() const
{
    QString result;
    if (len > 0)
        result.reserve(len);
    for (int i = 0; i < len; ++i) {
        if (text.at(start + i) == QLatin1Char('\\') && i < len - 1)
            ++i;
        result += text.at(start + i);
    }
    return result;
}

Parser::Parser(const QString &css, bool isFile)
{
    init(css, isFile);
}

Parser::Parser()
{
    index = 0;
    errorIndex = -1;
    hasEscapeSequences = false;
}

void Parser::init(const QString &css, bool isFile)
{
    QString styleSheet = css;
    if (isFile) {
        QFile file(css);
        if (file.open(QFile::ReadOnly)) {
            sourcePath = QFileInfo(styleSheet).absolutePath() + QLatin1String("/");
            sourceFile = css;
            QTextStream stream(&file);
            styleSheet = stream.readAll();
        } else {
            qWarning() << "HbCss::Parser - Failed to load file " << css;
            styleSheet.clear();
        }
    } else {
        sourcePath.clear();
        sourceFile.clear();
    }

    hasEscapeSequences = false;
    symbols.resize(0);
    Scanner::scan(Scanner::preprocess(styleSheet, &hasEscapeSequences), &symbols);
    index = 0;
    errorIndex = -1;
    errorCode = NoError;
    symbols.reserve(qMax(symbols.capacity(), symbols.size()));
}

bool Parser::parse(StyleSheet *styleSheet)
{
    errorCode = Parser::UnknownError;
#ifdef HB_CSS_INSPECTOR
    styleSheet->fileName = sourceFile;
#endif
    try {
        if (testTokenAndEndsWith(ATKEYWORD_SYM, QLatin1String("charset"))) {
            if (!next(STRING)) return false;
            if (!next(SEMICOLON)) return false;
        }

        while (test(S) || test(CDO) || test(CDC)) {}

        while (testImport()) {
            ImportRule rule(styleSheet->memoryType);
            if (!parseImport(&rule)) return false;
#ifdef CSS_PARSER_TRACES
            rule.print();
#endif
            styleSheet->importRules.append(rule);
            while (test(S) || test(CDO) || test(CDC)) {}
        }

        do {
            //newly added for variable support
            if (testVariable()) {
                VariableRule rule(styleSheet->memoryType);
                if (!parseVariableset(&rule)) return false;
#ifdef CSS_PARSER_TRACES
                rule.print();
#endif
                styleSheet->variableRules.append(rule);
            }else if (testMedia()) {
                MediaRule rule(styleSheet->memoryType);
                if (!parseMedia(&rule)) return false;
                styleSheet->mediaRules.append(rule);
            } else if (testPage()) {
                PageRule rule(styleSheet->memoryType);
                if (!parsePage(&rule)) return false;
                styleSheet->pageRules.append(rule);
            } else if (testRuleset()) {
                StyleRule rule(styleSheet->memoryType);
                if (!parseRuleset(&rule)) return false;
#ifdef CSS_PARSER_TRACES
                rule.print();
#endif
#ifdef HB_CSS_INSPECTOR
                rule.owningStyleSheet = styleSheet;
#endif
                if(rule.selectors.count() > 1){
                    foreach(const HbCss::Selector &selector, rule.selectors){
                        QString stackName = selector.basicSelectors.last().elementName;
                        if(stackName.length() < 1){
                            stackName = GLOBAL_CSS_SELECTOR;
                        }
                        StyleRule newRule(rule.memoryType);
                        newRule.declarations = rule.declarations;
                        newRule.selectors.append(selector);
#ifdef HB_CSS_INSPECTOR
                        newRule.owningStyleSheet = styleSheet;
#endif
                        addRuleToWidgetStack(styleSheet, stackName, newRule);
                    }
                } else {
                    QString stackName = rule.selectors.at(0).basicSelectors.last().elementName;
                    if(stackName.length() < 1){
                        stackName = GLOBAL_CSS_SELECTOR;
                    }
                    addRuleToWidgetStack(styleSheet, stackName, rule);
                }
            } else if (test(ATKEYWORD_SYM)) {
                if (!until(RBRACE)) return false;
            } else if (hasNext()) {
                return false;
            }
            while (test(S) || test(CDO) || test(CDC)) {}
        } while (hasNext());

    } catch(std::bad_alloc &badAlloc) {
        //an exception happened, probably due to OOM
        Q_UNUSED(badAlloc)
        errorCode = OutOfMemoryError;
        return false;
    }

    errorCode = NoError;
    return true;
}

void Parser::addRuleToWidgetStack(StyleSheet *sheet, const QString &stackName, StyleRule &rule)
{
    uint stackNameHash = qHash(stackName);
    WidgetStyleRules* widgetStack = sheet->widgetStack(stackNameHash);

    if (!widgetStack) {
#ifdef CSSSTACKS_DEBUG
        qDebug() << "Creating stack for classname" << stackName;
#endif
        HbCss::WidgetStyleRules rules(stackNameHash, sheet->memoryType);
        widgetStack = sheet->addWidgetStack(rules);
    }

    // Add rule into correct (portrait/landscape/any) list
    quint64 negated = 0;
    quint64 pseudo = rule.selectors.last().pseudoClass(&negated);
  
    if (((pseudo & HbCss::PseudoClass_Portrait) && ((negated & HbCss::PseudoClass_Portrait) == 0))
            || (negated & HbCss::PseudoClass_Landscape)) {
        widgetStack->portraitRules.append(rule);
    } else if (((pseudo & HbCss::PseudoClass_Landscape) && 
            ((negated & HbCss::PseudoClass_Landscape) == 0))
            || (negated & HbCss::PseudoClass_Portrait)) {
        widgetStack->landscapeRules.append(rule);
    } else {
        widgetStack->styleRules.append(rule);
    }
}

Symbol Parser::errorSymbol()
{
    if (errorIndex == -1) return Symbol();
    return symbols.at(errorIndex);
}

static inline void removeOptionalQuotes(HbString *str)
{
    if (!str->startsWith(QLatin1Char('\''))
        && !str->startsWith(QLatin1Char('\"')))
        return;
    str->remove(0, 1);
    str->chop(1);
}

bool Parser::parseImport(ImportRule *importRule)
{
    skipSpace();

    if (test(STRING)) {
        importRule->href = lexem();
    } else {
        if (!testAndParseUri(&importRule->href)) return false;
    }
    removeOptionalQuotes(&importRule->href);

    skipSpace();

    if (testMedium()) {
        if (!parseMedium(&importRule->media)) return false;

        while (test(COMMA)) {
            skipSpace();
            if (!parseNextMedium(&importRule->media)) return false;
        }
    }

    if (!next(SEMICOLON)) return false;

    skipSpace();
    return true;
}

bool Parser::parseMedia(MediaRule *mediaRule)
{
    do {
        skipSpace();
        if (!parseNextMedium(&mediaRule->media)) return false;
    } while (test(COMMA));

    if (!next(LBRACE)) return false;
    skipSpace();

    while (testRuleset()) {
        StyleRule rule(mediaRule->memoryType);
        if (!parseRuleset(&rule)) return false;
#ifdef CSS_PARSER_TRACES        
        rule.print();
#endif
        mediaRule->styleRules.append(rule);
    }

    if (!next(RBRACE)) return false;
    skipSpace();
    return true;
}

bool Parser::parseMedium(HbStringVector *media)
{
    media->append(HbString(lexem(), media->memoryType()));
    skipSpace();
    return true;
}

bool Parser::parsePage(PageRule *pageRule)
{
    skipSpace();

    if (testPseudoPage())
        if (!parsePseudoPage(&pageRule->selector)) return false;

    skipSpace();
    if (!next(LBRACE)) return false;

    do {
        skipSpace();
        Declaration decl(pageRule->memoryType);
        if (!parseNextDeclaration(&decl)) return false;
        if (!decl.isEmpty())
            pageRule->declarations.append(decl);
    } while (test(SEMICOLON));

    if (!next(RBRACE)) return false;
    skipSpace();
    return true;
}

bool Parser::parsePseudoPage(HbString *selector)
{
    if (!next(IDENT)) return false;
    *selector = lexem();
    return true;
}

bool Parser::parseNextOperator(Value *value)
{
    if (!hasNext()) return true;
    switch (next()) {
        case SLASH: value->type = Value::TermOperatorSlash; skipSpace(); break;
        case COMMA: value->type = Value::TermOperatorComma; skipSpace(); break;
        default: prev(); break;
    }
    return true;
}

bool Parser::parseCombinator(BasicSelector::Relation *relation)
{
    *relation = BasicSelector::NoRelation;
    if (lookup() == S) {
        *relation = BasicSelector::MatchNextSelectorIfAncestor;
        skipSpace();
    } else {
        prev();
    }
    if (test(PLUS)) {
        *relation = BasicSelector::MatchNextSelectorIfPreceeds;
    } else if (test(GREATER)) {
        *relation = BasicSelector::MatchNextSelectorIfParent;
    }
    skipSpace();
    return true;
}

bool Parser::parseProperty(Declaration *decl)
{
    decl->property = lexem();
    decl->propertyId = static_cast<Property>(findKnownValue(
        decl->property, properties, NumProperties));
    skipSpace();
    return true;
}




bool Parser::parseVariableset(VariableRule *variableRule)
{
//no selector needs to be identified
//declarations part

    skipSpace();
    if (!next(LBRACE)) return false;
    const int declarationStart = index;

    do {
        skipSpace();
        Declaration decl(variableRule->memoryType);
        const int rewind = index;
        if (!parseNextDeclaration(&decl)) {
            index = rewind;
            const bool foundSemicolon = until(SEMICOLON);
            const int semicolonIndex = index;

            index = declarationStart;
            const bool foundRBrace = until(RBRACE);

            if (foundSemicolon && semicolonIndex < index) {
                decl = Declaration(variableRule->memoryType);
                index = semicolonIndex - 1;
            } else {
                skipSpace();
                return foundRBrace;
            }
        }
        if (!decl.isEmpty())
        {
#ifdef CSS_PARSER_TRACES
            decl.print();
#endif
            variableRule->declarations.append(decl);
        }

    } while (test(SEMICOLON));

    if (!next(RBRACE)) return false;
    skipSpace();
    return true;

}



bool Parser::parseRuleset(StyleRule *styleRule)
{
    Selector sel(styleRule->memoryType);
    if (!parseSelector(&sel)) return false;
#ifdef CSS_PARSER_TRACES    
    sel.print();
#endif
    styleRule->selectors.append(sel);

    while (test(COMMA)) {
        skipSpace();
        Selector sel(styleRule->memoryType);
        if (!parseNextSelector(&sel)) return false;
#ifdef CSS_PARSER_TRACES
        sel.print();
#endif
        styleRule->selectors.append(sel);
    }

    skipSpace();
    if (!next(LBRACE)) return false;
    const int declarationStart = index;

    do {
        skipSpace();
        Declaration decl(styleRule->memoryType);
        const int rewind = index;
        if (!parseNextDeclaration(&decl)) {
            index = rewind;
            const bool foundSemicolon = until(SEMICOLON);
            const int semicolonIndex = index;

            index = declarationStart;
            const bool foundRBrace = until(RBRACE);

            if (foundSemicolon && semicolonIndex < index) {
                decl = Declaration(styleRule->memoryType);
                index = semicolonIndex - 1;
            } else {
                skipSpace();
                return foundRBrace;
            }
        }
        if (!decl.isEmpty())
        {    
#ifdef CSS_PARSER_TRACES
            decl.print();
#endif
            styleRule->declarations.append(decl);
        }
    } while (test(SEMICOLON));

    if (!next(RBRACE)) return false;
    skipSpace();
    return true;
}

bool Parser::parseSelector(Selector *sel)
{
    BasicSelector basicSel(sel->memoryType);
    if (!parseSimpleSelector(&basicSel)) return false;
    while (testCombinator()) {
        if (!parseCombinator(&basicSel.relationToNext)) return false;

        if (!testSimpleSelector()) break;
        sel->basicSelectors.append(basicSel);

        basicSel = BasicSelector(sel->memoryType);
        if (!parseSimpleSelector(&basicSel)) return false;
    }
#ifdef CSS_PARSER_TRACES
    basicSel.print();
#endif
    sel->basicSelectors.append(basicSel);
    return true;
}

bool Parser::parseSimpleSelector(BasicSelector *basicSel)
{
    int minCount = 0;
    if (lookupElementName()) {
        if (!parseElementName(&basicSel->elementName)) return false;
    } else {
        prev();
        minCount = 1;
    }
    bool onceMore;
    int count = 0;
    do {
        onceMore = false;
        if (test(HASH)) {
            QString id = lexem();
            // chop off leading #
            id.remove(0, 1);
            basicSel->ids.append(HbString(id, basicSel->memoryType));
            onceMore = true;
        } else if (testClass()) {
            onceMore = true;
            AttributeSelector a(basicSel->memoryType);
            a.name = QLatin1String("class");
            a.nameHash = qHash(a.name);
            a.valueMatchCriterium = AttributeSelector::MatchContains;
            if (!parseClass(&a.value)) return false;
#ifdef CSS_PARSER_TRACES
            a.print();
#endif
            basicSel->attributeSelectors.append(a);
        } else if (testAttrib()) {
            onceMore = true;
            AttributeSelector a(basicSel->memoryType);
            if (!parseAttrib(&a)) return false;
#ifdef CSS_PARSER_TRACES
            a.print();
#endif
            basicSel->attributeSelectors.append(a);
        } else if (testPseudo()) {
            onceMore = true;
            Pseudo ps(basicSel->memoryType);
            if (!parsePseudo(&ps)) return false;
#ifdef CSS_PARSER_TRACES
            ps.print();
#endif
            basicSel->pseudos.append(ps);
        }
        if (onceMore) ++count;
    } while (onceMore);
    return count >= minCount;
}

bool Parser::parseClass(HbString *name)
{
    if (!next(IDENT)) return false;
    *name = lexem();
    return true;
}

bool Parser::parseElementName(HbString *name)
{
    switch (lookup()) {
        case STAR: name->clear(); break;
        case IDENT: *name = lexem(); break;
        default: return false;
    }
    return true;
}

bool Parser::parseAttrib(AttributeSelector *attr)
{
    skipSpace();
    attr->negated = false;
    if (test(EXCLAMATION_SYM)) {
        attr->negated = true;
    }

    if (!next(IDENT)) return false;
    attr->name = lexem();
    attr->nameHash = qHash(attr->name);
    skipSpace();

    if (test(EXCLAMATION_SYM)) {
        attr->negated = !(attr->negated);
    }

    if (test(EQUAL)) {
        attr->valueMatchCriterium = AttributeSelector::MatchEqual;
    } else if (test(INCLUDES)) {
        attr->valueMatchCriterium = AttributeSelector::MatchContains;
    } else if (test(DASHMATCH)) {
        attr->valueMatchCriterium = AttributeSelector::MatchBeginsWith;
    } else {
        return next(RBRACKET);
    }

    skipSpace();

    if (!test(IDENT) && !test(STRING)) return false;
    attr->value = unquotedLexem();

    skipSpace();
    return next(RBRACKET);
}

bool Parser::parsePseudo(Pseudo *pseudo)
{
    test(COLON);
    pseudo->negated = test(EXCLAMATION_SYM);
    if (test(IDENT)) {
        pseudo->name = lexem();
        pseudo->type = static_cast<quint64>(findKnownValue((pseudo->name), pseudos, NumPseudos));
        return true;
    }
    if (!next(FUNCTION)) return false;
    pseudo->function = lexem();
    // chop off trailing parenthesis
    pseudo->function.chop(1);
    skipSpace();
    if (!test(IDENT)) return false;
    pseudo->name = lexem();
    skipSpace();
    return next(RPAREN);
}

bool Parser::parseNextDeclaration(Declaration *decl)
{
    if (!testProperty())
        return true; // not an error!
    if (!parseProperty(decl)) return false;
    if (!next(COLON)) return false;
    skipSpace();
    if (!parseNextExpr(&decl->values)) return false;
    if (testPrio())
        if (!parsePrio(decl)) return false;
    return true;
}

bool Parser::testPrio()
{
    const int rewind = index;
    if (!test(EXCLAMATION_SYM)) return false;
    skipSpace();
    if (!test(IDENT)) {
        index = rewind;
        return false;
    }
    if (lexem().compare(QLatin1String("important"), Qt::CaseInsensitive) != 0) {
        index = rewind;
        return false;
    }
    return true;
}

bool Parser::parsePrio(Declaration *declaration)
{
    declaration->important = true;
    skipSpace();
    return true;
}

bool Parser::parseExpr(HbVector<Value> *values)
{
    Value val(values->memoryType());
    if (!parseTerm(&val)) return false;
#ifdef CSS_PARSER_TRACES
    val.print();
#endif
    values->append(val);

    bool onceMore;
    do {
        onceMore = false;
        val = Value(values->memoryType());
        if (!parseNextOperator(&val)) return false;
        if (val.type != HbCss::Value::Unknown) {
#ifdef CSS_PARSER_TRACES
            val.print();
#endif
            values->append(val);
        }
        if (testTerm()) {
            onceMore = true;
            val = Value(values->memoryType());
            if (!parseTerm(&val)) return false;
#ifdef CSS_PARSER_TRACES
            val.print();
#endif
            values->append(val);
        }
    } while (onceMore);
    return true;
}

bool Parser::testTerm()
{
    return test(PLUS) || test(MINUS)
           || test(NUMBER)
           || test(PERCENTAGE)
           || test(LENGTH)
           || test(STRING)
           || test(IDENT)
           || testHexColor()
           || testFunction();
}

bool Parser::parseTerm(Value *value)
{
    QString str = lexem();
    bool haveUnary = false;
    if (lookup() == PLUS || lookup() == MINUS) {
        haveUnary = true;
        if (!hasNext()) return false;
        next();
        str += lexem();
    }

    value->variant = str;
    value->type = HbCss::Value::String;
    switch (lookup()) {
        case NUMBER:
            value->type = Value::Number;
            value->variant.convert(HbVariant::Double);
            break;
        case PERCENTAGE:
            value->type = Value::Percentage;
            str.chop(1); // strip off %
            value->variant = str;
            break;
        case LENGTH:
            value->type = Value::Length;
            break;

        case STRING:
            if (haveUnary) return false;
            value->type = Value::String;
            str.chop(1);
            str.remove(0, 1);
            value->variant = str;
            break;
        case IDENT: {
            if (haveUnary) return false;
            value->type = Value::Identifier;
            const int id = findKnownValue(str, values, NumKnownValues);
            if (id != 0) {
                value->type = Value::KnownIdentifier;
                value->variant = id;
                value->original = str;
            }
            break;
        }
        default: {
            if (haveUnary) return false;
            prev();
            if (testHexColor()) {
                QColor col;
                if (!parseHexColor(&col)) return false;
                value->type = Value::Color;
                value->variant = col;
            } else if (testFunction()) {
                HbString name(value->memoryType), args(value->memoryType);
                if (!parseFunction(&name, &args)) return false;
                if (name == QLatin1String("url")) {
                    value->type = Value::Uri;
                    removeOptionalQuotes(&args);
                    if (QFileInfo(args).isRelative() && !sourcePath.isEmpty()) {
                        args.prepend(sourcePath);
                    }
                    value->variant = args;
                }
                //changes for variable support
                else if (name == QLatin1String("var")) {
                    value->type = Value::Variable;
                    value->variant = args;
                } else if (name == QLatin1String("-var")) {                    
                    value->type = Value::VariableNegative;
                    value->variant = args;
                } //change end
                //changes for expression support
                else if (name == QLatin1String("expr")) {
                    value->type = Value::Expression;
                    value->variant = args;
                } else if (name == QLatin1String("-expr")) {
                    value->type = Value::ExpressionNegative;
                    value->variant = args;
                } //change end
                else {
                    value->type = Value::Function;
                    value->variant = QStringList() << name << args;
                }
            } else {
                return recordError();
            }
            return true;
        }
    }
    skipSpace();
    return true;
}

bool Parser::parseFunction(HbString *name, HbString *args)
{
    *name = lexem();
    name->chop(1);
    skipSpace();
    const int start = index;
    if (!until(RPAREN)) return false;
    for (int i = start; i < index - 1; ++i)
        args->append(symbols.at(i).lexem());
    /*
    if (!nextExpr(&arguments)) return false;
    if (!next(RPAREN)) return false;
    */
    skipSpace();
    return true;
}

bool Parser::parseHexColor(QColor *col)
{
    col->setNamedColor(lexem());
    if (!col->isValid()) return false;
    skipSpace();
    return true;
}

bool Parser::testAndParseUri(HbString *uri)
{
    const int rewind = index;
    if (!testFunction()) return false;

    HbString name(uri->memoryType()), args(uri->memoryType());
    if (!parseFunction(&name, &args)) {
        index = rewind;
        return false;
    }
    if (name.toLower() != QLatin1String("url")) {
        index = rewind;
        return false;
    }
    *uri = args;
    removeOptionalQuotes(uri);
    return true;
}

bool Parser::testSimpleSelector()
{
    return testElementName()
           || (test(HASH))
           || testClass()
           || testAttrib()
           || testPseudo();
}

bool Parser::next(HbCss::TokenType t)
{
    if (hasNext() && next() == t)
        return true;
    return recordError();
}

bool Parser::test(HbCss::TokenType t)
{
    if (index >= symbols.count())
        return false;
    if (symbols.at(index).token == t) {
        ++index;
        return true;
    }
    return false;
}

QString Parser::unquotedLexem() const
{
    QString s = lexem();
    if (lookup() == STRING) {
        s.chop(1);
        s.remove(0, 1);
    }
    return s;
}

QString Parser::lexemUntil(HbCss::TokenType t)
{
    QString lexem;
    while (hasNext() && next() != t)
        lexem += symbol().lexem();
    return lexem;
}

bool Parser::until(HbCss::TokenType target, HbCss::TokenType target2)
{
    int braceCount = 0;
    int brackCount = 0;
    int parenCount = 0;
    if (index) {
        switch(symbols.at(index-1).token) {
        case LBRACE: ++braceCount; break;
        case LBRACKET: ++brackCount; break;
        case FUNCTION:
        case LPAREN: ++parenCount; break;
        default: ;
        }
    }
    while (index < symbols.size()) {
        HbCss::TokenType t = symbols.at(index++).token;
        switch (t) {
        case LBRACE: ++braceCount; break;
        case RBRACE: --braceCount; break;
        case LBRACKET: ++brackCount; break;
        case RBRACKET: --brackCount; break;
        case FUNCTION:
        case LPAREN: ++parenCount; break;
        case RPAREN: --parenCount; break;
        default: break;
        }
        if ((t == target || (target2 != NONE && t == target2))
            && braceCount <= 0
            && brackCount <= 0
            && parenCount <= 0)
            return true;

        if (braceCount < 0 || brackCount < 0 || parenCount < 0) {
            --index;
            break;
        }
    }
    return false;
}

bool Parser::testTokenAndEndsWith(HbCss::TokenType t, const QLatin1String &str)
{
    if (!test(t)) return false;
    if (!lexem().endsWith(str, Qt::CaseInsensitive)) {
        prev();
        return false;
    }
    return true;
}


#ifdef CSS_PARSER_TRACES
static const QString what(Value::Type t)
{
    QString returnString;
    switch(t) {
    case Value::Unknown: 
        returnString = QString("Unknown");
        break;
    case Value::Number:
        returnString = QString("Number");
        break;
    case Value::Percentage:
        returnString = QString("Percentage");
        break;
    case Value::Length:
        returnString = QString("Length");
        break;
    case Value::String:
        returnString = QString("String");
        break;
    case Value::Identifier:
        returnString = QString("Identifier");
        break;
    case Value::KnownIdentifier:
        returnString = QString("KnownIdentifier");
        break;
    case Value::Uri:
        returnString = QString("Uri");
        break;
    case Value::Color:
        returnString = QString("Color");
        break;
    case Value::Function:
        returnString = QString("Function");
        break;
    case Value::TermOperatorSlash:
        returnString = QString("TermOperatorSlash");
        break;
    case Value::TermOperatorComma:
        returnString = QString("TermOperatorComma");
        break;
    case Value::Variable:
        returnString = QString("Variable");
        break;
    default:
        break;
    }
    return returnString;
}

void Value::print() const
{
    qDebug() <<"\t \t \t"<<"==============Value::Print():Begin==================";
    qDebug() <<"\t \t \t"<< "Value::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() <<"\t \t \t"<< "Value::Type type = " << what(type);
    qDebug() <<"\t \t \t"<< "Value::HbString original = " << original;
    qDebug() <<"\t \t \t"<< "Value::HbVariant variant = " << variant.toString();
    qDebug() <<"\t \t \t"<<"==============Value::Print():End====================";
}

void Declaration::print() const
{
    qDebug() <<"\t"<<"==============Declaration::Print():Begin==================";
    qDebug() <<"\t"<<"Declaration::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "\t"<< "Declaration::HbString property = " << property;
    qDebug() << "\t"<< "Declaration::Property propertyId = " << propertyId;
    qDebug() << "\t"<< "Declaration::HbVector<Value> values = " ;
    values.print();
    qDebug() <<"\t"<<"==============Declaration::Print():End====================";
}

void Pseudo::print() const
{
    qDebug() <<"==============Pseudo::Print():Begin==================";
    qDebug() << "Pseudo::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "Pseudo::HbString name= " << name;
    qDebug() << "Pseudo::HbString function = " << function;
    qDebug() <<"==============Pseudo::Print():End==================";    
}

void AttributeSelector::print() const
{
    qDebug() <<"==============AttributeSelector::Print():Begin==================";
    qDebug() << "AttributeSelector::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "AttributeSelector::HbString name= " << name;
    qDebug() << "AttributeSelector::HbString value = " << value;
    qDebug() <<"==============AttributeSelector::Print():End==================";
}

void BasicSelector::print() const
{
    qDebug() <<"\t \t"<<"==============BasicSelector::Print():Begin==================";
    qDebug() <<"\t \t"<<"BasicSelector::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() <<"\t \t"<<"BasicSelector::HbString elementName= " << elementName;
    //qDebug() <<"\t \t"<<"BasicSelector::QStringList ids = " << ids;
    qDebug() <<"\t \t"<<"BasicSelector::PseudoVector pseudos = ";
    pseudos.print();
    qDebug() <<"\t \t"<< "BasicSelector::AttributeSelectorVector attributeSelectors = ";
    attributeSelectors.print();
    qDebug() <<"\t \t"<<"==============BasicSelector::Print():End====================";
}

void Selector::print() const
{
    qDebug() <<"\t "<<"==============Selector::Print():Begin==================";
    qDebug() <<"\t "<<"Selector::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() <<"\t "<<"Selector::BasicSelectorVector basicSelectors= ";
    basicSelectors.print();    
    qDebug() <<"\t "<<"==============Selector::Print():End==================";
}

void StyleRule::print() const
{
    qDebug() <<"==============StyleRule::Print():Begin==================";
    qDebug() << "StyleRule::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "StyleRule::SelectorVector selectors = ";
    selectors.print();
    qDebug() << "StyleRule::DeclarationVector declarations = ";
    declarations.print();
    qDebug() <<"==============StyleRule::Print():End==================";
}

void VariableRule::print() const
{
    qDebug() <<"==============VariableRule::Print():Begin==================";
    qDebug() << "VariableRule::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "VariableRule::DeclarationVector declarations = ";
    declarations.print();
    qDebug() <<"==============VariableRule::Print():End==================";
}

void MediaRule::print() const
{
    qDebug() <<"==============MediaRule::Print():Begin==================";
    qDebug() << "MediaRule::HbMemoryManager::MemoryType memoryType = " << memoryType;
    //qDebug() << "MediaRule::QStringList media = " << media;
    qDebug() << "MediaRule::StyleRuleVector styleRules = ";
    styleRules.print();
    qDebug() <<"==============MediaRule::Print():End==================";
}

void PageRule::print() const
{
    qDebug() <<"==============PageRule::Print():Begin==================";
    qDebug() << "PageRule::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "PageRule::HbString selector = " << selector;
    qDebug() << "PageRule::DeclarationVector declarations = ";
    declarations.print();
    qDebug() <<"==============PageRule::Print():End==================";
}

void ImportRule::print() const
{
    qDebug() <<"==============ImportRule::Print():Begin==================";
    qDebug() << "ImportRule::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "ImportRule::HbString href = " << href;
    //qDebug() << "ImportRule::QStringList media = " << media;
    qDebug() <<"==============ImportRule::Print():End==================";
}

void WidgetStyleRules::print() const
{
    qDebug() <<"==============WidgetStyleRules::Print():Begin==================";
    qDebug() << "Generic rules:";
    styleRules.print();
    qDebug() << "Portrait rules:";
    portraitRules.print();
    qDebug() << "Landscape rules:";
    landscapeRules.print();
    qDebug() <<"==============WidgetStyleRules::Print():End==================";
}

void StyleSheet::print() const
{
    qDebug() <<"==============StyleSheet::Print():Begin==================";
    qDebug() << "StyleSheet::HbMemoryManager::MemoryType memoryType = " << memoryType;
    qDebug() << "StyleSheet::VariableRuleVector variableRules = ";
    variableRules.print();
    qDebug() << "StyleSheet::WidgetStyleRuleVector widgetRules = ";
    widgetRules.print();
    qDebug() << "StyleSheet::MediaRuleVector mediaRules = ";
    mediaRules.print();
    qDebug() << "StyleSheet::PageRulesVector pageRules = ";
    pageRules.print();
    qDebug() << "StyleSheet::ImportRuleVector importRules = ";
    importRules.print();
    qDebug() <<"==============StyleSheet::Print():End==================";
}

#endif // CSS_PARSER_TRACES

//QT_END_NAMESPACE
