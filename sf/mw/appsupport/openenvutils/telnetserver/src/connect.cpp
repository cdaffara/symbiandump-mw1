// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// dhcp client to start a connection.
// 
//

/*
* ==============================================================================
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this 
   list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution. 
 * Neither the name of the <ORGANIZATION> nor the names of its contributors 
   may be used to endorse or promote products derived from this software 
   without specific prior written permission. 
   
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* ==============================================================================
*/

#include <e32cmn.h>
//
#include <es_sock.h>
#include <e32std.h>

//For ipconfig
#include <e32cons.h>
#include <in_sock.h>
#include <in_iface.h>
#include <nifman.h>


// dhcp client - To initiate a request for an IP to a
// dhcp server on the same network(via ethernet)

#ifdef __cplusplus
extern "C" {
#endif

void GetIpFromDHCPServer(void);
void ProgramL(CConsoleBase& aCons);
void PrintFeature(CConsoleBase& aCons, TUint& aMask, TUint aFeature, const TDesC& aDes);


TInt getConnectionUp()
    {
	__UHEAP_MARK;
	// Standard stuff
	CTrapCleanup* trap = CTrapCleanup::New();
	if(trap==NULL)
		return KErrNoMemory;

	CConsoleBase* cons=NULL;
	TRAPD(e, cons=Console::NewL(_L("Telnetd ipconfig utility"),TSize(KConsFullScreen,KConsFullScreen));)
	if(e!=KErrNone)
		{
		delete trap;
		return e;
		}

	TRAPD(res, ProgramL(*cons);)
	if(res!=KErrNone)
		{
		cons->Printf(_L("Error %d\n"), res);
		cons->Printf(_L("Press any key to exit"));
		}

	cons->Getch();
	
	delete cons;
	delete trap;

	__UHEAP_MARKEND;
	return res;
    }

void PrintFeature(CConsoleBase& aCons, TUint& aMask, TUint aFeature, const TDesC& aDes)
	{
	if(aMask&aFeature)
		{
		aMask&=~aFeature;
		aCons.Printf(aDes);
		if(aMask)
			aCons.Printf(_L(","));
		}
	}
void GetIpFromDHCPServer(void)
    {
    RSocketServ rSockServer;
    if(KErrNone==rSockServer.Connect())
        {
        RConnection rConnect;
        if(KErrNone==rConnect.Open(rSockServer))
            {
            TRequestStatus status; 
            rConnect.Start(status);
                
            User::WaitForRequest(status); 
            }
        } 
    }


	void ProgramL(CConsoleBase& aCons)
    {
	GetIpFromDHCPServer();
    TAutoClose<RSocketServ> ss;
    User::LeaveIfError(ss.iObj.Connect());
    ss.PushL();

    TAutoClose<RSocket> sock;
    User::LeaveIfError(sock.iObj.Open(ss.iObj, _L("udp")));
    sock.PushL();

    User::LeaveIfError(sock.iObj.SetOpt(KSoInetEnumInterfaces, KSolInetIfCtrl));

    TProtocolDesc in;
    User::LeaveIfError(sock.iObj.Info(in));
    aCons.Printf(_L("Mobile device IP Configuration:\n\n"));

    TPckgBuf<TSoInetInterfaceInfo> info, next;

    TInt res=sock.iObj.GetOpt(KSoInetNextInterface, KSolInetIfCtrl, info);
    if(res!=KErrNone)
        User::Leave(res);
    TBool hasIP=EFalse;
	while(res==KErrNone)
        {
		
        res=sock.iObj.GetOpt(KSoInetNextInterface, KSolInetIfCtrl, next);
		// || info().iAddress.IsLinkLocal() || info().iAddress.IsSiteLocal()
	if(info().iState != EIfUp || info().iFeatures&KIfIsLoopback  || (info().iAddress.Family() == KAfInet6 && info().iAddress.IsV4Mapped() == EFalse)) //(info().iAddress.IsV4Compat() == EFalse || 
		{
		info = next; continue;	
		}
	hasIP=1;
		aCons.Printf(_L("%S:\n"), &info().iName);
        
       
            TName address;
	        info().iAddress.Output(address);
	        aCons.Printf(_L("IP Address:%S\n"),&address);
       
        
        info().iNetMask.Output(address);
        aCons.Printf(_L("Subnet mask:%S\n\n"), &address);
        
        if(res==KErrNone)
            {
            info = next;
         	}
        }
    if(hasIP == EFalse)
	{
	aCons.Printf(_L("No IP address obtained."));
	User::Leave(KErrNotFound );
	}
	aCons.Printf(_L("Telnetd to start on port 23.\nPress any key..."));
    sock.Pop();
    ss.Pop();
    }	
#ifdef __cplusplus
} //extern "C" {
#endif
