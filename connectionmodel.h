/*
  connectionmodel.h

  This file is part of Endoscope, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krauss@kdab.com>

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

#ifndef ENDOSCOPE_CONNECTIONMODEL_H
#define ENDOSCOPE_CONNECTIONMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include <QVector>
#include <QtCore/qsharedpointer.h>
#include <QPointer>

namespace Endoscope {

class ConnectionModel : public QAbstractTableModel
{
  Q_OBJECT
  public:
    enum Role {
      SenderRole = Qt::UserRole + 1,
      ReceiverRole,
      ConnectionValidRole
    };
    explicit ConnectionModel(QObject* parent = 0);

    void connectionAdded( QObject* sender, const char* signal, QObject* receiver, const char* method, Qt::ConnectionType type );
    void connectionRemoved( QObject* sender, const char* signal, QObject* receiver, const char* method );

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

  private:
    struct Connection
    {
      QObject *rawSender;
      QPointer<QObject> sender;
      QByteArray signal;
      QObject *rawReceiver;
      QPointer<QObject> receiver;
      QByteArray method;
      QByteArray location;
      Qt::ConnectionType type;
      bool valid;
    };
    QVector<Connection> m_connections;
};

}

#endif // ENDOSCOPE_CONNECTIONMODEL_H
