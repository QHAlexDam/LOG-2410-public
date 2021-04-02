/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#endif

#include "teamviewer.h"

TeamViewer::TeamViewer()
   : m_teamManager()
	, m_rootTeam(m_teamManager.addTeam("House"))
	, scrollArea(new QScrollArea)
	, container(new QWidget)
	, hBoxLayout(new QHBoxLayout)
{
	container->setLayout(hBoxLayout);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setVisible(false);
	scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);

	createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

bool TeamViewer::loadFile(AbsTeamComponent& team, const QString& fileName, const QString& role)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

	if (role.isEmpty())
	{
		const TeamMember& newMember = m_teamManager.addMemberToTeam(dynamic_cast<Team&>(team), fileName.toStdString(), newImage);
		m_viewWidgets.push_back(TeamComponentView(newMember));
	}
	else
	{
		const TeamMemberRole& newMember = m_teamManager.addMemberToTeamwRole(dynamic_cast<Team&>(team), fileName.toStdString(), newImage, role.toStdString());
		m_viewWidgets.push_back(TeamComponentView(newMember));
	}

	hBoxLayout->addWidget(m_viewWidgets.back().getWidget());
	scrollArea->setWidget(container);

	scrollArea->setVisible(true);
	printAct->setEnabled(true);
	updateActions();

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
    statusBar()->showMessage(message);
    return true;
}

bool TeamViewer::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(m_viewWidgets.back().getImage())) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        //const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
		const QStringList picturesLocations(".\\photos");
		dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void TeamViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(m_rootTeam, dialog.selectedFiles().first(), QString())) {}
}

void TeamViewer::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void TeamViewer::print()
{
#if 0
    Q_ASSERT(m_TeamMember[currentColumn].getImageLabel()->pixmap());
#if QT_CONFIG(printdialog)
//! [6] //! [7]
    QPrintDialog dialog(&printer, this);
//! [7] //! [8]
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = m_TeamMember[currentColumn].getImageLabel()->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(m_TeamMember[currentColumn].getImageLabel()->pixmap()->rect());
        painter.drawPixmap(0, 0, *m_TeamMember[currentColumn].getImageLabel()->pixmap());
    }
#endif
#endif
}
//! [8]

void TeamViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(m_viewWidgets.back().getImage());
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
        if (mimeData->hasImage()) {
            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull())
                return image;
        }
    }
    return QImage();
}
#endif // !QT_NO_CLIPBOARD

void TeamViewer::paste()
{
#ifndef QT_NO_CLIPBOARD
    const QImage newImage = clipboardImage();
    if (newImage.isNull()) {
        statusBar()->showMessage(tr("No image in clipboard"));
    } else {
        //setImage(newImage);
        setWindowFilePath(QString());
        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
        statusBar()->showMessage(message);
    }
#endif // !QT_NO_CLIPBOARD
}

bool TeamViewer::loadFile(const QString & fileName)
{
	m_viewWidgets.clear();
	m_rootTeam.deleteAllComponents();
	return loadFile(m_rootTeam, fileName, QString());
}

void TeamViewer::openTeam()
{
	const QString message = tr("openTeam called");
	statusBar()->showMessage(message);

	m_viewWidgets.clear(); 
	m_rootTeam.deleteAllComponents();

	AbsTeamComponent& team_govrmt(m_rootTeam.addTeamComponent(Team("Gouvernement")));
	loadFile(team_govrmt, ".\\photos\\Trudeau.jpg", QString());
	loadFile(team_govrmt, ".\\photos\\Freeland.jpg", QString());

	AbsTeamComponent& team_opposit(m_rootTeam.addTeamComponent(Team("Opposition")));
	loadFile(team_opposit, ".\\photos\\Scheer.jpg", QString());
	loadFile(team_opposit, ".\\photos\\Raitt.jpg", QString());
}

void TeamViewer::openRole()
{
	const QString message = tr("openRole called");
	statusBar()->showMessage(message);
	m_viewWidgets.clear();
	m_rootTeam.deleteAllComponents();

	AbsTeamComponent& team_govrmt(m_rootTeam.addTeamComponent(Team("Gouvernement")));
	loadFile(team_govrmt, ".\\photos\\Trudeau.jpg", QString("Premier Ministre"));
	loadFile(team_govrmt, ".\\photos\\Freeland.jpg", QString("Ministre"));

	AbsTeamComponent& team_opposit(m_rootTeam.addTeamComponent(Team("Opposition")));
	loadFile(team_opposit, ".\\photos\\Scheer.jpg", QString("Chef de l'opposition"));
	loadFile(team_opposit, ".\\photos\\Raitt.jpg", QString("Ex Membre du Parlement"));
}

void TeamViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Team Viewer</b> application shows how to combine Qt "
               "with the Composite and Decorator design patterns "
               "for displaying groups of images structured in teams. "
               "The QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates QLabel's ability to display images and text."
				"</p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}

void TeamViewer::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &TeamViewer::open);
    openAct->setShortcut(QKeySequence::Open);

	QAction *openTeamAct = fileMenu->addAction(tr("Open &Team"), this, &TeamViewer::openTeam);
	openTeamAct->setShortcut(tr("Ctrl+T"));

	QAction *openRoleAct = fileMenu->addAction(tr("Open &Role"), this, &TeamViewer::openRole);
	openRoleAct->setShortcut(tr("Ctrl+R"));

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &TeamViewer::saveAs);
    saveAsAct->setEnabled(false);

    printAct = fileMenu->addAction(tr("&Print..."), this, &TeamViewer::print);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAct = editMenu->addAction(tr("&Copy"), this, &TeamViewer::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &TeamViewer::paste);
    pasteAct->setShortcut(QKeySequence::Paste);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &TeamViewer::about);
    helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);
}

void TeamViewer::updateActions()
{
    saveAsAct->setEnabled(!m_viewWidgets.back().getImage().isNull());
    copyAct->setEnabled(!m_viewWidgets.back().getImage().isNull());
}
