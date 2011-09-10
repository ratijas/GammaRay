/*
  main.cpp

  This file is part of Endoscope, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

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

#include "config-endoscope.h"
#include "config-endoscope-version.h"
#include "probefinder.h"
#include "injector/injectorfactory.h"

#include <QCoreApplication>
#include <QDebug>
#include <QStringList>

#include <iostream>
using namespace std;

using namespace Endoscope;

int main(int argc, char **argv)
{
  QCoreApplication app(argc, argv);
  QStringList args = app.arguments();
  args.takeFirst(); // that's us

  QString injectorType;
  int pid = -1;
  while (!args.isEmpty() && args.first().startsWith('-')) {
    const QString arg = args.takeFirst();
    if ((arg == QLatin1String("-i") || arg == QLatin1String("--injector")) && !args.isEmpty()) {
      injectorType = args.takeFirst();
      continue;
    }
    if ((arg == QLatin1String("-p") || arg == QLatin1String("--pid")) && !args.isEmpty()) {
      pid = args.takeFirst().toInt();
      continue;
    }
    if (arg == QLatin1String("-h") || arg == QLatin1String("--help")) {
      QTextStream out(stdout);
      out << "Usage: " << argv[0]
          << " [--injector <injector>] --pid <pid> | <application> <args>" << endl;
      out << "" << endl;
      out << "Inspect runtime internals of a Qt-application, such as:" << endl;
      out << "  QObject tree, properties, signal/slots, widgets, models," << endl;
      out << "  graphics views, javascript debugger, resources," << endl;
      out << "  state machines, meta types, fonts, codecs, text documents" << endl;
      out << "" << endl;
      out << "Options:" << endl;
      out << " -i, --injector <injector>\tset injection type, possible values:" << endl;
      out << "                          \t" << InjectorFactory::availableInjectors().join( ", " ) << endl;
      out << " -p, --pid <pid>          \tattach to running Qt application" << endl;
      out << " -h, --help               \tprint program help and exit" << endl;
      out << " -v, --version            \tprint program version and exit" << endl;
      return 0;
    }
    if (arg == QLatin1String("-v") || arg == QLatin1String("--version")) {
      QTextStream out(stdout);
      out << PROGRAM_NAME << " version " << ENDOSCOPE_VERSION_STRING << endl;
      out << "Copyright (C) 2010-2011 Klaralvdalens Datakonsult AB, "
          << "a KDAB Group company, info@kdab.com" << endl;
      return 0;
    }
  }

  if (args.isEmpty() && pid <= 0) {
    qWarning() << "Nothing to probe. Usage:"
               << argv[0]
               << "[--injector <injector>] --pid <pid> | <application> <args>";
    return 1;
  }

  const QString probeDll = ProbeFinder::findProbe(QLatin1String("endoscope_probe"));
  AbstractInjector::Ptr injector;
  if (injectorType.isEmpty()) {
    if (pid > 0) {
      injector = InjectorFactory::defaultInjectorForAttach();
    } else {
      injector = InjectorFactory::defaultInjectorForLaunch();
    }
  } else {
    injector = InjectorFactory::createInjector(injectorType);
  }

  if (injector) {
    if (pid > 0) {
      if (!injector->attach(pid, probeDll, QLatin1String("endoscope_probe_inject"))) {
        cout << "Unable to attach injector " << injector->name().toLatin1().data() << endl;
        return 0;
      } 
    } else {
      if (!injector->launch(args, probeDll, QLatin1String("endoscope_probe_inject"))) {
        cout << "Failed to launch injector " << injector->name().toLatin1().data() << endl;
        return 0;
      }
    }
    return 1;
  }

  if (injectorType.isEmpty()) {
    if (pid > 0) {
#if defined(Q_OS_WIN)
      cout << "Sorry, but at this time there is no attach injector on the Windows platform" << endl;
      cout << "Only the launch injector windll is available on Windows" << endl;
#else
      cout << "Uh-oh, there is no default attach injector" << endl;
#endif
    } else {
      cout << "Uh-oh, there is no default launch injector" << endl;
    }
  } else {
    cout << "Injector " << injectorType.toLatin1().data() << " not found." << endl;
  }
  return 0;
}
