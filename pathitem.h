/*
* This file is part of GDSII.
*
* pathitem.h -- The header file which declare the path item for GDS canvas.
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
#include <QGraphicsItem>
#include "GDS/path.h"

namespace CANVAS
{
	class PathItem : public QGraphicsItem
	{
		GDS::Path* Data;

	public:
		PathItem(GDS::Path* data);
		virtual ~PathItem();

		virtual QRectF boundingRect() const;
        virtual QPainterPath shape() const;
		virtual void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = 0);
	};
}
