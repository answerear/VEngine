/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    vsceneviewer.h
 * @brief   Declaration of class VSceneViewer.
 * @author  aaronwang
 * @date    2011/4/23 16:42:23
 * @version v1.00
 **************************************************************************************************/


#ifndef __VSCENEVIEWER_H__
#define __VSCENEVIEWER_H__


#include <QtGui/QMainWindow>
#include "ui_vsceneviewer.h"


/**
 * @class VSceneViewer
 * @brief class of MainWindow.
 */
class VSceneViewer : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor for VSceneViewer.
	 * @param [in] parent Defaults to 0.
	 * @param [in] flags Defaults to 0.
	 */
	VSceneViewer(QWidget *parent = 0, Qt::WFlags flags = 0);

	/**
	 * @brief Destructor for VSceneViewer.
	 */
	~VSceneViewer();

public slots:
	/**
	 * @brief Response to the command of openning file.
	 * @return void  
	 */
	void onOpenFile();

	/**
	 * @brief Response to the command of closing file. 
	 * @return void  
	 */
	void onCloseFile();

	/**
	 * @brief Response to the command of openning recent file. 
	 * @return void  
	 */
	void onOpenRecentFile();

	/**
	 * @brief Response to the command of exit the application. 
	 * @return void  
	 */
	void onExitApp();

	/**
	 * @brief Response to the command of switching fog effect. 
	 * @return void  
	 */
	void onToggleFog();

	/**
	 * @brief Response to the command of switching lighting effect.
	 * @return void  
	 */
	void onToggleLighting();

	/**
	 * @brief Response to the command of showing of hiding the sky dome.
	 * @return void  
	 */
	void onToggleSkyDome();

	/**
	 * @brief Response to the command of showing of hiding the static entities.
	 * @return void  
	 */
	void onToggleStaticEntity();

	/**
	 * @brief Response to the command of showing of hiding the models.
	 * @return void  
	 */
	void onToggleModel();

	/**
	 * @brief Response to the command of switching the particle system effect.
	 * @return void  
	 */
	void onToggleParticleSystem();

	/**
	 * @brief Response to the command of showing of hiding the liquid terrain.
	 * @return void  
	 */
	void onToggleLiquidTerrain();

	/**
	 * @brief Response to the command of showing of hiding the collision area.
	 * @return void  
	 */
	void onToggleCollisionArea();

	/**
	 * @brief Response to the command of showing of hiding the mini map.
	 * @return void  
	 */
	void onToggleMiniMap();

	/**
	 * @brief Response to the command of switching full screen status.
	 * @return void  
	 */
	void onToggleFullScreen();

	/**
	 * @brief Response to the command of showing of hiding the grid.
	 * @return void  
	 */
	void onToggleGrid();

	/**
	 * @brief Response to the command of setting the render mode to solid.
	 * @return void  
	 */
	void onSolidRenderMode();

	/**
	 * @brief Response to the command of setting the render mode to wireframe.
	 * @return void  
	 */
	void onWireframeRenderMode();

	/**
	 * @brief Response to the command of setting the render mode to points.
	 * @return void  
	 */
	void onPointsRenderMode();

	/**
	 * @brief Response to the command of setting the camera mode to free control.
	 * @return void  
	 */
	void onFreeCtrlMode();

	/**
	 * @brief Response to the command of setting the camera mode to follow actor control.
	 * @return void  
	 */
	void onFollowCtrlMode();

	/**
	 * @brief Response to the command of reseting the camera position and orientation.
	 * @return void  
	 */
	void onResetCamera();

	/**
	 * @brief Response to the command of openning user manual.
	 * @return void  
	 */
	void onManual();

	/**
	 * @brief Response to the command of openning about dialog.
	 * @return void  
	 */
	void onAbout();

private:
	Ui::VSceneViewer ui;
};


#endif	/*__VSCENEVIEWER_H__*/
