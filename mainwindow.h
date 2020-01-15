#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFont>
#include <QFontComboBox>
#include <QMainWindow>
#include <qtextcodec.h>

#define LINESIGN 11
#define LINEMAXCOUNT 51
#define SETSPACE 5
#define SINGLEINSERT 12

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;//menu bar
class QTextEdit;// text show and edit class
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT//macro motivate the feature of Qt class

public:
    MainWindow();
    void LoadFile(const QString &file_Name);//load the file

private slots:
    void OpenFile();
    void CloseFile();
    void BoldTypeface();
    void ItaliseTypeface();
    void UnderlineTypeface();
    void FontBox(const QString &family);
    void SizeTypeface(const QString &ptr);
    void SetFontcolor();
    void BackGroundColor();
    void HexTransfer();//Transfer from Hex to ASCII
    void About();

private:
    void CreateActions();
    void CreateStatusBar();
    void ReadSettings();
    void SetCurrentFile(const QString &fileName);
    QString StrippedName(const QString &fullFileName);
    QTextEdit *textEdit;
    QString curFile;
    QFont font;
    QString content;
    QAction *openAct;
    QAction *closeAct;
    QAction *exitAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *underlineAct;
    QFontComboBox *fontBox;
    QComboBox *textSize;
    QAction *fontcolorAct;
    QAction *bgcolorAct;
    QAction *toHexAct;
    QAction *aboutAct;
    bool isHex;
};
#endif
