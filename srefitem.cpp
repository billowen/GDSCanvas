/*
 * This file is a part of GDSCanvas
 * srefitem.cpp -- A source file which define the sref item of GDSCavnas.
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
#include "GDSGadgets/boundingrect.h"
#include "GDS/library.h"
#include "GDS/boundary.h"
#include "GDS/path.h"
#include "GDS/structures.h"
#include "pathitem.h"
#include "boundaryitem.h"
#include "gadgets.h"
#include "srefitem.h"

namespace CANVAS
{
	SRefItem::SRefItem(GDS::SRef* data, QGraphicsItem* parent)
		: QGraphicsItem(parent)
	{
		assert(data != nullptr);
		m_Data = data;
		setFlags(ItemIsSelectable);
		m_ViewLevel = 99;
	}

	SRefItem::~SRefItem()
	{}

	QRectF SRefItem::boundingRect() const
	{
		int x, y, width, height;
		if (GDS::boundingRect(m_Data, x, y, width, height))
		{
			return QRectF(x, y, width, height);
		}
		else
		{
			return QRectF();
		}
	}

	QPainterPath SRefItem::shape() const
	{
		std::string sname = m_Data->structName();
		GDS::Structure* reference = GDS::Library::getInstance()->get(sname);
		if (reference == nullptr)
			return QPainterPath();
		int offset_x, offset_y;
		m_Data->xy(offset_x, offset_y);
		int x, y, width, height;
		if (GDS::boundingRect(reference, x, y, width, height))
		{
			QRect rect(x, y, width, height);
			int mag = m_Data->mag();
			int angle = m_Data->angle();
			QTransform transform;
			transform.translate(x, y);
			transform.scale(mag, mag);
			transform.rotate(angle);
			transform.translate(-x, -y);
			transform.translate(offset_x, offset_y);
			QRect rect2 = transform.mapRect(rect);
			QPainterPath path;
			path.addRect(rect2);
			return path;
		}
		else
			return QPainterPath();
	}

	void SRefItem::paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget)
	{
		paintSRef(painter, option, m_Data, m_ViewLevel);
	}

	int SRefItem::viewLevel() const
	{
		return m_ViewLevel;
	}

	void SRefItem::setViewLevel(int level)
	{
		m_ViewLevel = level;
	}
}
