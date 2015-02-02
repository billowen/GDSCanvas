#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <qmainwindow.h>
#include "boundaryitem.h"
#include "canvas.h"
#include "GDS/library.h"
#include "GDS/structures.h"
#include "GDS/boundary.h"
#include "GDS/path.h"


int main(int argc, char** argv)
{
    GDS::Structure* node = new GDS::Structure();
	GDS::Path* e = new GDS::Path();
	std::vector<int> x, y;
	x.push_back(0);
	y.push_back(0);
	x.push_back(100);
	y.push_back(0);
	x.push_back(100);
	y.push_back(100);
    x.push_back(200);
    y.push_back(100);
	e->setXY(x, y);
	e->setLayer(1);
	e->setDataType(0);
    e->setWidth(10);
	node->add(e);

    QApplication app(argc, argv);
	QGraphicsView view;
	CANVAS::Canvas* scene = new CANVAS::Canvas(node);
	view.setScene(scene);

	view.show();
	



    return app.exec();
}
