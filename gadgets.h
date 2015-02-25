/*
* This file is part of GDSCanvas.
*
* gadgets.h -- The header file which declare the funtions for GDS canvas.
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
#ifndef CANVASGADGETS_H
#define CANVASGADGETS_H
#include "GDS/boundary.h"
#include "GDS/path.h"
#include "GDS/sref.h"
#include "GDS/aref.h"
#include "GDS/structures.h"

class QPainter;
class QStyleOptionGraphicsItem;

namespace CANVAS
{

	void initPen(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		int num, int data_type);

	void initBrush(QPainter* painter,
		int num, int data_type);

	void paintBoundary(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Boundary* data,
		int offset_x = 0, int offset_y = 0);

	void paintPath(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Path* data,
		int offset_x = 0, int offset_y = 0);

	void paintSRef(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::SRef* data,
		int offset_x = 0, int offset_y = 0,
		int level = 99);

	void paintARef(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::ARef* data,
		int offset_x = 0, int offset_y = 0,
		int level = 99);

	void paintStructure(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		GDS::Structure* data,
		int offset_x = 0, int offset_y = 0,
		int level = 99);
}

#endif // !CANVASGADGETS_H

