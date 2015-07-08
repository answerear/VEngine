#include "vsceneviewer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VSceneViewer w;
	w.show();
	return a.exec();
}
