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
		GDS::Boundary* data)
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
		painter->drawPolygon(polygon);
	}

	void paintPath(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Path* data)
	{
		assert(data != nullptr);
		if (data == nullptr)
			return;

		initPen(painter, option, data->layer(), data->dataType());
		initBrush(painter, data->layer(), data->dataType());

		PathItem item(data);
		painter->drawPath(item.shape());
	}

	void paintStructure(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Structure* data,
		int level,
		int offset_x, int offset_y,
		bool reflect, double mag, double angle)
	{
		assert(data != nullptr);
		if (data == nullptr)
			return;

		int x, y, width, height;
		if (!GDS::boundingRect(data, x, y, width, height))
			return;

		QTransform transform_back = painter->transform();
		QTransform transform;
		if (reflect)
			transform.scale(1, -1);
		transform.scale(mag, mag);
		transform.rotate(angle);
		transform.translate(offset_x, offset_y);
		painter->setTransform(transform, true);

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
						paintBoundary(painter, option, node);
					}
					break;
				case GDS::PATH:
					if (GDS::Path* node = dynamic_cast<GDS::Path*>(tmp))
					{
						paintPath(painter, option, node);
					}
					break;
				case GDS::SREF:
					if (GDS::SRef* node = dynamic_cast<GDS::SRef*>(tmp))
					{
						paintSRef(painter, option, node, level - 1);
					}
					break;
				case GDS::AREF:
					if (GDS::ARef* node = dynamic_cast<GDS::ARef*>(tmp))
					{
						paintARef(painter, option, node, level - 1);
					}
					break;
				default:
					break;
				}
			}
		}
		painter->setTransform(transform_back);
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
		double mag = data->mag();
		double angle = data->angle();
		int s_x, s_y;
		data->xy(s_x, s_y);

		
		paintStructure(painter, option, reference, 
			level, s_x, s_y, reflect, mag, angle);
	}

	void paintARef(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::ARef* data,
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

		std::vector<int> x, y;
		data->xy(x, y);
		assert(x.size() >= 3 && y.size() >= 3);
		if (x.size() < 3 || y.size() < 3)
			return;
		int col = data->col();
		int row = data->row();
		assert(col > 0 && row > 0);
		if (col <= 0 || row <= 0)
			return;
		double angle = data->angle();
		double mag = data->mag();
		bool reflect = data->stransFlag(GDS::REFLECTION);

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
			int aref_width = sqrt(pow(x[1] - x[0], 2) + pow(y[1] - y[0], 2));
			int aref_height = sqrt(pow(x[2] - x[0], 2) + pow(y[2] - y[0], 2));
			QRect rect = QTransform().rotate(angle).mapRect(QRect(0, 0, r_width, r_height));
			painter->drawRect(rect);
			painter->drawText(rect, Qt::AlignCenter, QString(sname.c_str()));
		}
		else
		{
			int row_spacing_x = (x[2] - x[0]) / row;
			int row_spacing_y = (y[2] - y[0]) / row;
			int col_spacing_x = (x[1] - x[0]) / col;
			int col_spacing_y = (y[1] - y[0]) / col;

			int offset_x = x[0];
			int offset_y = y[0];
			for (int i = 0; i < row; i++)
			{
				offset_x = x[0] + row_spacing_x * i;
				offset_y = y[0] + row_spacing_y * i;
				for (int j = 0; j < col; j++)
				{
					paintStructure(painter, option, reference,
						level, offset_x, offset_y, reflect, mag, angle);
					offset_x += col_spacing_x;
					offset_y += col_spacing_y;
				}
			}
		}
	}
}