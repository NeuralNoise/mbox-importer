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

#include "mboxmainwindow.h"
#include "mboximportwidget.h"
#include "mboximportkernel.h"
#include "mboximporterinfogui.h"

#include <mailimporter/filtermbox.h>
#include <mailimporter/importmailswidget.h>
#include <MailImporterAkonadi/FilterImporterAkonadi>

#include <MailCommon/MailKernel>

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

MBoxMainWindow::MBoxMainWindow(const QString &filename, QWidget *parent)
    : QDialog(parent),
      mFileName(filename)
{
    setWindowTitle(i18n("Import mbox file"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    MBoxImporterKernel *kernel = new MBoxImporterKernel(this);
    CommonKernel->registerKernelIf(kernel);   //register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel);   //SettingsIf is used in FolderTreeWidget

    mImportWidget = new MBoxImportWidget(this);
    mainLayout->addWidget(mImportWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &MBoxMainWindow::reject);

    buttonBox->button(QDialogButtonBox::Close)->setDefault(true);
    mainLayout->addWidget(buttonBox);

    connect(mImportWidget, &MBoxImportWidget::importMailsClicked, this, &MBoxMainWindow::slotImportMBox);
    resize(800, 600);
}

MBoxMainWindow::~MBoxMainWindow()
{
}

void MBoxMainWindow::slotImportMBox()
{
    MailImporter::FilterInfo *info = new MailImporter::FilterInfo();
    MailImporter::FilterImporterAkonadi *filterImporterAkonadi = new MailImporter::FilterImporterAkonadi(info);
    MBoxImporterInfoGui *infoGui = new MBoxImporterInfoGui(mImportWidget);
    info->setFilterInfoGui(infoGui);
    filterImporterAkonadi->setRootCollection(mImportWidget->selectedCollection());
    info->clear(); // Clear info from last time

    info->setStatusMessage(i18n("Import in progress"));
    MailImporter::FilterMBox mbox;
    mbox.setFilterImporter(filterImporterAkonadi);
    mbox.setFilterInfo(info);
    info->clear();
    mbox.importMails({mFileName});
    info->setStatusMessage(i18n("Import finished"));
}

