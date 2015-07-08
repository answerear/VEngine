/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    selectscenedialog.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/4/24 10:02:16
 * @version v1.00
 **************************************************************************************************/


#ifndef __SELECTSCENEDIALOG_H__
#define __SELECTSCENEDIALOG_H__


#include "ui_selectscenedialog.h"


/**
 * @class SelectSceneDialog
 * @brief Supply a scene file selection dialog.
 */
class SelectSceneDialog : public QDialog, public Ui::SelectSceneDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor for SelectSceneDialog.
	 * @param [in] parent 
	 * @param [in] f Defaults to Qt::WindowTitleHint.
	 */
	SelectSceneDialog(QWidget *parent, Qt::WFlags f = Qt::Dialog | Qt::WindowTitleHint);

	/**
	 * @brief Destructor for SelectSceneDialog.
	 */
	~SelectSceneDialog();

public slots:
	/**
	 * @brief Response to click command of button OK. 
	 * @return void  
	 */
	void onBtnOK();

	/**
	 * @brief Response to click command of button Cancel.
	 * @return void  
	 */
	void onBtnCancel();

protected:
	void populateSceneFiles();

protected:
	QString mSceneFile;		/**< the file name of scene file */
};


#endif	/*__SELECTSCENEDIALOG_H__*/

