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
			transform.translate(x + width / 2.0, y + height / 2.0);
			transform.scale(mag, mag);
			transform.rotate(angle);
			transform.translate(-(x + width / 2.0), -(y + height / 2.0));
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
		std::string sname = m_Data->structName();
		GDS::Structure* reference = GDS::Library::getInstance()->get(sname);
		if (reference == nullptr)
			return;
		int x, y, width, height;
		if (!GDS::boundingRect(reference, x, y, width, height))
			return;

		int offset_x, offset_y;
		m_Data->xy(offset_x, offset_y);
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
		transform.translate(offset_x, offset_y);
		painter->setTransform(transform);

		if (m_ViewLevel <= 0)
		{
			QRect rect(x, y, width, height);
			painter->drawRect(rect);
			painter->drawText(rect, Qt::AlignCenter, QString(sname.c_str()));
		}
		else
		{
			for (size_t i = 0; i < reference->size(); i++)
			{
				GDS::Element* tmp = reference->get(i);
				if (tmp == nullptr)
					continue;
				switch (tmp->tag())
				{
				case GDS::BOUNDARY:
					if (GDS::Boundary* node = dynamic_cast<GDS::Boundary*>(tmp))
					{
						BoundaryItem item(node);
						item.paint(painter, option, widget);
					}
					break;
				case GDS::PATH:
					if (GDS::Path* node = dynamic_cast<GDS::Path*>(tmp))
					{
						PathItem item(node);
						item.paint(painter, option, widget);
					}
					break;
				case GDS::SREF:
					if (GDS::SRef* node = dynamic_cast<GDS::SRef*>(tmp))
					{
						SRefItem item(node);
						item.setViewLevel(m_ViewLevel - 1);
						item.paint(painter, option, widget);
					}
					break;
				default:
					break;
				}
			}
		}

		
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
