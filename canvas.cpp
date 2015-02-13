#include <assert.h>
#include "canvas.h"
#include "boundaryitem.h"
#include "GDS/elements.h"
#include "GDS/boundary.h"
#include "GDSGadgets/boundingrect.h"
#include "pathitem.h"

namespace CANVAS
{
	Canvas::Canvas(GDS::Structure *cell, QObject *parent)
		: QGraphicsScene(parent)
	{
		m_Cell = cell;
		refresh();
	}

	Canvas::~Canvas()
	{

	}

	void Canvas::refresh()
	{
		assert(m_Cell != nullptr);
		if (m_Cell == nullptr)
			return;

		int x, y;
		int c_width, c_height;
        if (GDS::boundingRect(m_Cell, x, y, c_width, c_height))
		{
			int scene_width = c_width * 1.2;
			int scene_height = c_height * 1.2;

			int orgin_x = x - (scene_width - c_width) / 2;
			int orgin_y = y - (scene_height - c_height) / 2;
			setSceneRect(orgin_x, orgin_y, scene_width, scene_height);

			for (size_t i = 0; i < m_Cell->size(); i++)
			{
				GDS::Element* e = m_Cell->get(i);
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

