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
* Description:  FtuWizard class definition
*
*/


#ifndef FTUWIZARD_H
#define FTUWIZARD_H

#include "ftuwizardmodellib.h"

#include <QFileInfo>
#include <QObject>
#include <QRectF>
#include <QDate>
#include <hbmenu>

class QGraphicsWidget;

/**
 * @ingroup group_ftuwizardmodel
 * @brief Represents a wizard plugin visualization information.
 *
 * FtuWizardSetting struct contains the information needed to construct the GUI in
 * the FTU fw.
 * 
 * @lib ?library
 * @since S60 ?S60_version
 */

typedef struct {
    /**
     * The default icon to be shown in table of contents UI component.
     */ 
    QFileInfo mTocDefaultIcon;
    /**
     * The pressed icon to be shown in table of contents UI component.
     */ 
    QFileInfo mTocPressedIcon;
    /**
     * The focussed icon to be shown in table of contents UI component.
     */ 
    QFileInfo mTocFocussedIcon;
    /**
     * The text to be shown in table of contents UI component.
     */ 
    QString   mTocLabel;
    
    /**
     * Secondary text to be shown in table of contents UI component.
     */ 
    QString   mTocSecondaryLabel;
        
	 /**
     * Informs Framework whether a plugin is having any ftu view or not.
	 * It should be set to false by plugin if it has any FTU view to be shown 
	 * when the plugin is activated. It should be set to true if plugin 
	 * is invoking any service/wizard, when plugin is selected/activated.
     */
    bool mNoViews;
} FtuWizardSetting; 

/**
 * @ingroup group_ftuwizardmodel
 * @brief Represents a wizard plugin in the framework.
 *
 * FtuWizard is an interface through which setting wizard plugin can be 
 * executed in the FTU fw.
 *
 * @lib ?library
 * @since S60 ?S60_version
 */

class FTUWIZARDMODEL_EXPORT FtuWizard : public QObject
{
    Q_OBJECT
public:
    /**
     * The reason why the FTU fw and the wizard plugin is being shut down.
     */
    enum ShutdownReason {MemoryLow, GraphicsMemoryLow, NormalBoot};

public:
    
    /**
     * Destructor.
     * @since S60 ?S60_version.
     */
    virtual ~FtuWizard(){}

    /**
     * Called by the FTU fw during initialize cycle of the wizard, it is during
     * this phase the wizard widget
     * usually creates its initial view.
     * Once the initialization is done, wizardInitialized signal must be 
     * emitted.
 	 * @param cenrepOwnerId Id of the owner of Cenrep holding wizard completion Information.
	 * @param wizardIdx Index of the Cenrep key for a wizard.
	 * If wizard is invoking another application, it should provide cenrepOwnerId and wizardIdx
	 * to the application. Application should write 1 into the wizardIdx cenrep on completion.

     * @since S60 ?S60_version.
     */
    virtual void initializeWizard(qint32 cenrepOwnerId, int wizardIdx) = 0;

    /**
     * Called by the FTU fw when the wizard becomes the current wizard.  
     * @since S60 ?S60_version.
     */
    virtual void activateWizard() = 0;

    /**
     * Called by the FTU fw when the wizard is no longer being displayed.  
     */
    virtual void deactivateWizard() = 0;

    /**
     * Called by the FTU fw before the wizard destructor is called.
     * @param reason The reason why the ftu fw is being killed.
     * @return The mode how wizard plugin is shut down. 
     *         true means the wizard can be deleted immediately.
     *         false means the wizard signals with shutdownDone when it can
     *         be released.
     */
    virtual bool shutdownWizard(ShutdownReason reason) = 0;

    /**
     * Called by the FTU fw to set the wizard size.
     * @param geometry The rectangle allocated to this wizard.
     */
    virtual void resizeWizard(const QRectF& geometry) = 0;

    /**
     * This wizard setting values is queryed by the FTU fw to get the display 
     * properties of the wizard required to be shown in the FTU fw's table of
     * contents view and menu strip (e.g the icon & name of the wizard).
     * The information must be available once the plugin instance has been 
     * created.
     * @return The plugin wizard gui settings. 
     */
    virtual const FtuWizardSetting& wizardSettings() = 0;
    
    /**
     * Called by FTU fw when back button is clicked in the plugin view.
     * In this method the wizard plugin can implement internally showing and
     * hiding GUI elements if the plugin has multiple 'views'.
     * @return true if back event is handled internally in wizard plugin
     *         false if the FTU fw is to handle the event.
     */
    virtual bool handleBackEvent() = 0;
    
     /**
     * Called by FTU fw to get the date when the settings have been saved.
     * @return The date when wizard settings have been completed.
     */    
    virtual QDate wizardCompletedDate() = 0;
    
protected:
    
    /**
     * Constructor.
     * @since S60 ?S60_version.
     */
    FtuWizard(){}
    
    
signals:
    /**
     * Emit this signal to update the text of the info text widget.
     * @param caller The calling wizard plugin instance.
     * @param text The info text to be shown.
     */
    void infoTextUpdated(FtuWizard *caller, QString text);
    
    /**
     * Emit this signal notify the progress on wizard settings.
     * @param caller The calling wizard plugin instance.
     * @param showCompleteness True if the progress is being indicated on the
     *                         UI.
     * @param completenessPercentage The progress of the settings operation.
     */
    void progressUpdated(FtuWizard *caller, bool showProgess, 
                         int progressPercentage);
    
    /**
     * Emit this signal when changing the view inside the wizard plugin.
     * @param caller The calling wizard plugin instance.
     * @param viewWidget The new plugin view widget to be shown.
    */
    void viewChanged(FtuWizard *caller, QGraphicsWidget* viewWidget);
    
    /**
     * Adds an over-lay widget on top of the current view, used to add an 
     * overlay animation on top of the current view
     * @param caller The calling wizard plugin instance.
     * @param overlay The overlay widget to be added.
     */    
    void overlayAdded(FtuWizard *caller, QGraphicsWidget* overlay);
    
    /**
     * Removes the overlay widget added by addOverlay signal. 
     * @param caller The calling wizard plugin instance.
     * @param overlay The overlay widget to be removed.
     */
    void overlayRemoved(FtuWizard *caller, QGraphicsWidget* overlay);
    
    /**
     * Emit this signal to indicate initialization is done. 
     * @param caller The calling wizard plugin instance.
     * @param success True if succeeded, false otherwise.
     */
    void wizardInitialized(FtuWizard *caller, bool success);
    
    /**
     * Emit this signal to indicate that the wizard plugin can be deleted. 
     * Note that this signal does not need to be emitted if true is returned
     * from shutdownWizard.
     * @param caller The calling wizard plugin instance.
     */
    void shutdownCompleted(FtuWizard *caller);
    
    /**
     * Emit this signal to indicate that the wizard plugin wants a custom view menu to be shown. 
     * from shutdownWizard.
     * @param caller The calling wizard plugin instance.
     * @param menu   The HbMenu instance that needs to be shown on the view
     */
    void updateMainMenu(FtuWizard *caller, HbMenu * menu);

    /**
     * Emit this signal to indicate that the wizard plugin wants to be deactivated. 
     * Wizard is deactivated and Table of Contents is shown by the application.
     * @param caller The calling wizard plugin instance.
     */
    void wizardDeactivated(FtuWizard *caller);

    /**
     * Emit this signal to update the text/icons of the widget.
     * FTU framework reads settings through wizardSettings()and 
     * displays accordingly. It should be ensured by plugin/wizard
     * that the modified settings are reflected through wizardSettings().
     * @param caller The calling wizard plugin instance.
     */
    void wizardSettingsChanged(FtuWizard *caller);
    
};

#endif // FTUWIZARD_H

