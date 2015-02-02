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
#include "pathitem.h"

namespace CANVAS
{
	PathItem::PathItem(GDS::Path* data)
		: QGraphicsItem()
	{
		assert(data != NULL);
		Data = data;
	}
	
	PathItem::~PathItem()
	{}

    QRectF PathItem::boundingRect() const
    {
        int x1, y1, x2, y2;
        if (Data->boundaryRect(x1, y1, x2, y2))
        {
            return QRectF(x1, y1, x2 - x1, y2 - y1);
        } 
        else
        {
            return QRectF();
        }
    }

    QPainterPath PathItem::shape() const
    {
        std::vector<int> x, y;
        Data->xy(x, y);
        assert(x.size() >= 2 && x.size() == y.size());
        if (x.size() < 2 || y.size() < 2)
            return QPainterPath();

        int width = Data->width();
        width = width < 0 ? -width : width;
        QPainterPath path;
        path.moveTo(x[0], y[0]);
        for (size_t i = 1; i < x.size() && i < y.size(); i++)
        {
            path.lineTo(x[i], y[i]);
        }
        QPainterPathStroker stroker;
        stroker.setJoinStyle(Qt::MiterJoin);
        stroker.setWidth(width);

        return stroker.createStroke(path);
    }

    void PathItem::paint(QPainter* painter,
            const QStyleOptionGraphicsItem* option, 
            QWidget* widget)
    {
        GDS::Techfile* techfile = GDS::Techfile::getInstance();
        GDS::LayerNode layer;
        bool flag = techfile->getLayer(Data->layer(), Data->dataType(), layer);
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

        QBrush brush;
        std::string stipple_name = layer.stipple();
        GDS::Stipple bits;
        if (stipple_name != "" && techfile->getStipple(stipple_name, bits))
        {
            QImage img(bits.row(), bits.col(), QImage::Format_RGB888);
            for (int i = 0; i < bits.row(); i++ )
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


        painter->drawPath(shape());
    }
}
