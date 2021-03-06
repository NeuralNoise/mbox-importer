/*
   Copyright (C) 2013-2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <kaboutdata.h>
#include <QApplication>
#include "mboxmainwindow.h"

#include <KLocalizedString>
#include <KCrash>
#include <QFileDialog>
#include <QIcon>
#include <qcommandlineparser.h>
#include <qcommandlineoption.h>
#include "mboximporter-version.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setDesktopFileName(QStringLiteral("org.kde.mboximporter"));
    KCrash::initialize();
    KLocalizedString::setApplicationDomain("mboximporter");

    KAboutData aboutData(QStringLiteral("mboximporter"),
                         i18n("MBox importer tool"),
                         QStringLiteral(MBOXIMPORTER_VERSION),
                         i18n("MBox Import Tool"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2013-2017 MBoxImporter authors"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kmail")));
    aboutData.setOrganizationDomain(QByteArray("kde.org"));
    aboutData.setProductName(QByteArray("mboximporter"));

    KAboutData::setApplicationData(aboutData);

    QString fileName;
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);

    parser.addPositionalArgument(QStringLiteral("url"), i18n("URL of mbox to be imported"), QStringLiteral("[url]"));

    parser.process(app);
    aboutData.processCommandLine(&parser);

    const QStringList &args = parser.positionalArguments();
    if (!args.isEmpty()) {
        fileName = args.at(0);
    } else {
        fileName = QFileDialog::getOpenFileName();
    }
    if (fileName.isEmpty()) {
        return 0;
    }
    MBoxMainWindow *w = new MBoxMainWindow(fileName);
    w->show();
    const int ret = app.exec();
    delete w;
    return ret;
}
