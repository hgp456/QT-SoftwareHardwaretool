// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "assistant.h"

#include <QApplication>
#include <QByteArray>
#include <QDir>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QStandardPaths>

using namespace Qt::StringLiterals;

//! [0]
Assistant::~Assistant()
{
    if (!m_process.isNull() && m_process->state() == QProcess::Running) {
        QObject::disconnect(m_process.data(), &QProcess::finished, nullptr, nullptr);
        m_process->terminate();
        m_process->waitForFinished(3000);
    }
}
//! [0]

//! [1]
void Assistant::showDocumentation()
{
    if (!startAssistant())
        return;

}
//! [1]

static QString documentationDirectory()
{
    QStringList paths;
    paths.append(QLatin1StringView(SRCDIR));

    for (const auto &dir : std::as_const(paths)) {
        const QString path = dir + "documentation";
        if (QFileInfo::exists(path)){
            qDebug()<<"Doc Path:" << path;
            return path;
        }
    }
    return {};
}

//! [2]
bool Assistant::startAssistant()
{
    m_process.reset(new QProcess);
    QObject::connect(m_process.data(), &QProcess::finished,
                     m_process.data(), [this](int exitCode, QProcess::ExitStatus status) {
        finished(exitCode, status);
    });

    QString app = QLibraryInfo::path(QLibraryInfo::BinariesPath);

    qDebug() << app;

    app += "/assistant";

    const QString collectionDirectory = documentationDirectory();
    if (collectionDirectory.isEmpty()) {
        showError(tr("The documentation directory cannot be found"));
        return false;
    }

    const QStringList args{"-collectionFile",
                           collectionDirectory + "/Tool2.qhc"};

    m_process->start(app, args);

    if (!m_process->waitForStarted(3000)) {
        showError(tr("Unable to launch Qt Assistant (%1): %2")
                  .arg(QDir::toNativeSeparators(app), m_process->errorString()));
        return false;
    }

    return true;
}
//! [2]

void Assistant::showError(const QString &message)
{
    QMessageBox::critical(QApplication::activeWindow(), tr("Simple Text Viewer"), message);
}

void Assistant::finished(int exitCode, QProcess::ExitStatus status)
{
    const QString stdErr = QString::fromLocal8Bit(m_process->readAllStandardError());
    if (status != QProcess::NormalExit)
        showError(tr("Assistant crashed: %1").arg(stdErr));
    else if (exitCode != 0)
        showError(tr("Assistant exited with %1: %2").arg(exitCode).arg(stdErr));
}
