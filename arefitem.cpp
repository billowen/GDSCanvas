/*
 * This file is a part of GDSCanvas.
 * arefitem.cpp -- A source file which defines the aref item of GDSCanvas.
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

#include <math.h>
#include <assert.h>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "GDS/library.h"
#include "GDS/structures.h"
#include "GDS/aref.h"
#include "GDS/boundary.h"
#include "GDS/path.h"
#include "GDSGadgets/boundingrect.h"
#include "arefitem.h"

namespace CANVAS
{
	ARefItem::ARefItem(GDS::ARef* data, QGraphicsItem* parent)
		: QGraphicsItem(parent)
	{
		assert(data != nullptr);
		m_Data = data;
		setFlags(ItemIsSelectable);
	}

	ARefItem::~ARefItem()
	{}

	QRectF ARefItem::boundingRect() const
	{
		int x, y;
		int width, height;
		if (GDS::boundingRect(m_Data, x, y, width, height))
			return QRectF(x, y, width, height);
		else
			return QRectF();
	}

	int calDist(int x1, int y1, int x2, int y2)
	{
		int dis = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
		return sqrt(dis);
	}

	QPainterPath ARefItem::shape() const
	{
		std::vector<int> v_x, v_y;
		m_Data->xy(v_x, v_y);
		assert(v_x.size() == 3 && v_y.size() == 3);
		if (v_x.size() != 3 || v_y.size() != 3)
			return QPainterPath();
		QPolygon polygon;
		polygon << QPoint(v_x[0], v_y[0]);
		polygon << QPoint(v_x[1], v_y[1]);
		int tx = v_x[2] - v_x[0] + v_x[1];
		int ty = v_y[2] - v_y[0] + v_y[1];
		polygon << QPoint(tx, ty);
		polygon << QPoint(v_x[2], v_y[2]);
		QPainterPath path;
		path.addPolygon(polygon);

		return path;
	}

	void ARefItem::paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget)
	{
		std::string sname = m_Data->structName();
		GDS::Structure* reference = GDS::Library::getInstance()->get(sname);
		if (reference == nullptr)
			return;
		int x, y, width, height;
		if (!GDS::boundingRect(reference, x, y, width, height))
			return;

		bool reflect = m_Data->stransFlag(GDS::REFLECTION);
		int mag = m_Data->mag();
		int angle = m_Data->angle();
		QTransform transform;
		transform.translate(x + width / 2.0, y + height / 2.0);
		if (reflect)
			transform.scale(-1, 1);
		transform.scale(mag, mag);
		transform.rotate(angle);
		transform.translate(-(x + width / 2.0), -(y + height / 2.0));
		painter->setTransform(transform);

		if (m_ViewLevel <= 0)
		{

		}
	}

	int ARefItem::viewLevel() const
	{
		return m_ViewLevel;
	}

	void ARefItem::setViewLevel(int level)
	{
		m_ViewLevel = level;
	}
}
