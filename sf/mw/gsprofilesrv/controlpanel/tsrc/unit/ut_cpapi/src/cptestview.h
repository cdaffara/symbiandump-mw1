#ifndef	CPTESTVIEW_H 
#define	CPTESTVIEW_H

#include <cpbasesettingview.h>
#include <QGraphicsItem>

class HbDataFormModelItem;
class CpSettingFormItemData;

class CpTestView : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit CpTestView(QGraphicsItem *parent = 0);
    virtual ~CpTestView();
public:
    void testClose();
};
#endif

