#ifndef	FIRSTPLUGINVIEWFORTEST_H 
#define	FIRSTPLUGINVIEWFORTEST_H

#include <cpbasesettingview.h>
#include <QGraphicsItem>

class HbDataFormModelItem;
class CpSettingFormItemData;

class FirstPluginViewForTest : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit FirstPluginViewForTest(QGraphicsItem *parent = 0);
    virtual ~FirstPluginViewForTest();
public:
    void testClose();
private slots:   
    void sliderValueChanged(int value);
    
private:
    HbDataFormModelItem   *mGroupItem;
    CpSettingFormItemData *mSliderItem;
};
#endif
