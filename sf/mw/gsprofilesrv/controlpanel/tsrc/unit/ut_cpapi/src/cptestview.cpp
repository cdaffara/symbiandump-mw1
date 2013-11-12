#include "CpTestView.h"

#include <hbdataform.h>
#include <qstringlist>
#include <QDebug>
#include <hbdataformmodel.h>
#include <cpsettingformitemdata.h>
#include <hbmessagebox.h>

CpTestView::CpTestView(QGraphicsItem *parent) 
    : CpBaseSettingView(0,parent)
{

}
CpTestView::~CpTestView()
{
}

void CpTestView::testClose()
{
    close();
}

//End of File
