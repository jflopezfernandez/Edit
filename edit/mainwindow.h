#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>

// Unused
#include <QPlainTextDocumentLayout>
#include <QTextDocument>

namespace UI {
    class MainWindow;
} // End namespace UI

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString currentFile;

    QPlainTextEdit *textEdit;

    // TODO: Document function
    void createActions();

    // TODO: Document function
    void createStatusBar();

    // TODO: Document function
    void readSettings();

    // TODO: Document function
    void writeSettings();

    /*! \fn bool MainWindow::maybeSave()
     *
     *  \brief The maybeSave() function is called to save pending changes.
     *
     */

    bool maybeSave();

    // TODO: Document function
    bool saveFile(const QString& filename);

    // TODO: Document function
    void setCurrentFile(const QString& filename);

    // TODO: Document function
    QString strippedName(const QString& fullFilename);

public:
    // TODO: Document function
    MainWindow();

    // TODO: Document function
    ~MainWindow();

    void loadFile(const QString& filename);

protected:
    // TODO: Document function
    void closeEvent(QCloseEvent *event) override;

private slots:
    // TODO: Document function
    void newFile();

    // TODO: Document function
    void openFile();

    // TODO: Document function
    bool save();

    // TODO: Document function
    bool saveAs();

    // TODO: Document function
    void aboutDialog();

    /*! \fn void documentWasModified()
     *
     *  \brief The documentWasModified() slot is invoked each time the text
     *  in the QPlainTextEdit changes because of user edits. We call
     *  QWidget::setWindowModified() to make the title bar show that the file
     *  was modified. How this is done varies on each platform.
     *
     */

    void documentWasModified();
};

#endif // MAINWINDOW_H
