/**
 *
 * This file is part of Caesium.
 *
 * Caesium - Caesium is an image compression software aimed at helping photographers,
 * bloggers, webmasters, businesses or casual users at storing, sending and sharing digital pictures.
 *
 * Copyright (C) 2016 - Matteo Paonessa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>
 *
 */

#ifndef CTREEWIDGETITEM_H
#define CTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QFileInfo>

#include "src/utils.h"
#include "src/cimage.h"

class CTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

public:
    CTreeWidgetItem(QTreeWidget* parent) : QTreeWidgetItem(parent) {}
    CTreeWidgetItem(QTreeWidget* parent, QStringList list) : QTreeWidgetItem(parent, list) {}
    CTreeWidgetItem(QTreeWidget* parent, QString path) : QTreeWidgetItem(parent) {
        image = new CImage(path);
        this->setStatus(NEW);
    }

    CImage* image = NULL;

    citem_status getStatus() const;
    void setStatus(const citem_status &value);

private:
    citem_status status;

    bool operator< (const QTreeWidgetItem &other) const {
        int column = treeWidget()->sortColumn();
        switch (column) {
        case COLUMN_ORIGINAL_SIZE:
            return QFileInfo(text(COLUMN_PATH)).size() < QFileInfo(other.text(COLUMN_PATH)).size();
        case COLUMN_NEW_SIZE:
            //Sort by compressed size
            /*
             * WARNING This methods ignores the less significant bytes of the size
             * and may lead to inaccurate sorting. Won't fix for now as not critical.
             */
            return humanToDouble(text(column)) < humanToDouble(other.text(column));
        case COLUMN_SAVED:
            //Sort by saved space
            return ratioToDouble(text(column)) < ratioToDouble(other.text(column));
        default:
            //Sort by path
            return text(column).toLower() < other.text(column).toLower();
        }
    }

signals:
    void compressionStatusChanged(CTreeWidgetItem*, citem_status);
};

Q_DECLARE_METATYPE(CTreeWidgetItem*)


#endif // CTREEWIDGETITEM_H
