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

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QBrush>
#include "GDS/techfile.h"
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
}