/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    vsceneviewer.cpp
 * @brief	Implementation of class VSceneViewer.
 * @author  aaronwang
 * @date    2011/4/23 17:08:33
 * @version v1.00
 **************************************************************************************************/


#include "vsceneviewer.h"
#include "selectscenedialog.h"


/***************************************************************************************************
 *								Construction and Destruction
 **************************************************************************************************/

VSceneViewer::VSceneViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	QAction *action = ui.mMainToolBar->toggleViewAction();
	action->setIcon(QIcon(":/icons/toolbar.svg"));
	ui.mMenuView->insertAction(ui.mMenuPanel->menuAction(), action);
	
	action = ui.mSceneGraphDockWidget->toggleViewAction();
	action->setIcon(QIcon(":/icons/scene.svg"));
	ui.mMenuPanel->addAction(action);
	action = ui.mPropertiesDockWidget->toggleViewAction();
	action->setIcon(QIcon(":/icons/properties.svg"));
	ui.mMenuPanel->addAction(action);
	action = ui.mLogDockWidget->toggleViewAction();
	action->setIcon(QIcon(":/icons/messages.svg"));
	ui.mMenuPanel->addAction(action);
}

VSceneViewer::~VSceneViewer()
{

}


/***************************************************************************************************
 *										Public slots
 **************************************************************************************************/

void VSceneViewer::onOpenFile()
{
	SelectSceneDialog dialog(this);
	dialog.exec();

	if (dialog.result() == QDialog::Accepted)
	{

	}
}

void VSceneViewer::onCloseFile()
{

}

void VSceneViewer::onOpenRecentFile()
{

}

void VSceneViewer::onExitApp()
{

}

void VSceneViewer::onToggleFog()
{

}

void VSceneViewer::onToggleLighting()
{

}

void VSceneViewer::onToggleSkyDome()
{

} 

void VSceneViewer::onToggleStaticEntity()
{

}

void VSceneViewer::onToggleModel()
{

}

void VSceneViewer::onToggleParticleSystem()
{

}

void VSceneViewer::onToggleLiquidTerrain()
{

}

void VSceneViewer::onToggleCollisionArea()
{

}

void VSceneViewer::onToggleMiniMap()
{

}

void VSceneViewer::onToggleFullScreen()
{

}

void VSceneViewer::onToggleGrid()
{

}

void VSceneViewer::onSolidRenderMode()
{

}

void VSceneViewer::onWireframeRenderMode()
{

}

void VSceneViewer::onPointsRenderMode()
{

}

void VSceneViewer::onFreeCtrlMode()
{

}

void VSceneViewer::onFollowCtrlMode()
{

}

void VSceneViewer::onResetCamera()
{

}

void VSceneViewer::onManual()
{

}

void VSceneViewer::onAbout()
{

}
