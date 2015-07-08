/**************************************************************************************************
 * Copyright (C) 2011. Lavender Studio.
 * All rights reserved.
 *
 * @file    renderwidget.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2011/4/17 21:34:00
 * @version v1.00
 **************************************************************************************************/


#ifndef __RENDERWIDGET_H__
#define __RENDERWIDGET_H__


#include <QtGui/QtGui>


class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	RenderWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~RenderWidget();
};


#endif	/*__RENDERWIDGET_H__*/
