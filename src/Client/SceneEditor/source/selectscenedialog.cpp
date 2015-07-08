/***************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    selectscenedialog.cpp
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/4/24 10:16:33
 * @version v1.00
 **************************************************************************************************/


#include "selectscenedialog.h"


/***************************************************************************************************
 *								Construction and Destruction
 **************************************************************************************************/

SelectSceneDialog::SelectSceneDialog(QWidget *parent, Qt::WFlags f /* = Qt::Dialog | Qt::WindowTitleHint */)
	: QDialog(parent, f)
{
	setupUi(this);

	populateSceneFiles();
}

SelectSceneDialog::~SelectSceneDialog()
{

}


/***************************************************************************************************
 *										Public slots
 **************************************************************************************************/

void SelectSceneDialog::onBtnOK()
{
	accept();
}

void SelectSceneDialog::onBtnCancel()
{
	reject();
}


/***************************************************************************************************
 *									Protected methods
 **************************************************************************************************/

void SelectSceneDialog::populateSceneFiles()
{
	QString dataPath = "../data/scene/";


}

