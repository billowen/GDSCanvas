#ifndef GDSCANVAS_H
#define GDSCANVAS_H
#include <QGraphicsScene>
#include "GDS/structures.h"



namespace CANVAS
{
	class Canvas : public QGraphicsScene
	{
	public:
		Canvas(GDS::Structure *cell, QObject *parent = 0);
		virtual ~Canvas();

		void refresh();
	private:
		GDS::Structure* m_Cell;
	};
}



#endif