/*
  fontbrowser.h

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

#ifndef ENDOSCOPE_FONTBROWSER_H
#define ENDOSCOPE_FONTBROWSER_H

#include <qwidget.h>
#include <QFontDatabase>
#include <toolfactory.h>

class QItemSelection;

namespace Endoscope {

class FontModel;
namespace Ui { class FontBrowser; }

class FontBrowser : public QWidget
{
  Q_OBJECT
  public:
    explicit FontBrowser(ProbeInterface* probe, QWidget* parent = 0);

  private slots:
    void updateFonts(const QItemSelection &selected, const QItemSelection &deselected);

  private:
    QScopedPointer<Ui::FontBrowser> ui;
    FontModel *m_selectedFontModel;
};

class FontBrowserFactory : public QObject, public StandardToolFactory<QObject, FontBrowser>
{
  Q_OBJECT
  Q_INTERFACES(Endoscope::ToolFactory)
  public:
    virtual inline QString name() const { return tr( "Fonts" ); }
};

}

#endif // ENDOSCOPE_FONTBROWSER_H
