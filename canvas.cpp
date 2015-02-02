#include <assert.h>
#include "canvas.h"
#include "boundaryitem.h"
#include "GDS/elements.h"
#include "GDS/boundary.h"
#include "pathitem.h"

namespace CANVAS
{
	Canvas::Canvas(GDS::Structure *cell, QObject *parent)
		: QGraphicsScene(parent)
	{
		Cell = cell;
		refresh();
	}

	Canvas::~Canvas()
	{

	}

	void Canvas::refresh()
	{
		assert(Cell != nullptr);
		if (Cell == nullptr)
			return;

		int x1, x2, y1, y2;
		if (Cell->boundaryRect(x1, y1, x2, y2))
		{
			int width = (x2 - x1) * 1.2;
			int height = (y2 - y1) * 1.2;

			int orgin_x = (x1 + x2) / 2 - width / 2;
			int orgin_y = (y1 + y2) / 2 - height / 2;
			setSceneRect(orgin_x, orgin_y, width, height);

			for (size_t i = 0; i < Cell->size(); i++)
			{
				GDS::Element* e = Cell->get(i);
				if (e == nullptr)
					continue;
				if (GDS::Boundary* node = dynamic_cast<GDS::Boundary*>(e))
				{
					BoundaryItem* item = new BoundaryItem(node);
					addItem(item);
				}
                else if (GDS::Path* node = dynamic_cast<GDS::Path*>(e))
                {
                    PathItem* item = new PathItem(node);
                    addItem(item);
                }
			}
		}
		

		
	}
}

