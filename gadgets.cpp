/*
* This file is part of GDSCanvas.
*
* gadgets.cpp -- The source file which define the funtions for GDS canvas.
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
#include <QPen>
#include <QBrush>
#include "GDS/techfile.h"
#include "GDS/library.h"
#include "GDSGadgets/boundingrect.h"
#include "pathitem.h"
#include "gadgets.h"

namespace CANVAS
{

	void initPen(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		int num, int data_type)
	{
		GDS::Techfile* techfile = GDS::Techfile::getInstance();
		GDS::LayerNode layer;
		bool flag = techfile->getLayer(num, data_type, layer);
		//assert(flag);
		int r, g, b;
		layer.color(r, g, b);
		QColor co(r, g, b);

		QPen pen(co);
		if (option->state & QStyle::State_Selected)
		{
			pen.setStyle(Qt::DotLine);
			pen.setWidth(2);
		}
		painter->setPen(pen);
	}

	void initBrush(QPainter* painter,
		int num, int data_type)
	{
		GDS::Techfile* techfile = GDS::Techfile::getInstance();
		GDS::LayerNode layer;
		bool flag = techfile->getLayer(num, data_type, layer);
		//assert(flag);
		int r, g, b;
		layer.color(r, g, b);
		QColor co(r, g, b);

		QBrush brush;
		std::string stipple_name = layer.stipple();
		GDS::Stipple bits;
		if (stipple_name != "" && techfile->getStipple(stipple_name, bits))
		{
			QImage img(bits.row(), bits.col(), QImage::Format_RGB888);
			for (int i = 0; i < bits.row(); i++)
			{
				for (int j = 0; j < bits.col(); j++)
				{
					if (bits.bit(i, j))
						img.setPixel(i, j, co.rgb());
					else
						img.setPixel(i, j, Qt::white);
				}
			}
			QPixmap pix = QPixmap::fromImage(img);
			brush.setTexture(pix);
		}
		else
		{
			brush.setStyle(Qt::NoBrush);
		}
		painter->setBrush(brush);
	}

	void paintBoundary(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Boundary* data,
		int offset_x, int offset_y)
	{
		assert(data != nullptr);
		if (data == nullptr)
			return;

		initPen(painter, option, data->layer(), data->dataType());
		initBrush(painter, data->layer(), data->dataType());

		std::vector<int> v_x, v_y;
		data->xy(v_x, v_y);
		QPolygon polygon;
		for (size_t i = 0; i < v_x.size() && i < v_y.size(); i++)
		{
			polygon << QPoint(v_x[i], v_y[i]);
		}
		QPolygon p = QTransform().translate(offset_x, offset_y).map(polygon);
		painter->drawPolygon(p);
	}

	void paintPath(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Path* data,
		int offset_x, int offset_y)
	{
		assert(data != nullptr);
		if (data == nullptr)
			return;

		initPen(painter, option, data->layer(), data->dataType());
		initBrush(painter, data->layer(), data->dataType());

		PathItem item(data);
		QPainterPath path = QTransform().translate(offset_x, offset_y).map(item.shape());
		painter->drawPath(path);
	}

	void paintStructure(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Structure* data,
		int offset_x, int offset_y,
		int level)
	{
		assert(data != nullptr);
		if (data == nullptr)
			return;

		if (level <= 0)
		{
			QColor co(Qt::blue);
			QPen pen(co);
			if (option->state & QStyle::State_Selected)
			{
				pen.setStyle(Qt::DotLine);
				pen.setWidth(2);
			}
			painter->setPen(pen);

			int x, y, width, height;
			if (!GDS::boundingRect(data, x, y, width, height))
				return;
			std::string sname = data->name();
			QRect rect(x, y, width, height);
			painter->drawRect(rect);
			painter->drawText(rect, Qt::AlignCenter, QString(sname.c_str()));
		}
		else
		{
			for (size_t i = 0; i < data->size(); i++)
			{
				GDS::Element* tmp = data->get(i);
				if (tmp == nullptr)
					continue;
				switch (tmp->tag())
				{
				case GDS::BOUNDARY:
					if (GDS::Boundary* node = dynamic_cast<GDS::Boundary*>(tmp))
					{
						paintBoundary(painter, option, node, offset_x, offset_y);
					}
					break;
				case GDS::PATH:
					if (GDS::Path* node = dynamic_cast<GDS::Path*>(tmp))
					{
						paintPath(painter, option, node, offset_x, offset_y);
					}
					break;
				case GDS::SREF:
					if (GDS::SRef* node = dynamic_cast<GDS::SRef*>(tmp))
					{
						paintSRef(painter, option, node, offset_x, offset_y, level - 1);
					}
					break;
				case GDS::AREF:
					if (GDS::ARef* node = dynamic_cast<GDS::ARef*>(tmp))
					{
						paintARef(painter, option, node, offset_x, offset_y, level - 1);
					}
					break;
				default:
					break;
				}
			}
		}
	}

	void paintSRef(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::SRef* data,
		int offset_x, int offset_y,
		int level)
	{
		assert(data != nullptr);
		if (data == nullptr)
			return;

		std::string sname = data->structName();
		GDS::Structure* reference = GDS::Library::getInstance()->get(sname);
		if (reference == nullptr)
			return;
		int r_x, r_y, r_width, r_height;
		if (!GDS::boundingRect(reference, r_x, r_y, r_width, r_height))
			return;

		bool reflect = data->stransFlag(GDS::REFLECTION);
		int mag = data->mag();
		int angle = data->angle();

		QTransform transform_back = painter->transform();

		QTransform transform;
		transform.translate(r_x + r_width / 2.0, r_y + r_height / 2.0);
		if (reflect)
			transform.scale(-1, 1);
		transform.translate(-(r_x + r_width / 2.0), -(r_y + r_height / 2.0));
		transform.translate(r_x, r_y);
		transform.scale(mag, mag);
		transform.rotate(angle);
		transform.translate(-r_x, -r_y);
		transform.translate(offset_x, offset_y);
		painter->setTransform(transform, true);

		int s_x, s_y;
		data->xy(s_x, s_y);
		paintStructure(painter, option, reference, s_x, s_y, level);

		painter->setTransform(transform_back);
	}
}