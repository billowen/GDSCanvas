/*
* This file is part of GDSII.
*
* boundaryitem.cpp -- The source file which defines the boundary item for GDS canvas.
*
* Copyright (c) 2015 Kangpeng Shao <billowen035@gmail.com>
*
* GDSII is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at you option) any later version.
*
* GDSII is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABLILTY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License
* along with GDSII. If not, see <http://www.gnu.org/licenses/>.
**/

#include <assert.h>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "boundaryitem.h"
#include "GDS/techfile.h"
#include "GDSGadgets/boundingrect.h"
#include "gadgets.h"

namespace CANVAS
{
	BoundaryItem::BoundaryItem(GDS::Boundary* data, QGraphicsItem* parent)
		: QGraphicsItem(parent)
	{
		assert(data != nullptr);
		m_Data = data;
		setFlags(ItemIsSelectable);
	}

    BoundaryItem::~BoundaryItem()
    {}

	QRectF BoundaryItem::boundingRect() const
	{
		int x, y;
		int width, height;
        if (GDS::boundingRect(m_Data, x, y, width, height))
		{
			return QRectF(x, y, width, height);
		}
		else
		{
			return QRectF();
		}
	}

	QPainterPath BoundaryItem::shape() const
	{
		std::vector<int> v_x, v_y;
		m_Data->xy(v_x, v_y);
		QPolygon polygon;
		for (size_t i = 0; i < v_x.size() && i < v_y.size(); i++)
		{
			polygon << QPoint(v_x[i], v_y[i]);
		}

		QPainterPath path;
		path.addPolygon(polygon);

		return path;
	}

	void BoundaryItem::paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget)
	{
		paintBoundary(painter, option, m_Data);
	}
}
