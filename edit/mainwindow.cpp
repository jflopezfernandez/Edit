
#include "mainwindow.h"
#include "ui_mainwindow.h"

// ----------------------------------- Private --------------------------------

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    // Create New File
    const QIcon newFileIcon = QIcon::fromTheme("document-new", QIcon("C:/Users/jflop/Downloads/new-file.png"));

    QAction *actionNewFile = new QAction(newFileIcon, tr("&New"), this);
    actionNewFile->setShortcuts(QKeySequence::New);
    actionNewFile->setStatusTip(tr("Create a new file"));

    connect(actionNewFile, &QAction::triggered, this, &MainWindow::newFile);

    fileMenu->addAction(actionNewFile);

    // Open File
    const QIcon openFileIcon = QIcon::fromTheme("document-open", QIcon("C:/Users/jflop/Downloads/open-file.png"));

    QAction *actionOpenFile = new QAction(openFileIcon, tr("&Open..."), this);
    actionOpenFile->setShortcuts(QKeySequence::Open);
    actionOpenFile->setStatusTip(tr("Open an existing file."));

    connect(actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);

    fileMenu->addAction(actionOpenFile);

    // Save File
    const QIcon saveFileIcon = QIcon::fromTheme("document-save", QIcon("C:/Users/jflop/Downloads/save-file.png"));

    QAction *actionSaveFile = new QAction(saveFileIcon, tr("&Save"), this);
    actionSaveFile->setShortcuts(QKeySequence::Save);
    actionSaveFile->setStatusTip(tr("Save current file"));

    connect(actionSaveFile, &QAction::triggered, this, &MainWindow::save);

    fileMenu->addAction(actionSaveFile);

    // Exit Application
    const QIcon exitApplicationIcon = QIcon::fromTheme("application-exit", QIcon("C:/Users/jflop/Downloads/exit.png"));

    QAction *actionExitApplication = new QAction(exitApplicationIcon, tr("&Exit"), this);
    actionExitApplication->setShortcuts(QKeySequence::Quit);
    actionExitApplication->setToolTip(tr("Exit application."));

    connect(actionExitApplication, &QAction::triggered, this, &MainWindow::close);

    fileMenu->addAction(actionExitApplication);

    // Help Dialog
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *actionAboutDialog = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    actionAboutDialog->setStatusTip(tr("Show the Qt library's About dialog"));
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());

    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();

    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);

        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());

    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave() {
    if (!textEdit->document()->isModified()) {
        return true;
    }

    const QMessageBox::StandardButton optSaveChanges =
            QMessageBox::warning(this,
                                 tr("Application"),
                                 tr("The document has been modified. "
                                    "Do you want to save your changes?"),
                                 QMessageBox::Save |
                                 QMessageBox::Discard |
                                 QMessageBox::Cancel
                                 );

    switch (optSaveChanges) {
        case QMessageBox::Save: {
            return save();
        } break;

        case QMessageBox::Cancel: {
            return false;
        } break;

        default: { /** Empty */ } break;
    }

    return true;
}

bool MainWindow::saveFile(const QString& filename) {
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,
                             tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(filename),
                             file.errorString()));

        return false;
    }

    QTextStream outputStream(&file);
    outputStream << textEdit->toPlainText();

    setCurrentFile(filename);

    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString& filename) {
    currentFile = filename;

    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString showName = currentFile;
    if (currentFile.isEmpty()) {
        showName = "untitled.txt";
    }

    setWindowFilePath(showName);
}

QString MainWindow::strippedName(const QString& fullFilename) {
    return QFileInfo(fullFilename).fileName();
}

// ----------------------------------- Public ---------------------------------

MainWindow::MainWindow()
    : textEdit(new QPlainTextEdit)
{
    setCentralWidget(textEdit);

    createActions();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);

    setCurrentFile(QString());
}

MainWindow::~MainWindow()
{
    delete textEdit;
}

void MainWindow::loadFile(const QString& filename) {
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,
                             tr("Application"),
                             tr("Cannot read file %1: %2.")
                             .arg(QDir::toNativeSeparators(filename),
                             file.errorString()));

        return;
    }

    QTextStream inputStream(&file);

    textEdit->setPlainText(inputStream.readAll());

    setCurrentFile(filename);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

// ----------------------------------- Protected ------------------------------

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

// ----------------------------------- Private Slots --------------------------

void MainWindow::newFile() {
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
}

void MainWindow::openFile() {
    if (maybeSave()) {
        QString filename = QFileDialog::getOpenFileName(this);

        if (!filename.isEmpty()) {
            loadFile(filename);
        }
    }
}

bool MainWindow::save() {
    if (currentFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(currentFile);
    }

    return false;
}

bool MainWindow::saveAs() {
    QFileDialog saveFileDialog(this);

    saveFileDialog.setWindowModality(Qt::WindowModal);
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);

    if (saveFileDialog.exec() != QDialog::Accepted) {
        return false;
    }

    return saveFile(saveFileDialog.selectedFiles().first());
}

void MainWindow::aboutDialog() {
    QMessageBox::about(this,
        tr("About Application"),
        tr("The <b>Application</b> information goes "
           "here when ready.")
    ); // End QMessageBox::about
}

void MainWindow::documentWasModified() {
    setWindowModified(textEdit->document()->isModified());
}
