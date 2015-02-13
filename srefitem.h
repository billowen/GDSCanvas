/*
 * This file is a part of GDSCanvas
 * srefitem.h -- A header file which declare the sref item for GDSCanvas.
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

#ifndef CANVASSREFITEM_H
#define CANVASSREFITEM_H

#include <QGraphicsItem>
#include "GDS/sref.h"

namespace CANVAS
{
	class SRefItem : public QGraphicsItem
	{
	public:
		SRefItem(GDS::SRef* data, QGraphicsItem* parent = 0);
		virtual ~SRefItem();

		virtual QRectF boundingRect() const;
		virtual QPainterPath shape() const;
		virtual void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = 0);
	private:
		GDS::SRef* m_Data;
	};

}



#endif // !CANVASSREFITEM_H

