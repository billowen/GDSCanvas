/*
* This file is part of GDSII.
*
* pathitem.cpp -- The source file which defines the path item for GDS canvas.
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
#include "GDS/path.h"
#include "GDS/techfile.h"
#include "GDSGadgets/boundingrect.h"
#include "pathitem.h"
#include "gadgets.h"

namespace CANVAS
{
	PathItem::PathItem(GDS::Path* data, QGraphicsItem* parent)
		: QGraphicsItem(parent)
	{
		assert(data != NULL);
		m_Data = data;
		setFlags(ItemIsSelectable);
	}
	
	PathItem::~PathItem()
	{}

    QRectF PathItem::boundingRect() const
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

    QPainterPath PathItem::shape() const
    {
		std::vector<int> v_x, v_y;
		m_Data->xy(v_x, v_y);

		assert(v_x.size() == v_y.size());
		if (v_x.size() != v_y.size())
			return QPainterPath();
		for (size_t i = 0; i < v_x.size() - 1;)
		{
			if (v_x[i] == v_x[i + 1] && v_y[i] == v_y[i + 1])
			{
				v_x.erase(v_x.begin() + i + 1);
				v_y.erase(v_y.begin() + i + 1);
			}
			else
			{
				i++;
			}
		}
		assert(v_x.size() >= 2 && v_y.size() >= 2);
		if (v_x.size() < 2 || v_y.size() < 2)
			return QPainterPath();
		int path_width = m_Data->width();
		if (path_width < 0)
			path_width = -path_width;

		QPainterPath path;
		path.moveTo(v_x[0], v_y[0]);
		for (size_t i = 1; i < v_x.size() && i < v_y.size(); i++)
		{
			path.lineTo(v_x[i], v_y[i]);
		}
		QPainterPathStroker stroker;
		stroker.setJoinStyle(Qt::MiterJoin);
		stroker.setWidth(path_width);

        return stroker.createStroke(path);
    }

    void PathItem::paint(QPainter* painter,
            const QStyleOptionGraphicsItem* option, 
            QWidget* widget)
    {
		paintPath(painter, option, m_Data);
    }
}
