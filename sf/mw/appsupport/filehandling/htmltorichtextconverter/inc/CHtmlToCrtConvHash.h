// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __CHTMLTOCRTCONVHASH_H__
#define __CHTMLTOCRTCONVHASH_H__

enum THtmlToCrtConvTagType
	{
	EHtmlTagUnknown = -1,
	EHtmlTagH1,
	EHtmlTagH2,
	EHtmlTagH3,
	EHtmlTagH4,
	EHtmlTagH5,
	EHtmlTagH6,
	EHtmlTagParagraph,
	EHtmlTagUnorderedList,
	EHtmlTagCenter,
	EHtmlTagBlockquote,
	EHtmlTagItalic,
	EHtmlTagBold,
	EHtmlTagUnderline,
	EHtmlTagStrikethrough,
	EHtmlTagSubscript,
	EHtmlTagSuperscript,
	EHtmlTagEmphasis,
	EHtmlTagStrong,
	EHtmlTagBreak,
	EHtmlTagHtml,
	EHtmlTagHead,
	EHtmlTagTitle,
	EHtmlTagStyle,
	EHtmlTagScript,
	EHtmlTagBase,
	EHtmlTagMeta,
	EHtmlTagLink,
	EHtmlTagBody,
	EHtmlTagAddress,
	EHtmlTagOrderedList,
	EHtmlTagDefinitionList,
	EHtmlTagListItem,
	EHtmlTagDefinitionTerm,
	EHtmlTagDefinitionDefinition,
	EHtmlTagPreformatted,
	EHtmlTagDivision,
	EHtmlTagForm,
	EHtmlTagIsindex,
	EHtmlTagHorizontalRule,
	EHtmlTagTable,
	EHtmlTagCaption,
	EHtmlTagTableRow,
	EHtmlTagTableHeader,
	EHtmlTagTableData,
	EHtmlTagDirectory,
	EHtmlTagMenu,
	EHtmlTagTeletype,
	EHtmlTagBig,
	EHtmlTagSmall,
	EHtmlTagDefinition,
	EHtmlTagCode,
	EHtmlTagSample,
	EHtmlTagKeyboard,
	EHtmlTagVariable,
	EHtmlTagCitation,
	EHtmlTagInput,
	EHtmlTagSelect,
	EHtmlTagOption,
	EHtmlTagTextarea,
	EHtmlTagAnchor,
	EHtmlTagImage,
	EHtmlTagApplet,
	EHtmlTagFont,
	EHtmlTagBasefont,
	EHtmlTagMap,
	EHtmlTagArea
	};

enum THtmlToCrtConvAttributeType
	{
	EHtmlAttributeUnknown = -1,
	EHtmlAttributeAlign,
	EHtmlAttributePrompt,
	EHtmlAttributeHref,
	EHtmlAttributeName,
	EHtmlAttributeContent,
	EHtmlAttributeRel,
	EHtmlAttributeRev,
	EHtmlAttributeTitle,
	EHtmlAttributeBackground,
	EHtmlAttributeBgcolor,
	EHtmlAttributeText,
	EHtmlAttributeLink,
	EHtmlAttributeVlink,
	EHtmlAttributeAlink,
	EHtmlAttributeCompact,
	EHtmlAttributeType,
	EHtmlAttributeStart,
	EHtmlAttributeValue,
	EHtmlAttributeWidth,
	EHtmlAttributeAction,
	EHtmlAttributeMethod,
	EHtmlAttributeEnctype,
	EHtmlAttributeNoshade,
	EHtmlAttributeSize,
	EHtmlAttributeBorder,
	EHtmlAttributeCellspacing,
	EHtmlAttributeCellpadding,
	EHtmlAttributeValign,
	EHtmlAttributeNowrap,
	EHtmlAttributeRowspan,
	EHtmlAttributeColspan,
	EHtmlAttributeHeight,
	EHtmlAttributeChecked,
	EHtmlAttributeMaxlength,
	EHtmlAttributeSrc,
	EHtmlAttributeMultiple,
	EHtmlAttributeSelected,
	EHtmlAttributeRows,
	EHtmlAttributeCols,
	EHtmlAttributeAlt,
	EHtmlAttributeHspace,
	EHtmlAttributeVspace,
	EHtmlAttributeUsemap,
	EHtmlAttributeIsmap,
	EHtmlAttributeCodebase,
	EHtmlAttributeCode,
	EHtmlAttributeColor,
	EHtmlAttributeClear,
	EHtmlAttributeShape,
	EHtmlAttributeCoords,
	EHtmlAttributeNohref
	};

//Character Entities
_LIT16(KHtmlEntityUnknown, "");
_LIT16(KHtmlEntitySpace, " ");
_LIT16(KHtmlEntityLt, "<");
_LIT16(KHtmlEntityGt, ">");

_LIT16(KHtmlGt, "GT");
_LIT16(KHtmlLt, "LT");
_LIT16(KHtmlNbsp, "NBSP");

#endif
