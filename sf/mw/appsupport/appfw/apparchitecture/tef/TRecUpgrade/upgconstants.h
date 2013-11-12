// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



/**
 @file
 @internalComponent - Internal Symbian test code
*/

#if !defined(__RECUPGRADECONSTANTS_H__)
#define __RECUPGRADECONSTANTS_H__


_LIT(KUpgDocPath, "Z:\\system\\data\\TApsRecogUpgradeTest.upg");
_LIT(KUprDocPath, "Z:\\system\\data\\TApsRecogUpgradeTest.upr");
_LIT(KUpgRscTargetDir,"C:\\resource\\plugins\\");
_LIT(KUpgDllTargetDir,"C:\\sys\\bin\\");
_LIT(KUpgRscSourcePath, "Z:\\system\\data\\recupgrade.rsc");
_LIT(KUprRscSourcePath, "Z:\\system\\data\\recupgrade2.rsc");
_LIT(KUpgRscTargetPath,"C:\\resource\\plugins\\recupgrade.rsc");
_LIT(KUpgDllTargetPath, "C:\\sys\\bin\\recupgrade.dll");
_LIT(KUpgDllSourcePathArmv5, "Z:\\system\\data\\recupgrade_armv5_rel.dll");
_LIT(KUprDllSourcePathArmv5, "Z:\\system\\data\\recupgrade2_armv5_rel.dll");
_LIT8(KMimeUpgType,"mime/upg");
_LIT8(KMimeUprType,"mime/upr");

#define KUidUpgradeRecognizerValue 0xA000B461

const TUid KUidUpgradeRecognizer={KUidUpgradeRecognizerValue};
const TInt KNumUpgradeTypes=1;
	
#endif
