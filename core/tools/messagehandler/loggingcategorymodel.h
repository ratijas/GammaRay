/*
  loggingcategorymodel.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2016-2021 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  accordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMARAY_LOGGINGCATEGORYMODEL_H
#define GAMMARAY_LOGGINGCATEGORYMODEL_H

#include <QAbstractTableModel>
#include <QLoggingCategory>
#include <QVector>

namespace GammaRay {
void categoryFilter(QLoggingCategory *category);

class LoggingCategoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LoggingCategoryModel(QObject *parent = nullptr);
    ~LoggingCategoryModel() override;

    Q_INVOKABLE QByteArray exportLoggingConfig(bool all, bool forFile);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void addCategorySignal(QLoggingCategory *category);

private:
    void addCategorySlot(QLoggingCategory *category);
    void addCategory(QLoggingCategory *category);

    struct CategoryWithDefaultValues {
        QLoggingCategory *category;
        bool wasDebugEnabled;
        bool wasInfoEnabled;
        bool wasWarningEnabled;
        bool wasCriticalEnabled;
    };
    QVector<CategoryWithDefaultValues> m_categories;
    QLoggingCategory::CategoryFilter m_previousFilter;

    friend void categoryFilter(QLoggingCategory *);
    static LoggingCategoryModel *m_instance;
};
}

#endif // GAMMARAY_LOGGINGCATEGORYMODEL_H
