@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of the License "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem
Rem Create a private key fo rthe CA:
openssl req -nodes -newkey rsa:1024 -keyout %SECURITYSOURCEDIR%\installinstallationservices\switestfw\testcertificates\swi\test\tsisfile\data\signedsis\SymbianTestRootExpiredCARSA\cakey.pem -out %SECURITYSOURCEDIR%\installinstallationservices\switestfw\testcertificates\swi\test\tsisfile\data\signedsis\SymbianTestRootExpiredCARSA\careq.pem -config rsa.config

date 01-01-01
Rem Create the CA certificate:
openssl x509 -req -in %SECURITYSOURCEDIR%\installinstallationservices\switestfw\testcertificates\swi\test\tsisfile\data\signedsis\SymbianTestRootExpiredCARSA\careq.pem -extfile rsa.config -extensions v3_ca -signkey %SECURITYSOURCEDIR%\installinstallationservices\switestfw\testcertificates\swi\test\tsisfile\data\signedsis\SymbianTestRootExpiredCARSA\cakey.pem -out %SECURITYSOURCEDIR%\installationservices\switestfw\testcertificates\swi\test\tsisfile\data\signedsis\SymbianTestRootExpiredCARSA\cacert.pem -days 1

