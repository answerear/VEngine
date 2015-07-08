/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    scenegraphwidget.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/4/17 21:53:17
 * @version v1.00
 **************************************************************************************************/


#ifndef __SCENEGRAPHWIDGET_H__
#define __SCENEGRAPHWIDGET_H__


#include <QtGui/QtGui>


class SceneGraphWidget : public QTreeWidget
{
	Q_OBJECT

public:
	SceneGraphWidget(QWidget *parent = 0);
	~SceneGraphWidget();
};


#endif	/*__SCENEGRAPHWIDGET_H__*/
