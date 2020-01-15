

#include <QtWidgets>
#include "mainwindow.h"

MainWindow::MainWindow():textEdit(new QTextEdit)// constructor function,initialize textedit as a QplainTextEdit class pointer
{
    setCentralWidget(textEdit);// modify textedit's position to the center of the screen

    CreateActions();
    CreateStatusBar();

    ReadSettings();
    SetCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);

    textEdit->setReadOnly(true);//readonly
    textEdit->setFont(font);

    OpenFile();
}

void MainWindow::OpenFile()//open the file we choose
{
    QString file_Name = QFileDialog::getOpenFileName(this);
    if (!file_Name.isEmpty())
        LoadFile(file_Name);
}

void MainWindow::CloseFile()//close current file
{
    textEdit->setPlainText(NULL);
    SetCurrentFile("TextReader");
    statusBar()->showMessage(tr("Ready"), 2000);
}

void MainWindow::BoldTypeface()//bold the font
{
    font.QFont::setBold(!font.QFont::bold());
    textEdit->setFont(font);
}

void MainWindow::ItaliseTypeface()//italic the font
{
    font.QFont::setItalic(!font.QFont::italic());
    textEdit->setFont(font);
}

void MainWindow::UnderlineTypeface()//underline the font
{
    font.QFont::setUnderline(!font.QFont::underline());
    textEdit->setFont(font);
}

void MainWindow::FontBox(const QString &family)
{
    font.QFont::setFamily(family);//change family of Font
    textEdit->setFont(font);
}

void MainWindow::SizeTypeface(const QString &ptr)
{
    qreal pointSizeF = ptr.toFloat();
    if (pointSizeF > 0) {
        font.QFont::setPointSizeF(pointSizeF);
        textEdit->setFont(font);
    }
}

void MainWindow::HexTransfer()
{
    //When show text,we will add some extra void line to make it clear
    //hence the result may differ initial file littlely.
    //Fomat below:
    //00000000h: xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx\n
    if(!isHex){
        QChar *Char = new QChar[content.count()];
        Char = content.data();
        QString UnicodeFormat;
        for(int i=0;i < content.count();i++){ //change the char into unicode
            UnicodeFormat.append(QString("%1").arg(Char[i].unicode(),2,16,QChar('0')));
        }
        int j=0;
        UnicodeFormat.insert(0,QString("%1h: ").arg(j++,8,16,QChar('0')));//first line number;
        for(int i=LINESIGN;i < UnicodeFormat.count();i++){//add the line number
            if(0 == (i+1)%LINEMAXCOUNT){
                UnicodeFormat.insert(i,QString("\n%1h: ").arg(j++,8,16,QChar('0')));
                i += SINGLEINSERT ;
            }else if(0==((i+1)%LINEMAXCOUNT-LINESIGN)%SETSPACE){
                UnicodeFormat.insert(i,QChar(' '));
            }
        }
        textEdit->setPlainText(UnicodeFormat);//show unicode
        isHex=true;
    }else{
        textEdit->setPlainText(content);//show the plain text
        isHex=false;
    }
}

void MainWindow::SetFontcolor()//control the color of font
{
    QColor fontcol = QColorDialog::getColor(Qt::white, this);
    QPalette fontpal = palette();
    fontpal.setColor (QPalette::Text,fontcol);
    setPalette (fontpal);
    QPixmap FontColorIcon(16, 16);
    FontColorIcon.fill(fontcol);
    fontcolorAct->setIcon(FontColorIcon);
}

void MainWindow::BackGroundColor()//background color control
{
    QColor BackGroundcol = QColorDialog::getColor(Qt::white, this);
    QPalette BackGroundpal = palette();
    BackGroundpal.setColor (QPalette::Base,BackGroundcol);
    setPalette (BackGroundpal);
    QPixmap BackGroundgColorIcon(16, 16);
    BackGroundgColorIcon.fill(BackGroundcol);
    bgcolorAct->setIcon(BackGroundgColorIcon);
}

void MainWindow::About()
{
   QMessageBox::about(this, tr("About Application"),
            tr("Environment:Win 10, Qt 5.6"));
}

void MainWindow::CreateActions()
{
    //file menu
    QMenu *fileMenu = menuBar()->addMenu(tr("File(&F)")); //add File in Menu bar;
    QToolBar *fileToolBar = addToolBar(tr("File"));    //add File in tool bar

    //open Icon
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/folder.png"));
    openAct = new QAction(openIcon, tr("Open(&O)..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a new file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::OpenFile);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
    //close Icon
    const QIcon closeIcon = QIcon::fromTheme("document-close", QIcon(":/images/close.png"));
    closeAct = new QAction(closeIcon, tr("Close(&C)..."), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close current file"));
    connect(closeAct, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(closeAct);
    fileToolBar->addAction(closeAct);
    fileMenu->addSeparator();
    //exit Icon
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    exitAct = fileMenu->addAction(exitIcon, tr("Exit(&E)"), this, &QWidget::close);
    exitAct->setStatusTip(tr("Exit the reader"));
    exitAct->setShortcuts(QKeySequence::Quit);

    //format menu
    QMenu *styleMenu = menuBar()->addMenu(tr("Format(&S)"));
    QToolBar *styleToolBar = addToolBar(tr("Format"));
    //bold Icon
    const QIcon boldIcon = QIcon::fromTheme("bold", QIcon(":/images/bold.png"));
    boldAct = new QAction(boldIcon, tr("Bold(&B)"), this);
    boldAct->setShortcuts(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Bold the Text"));
    connect(boldAct, &QAction::triggered, this, &MainWindow::BoldTypeface);
    styleMenu->addAction(boldAct);
    styleToolBar->addAction(boldAct);
    //italic Icon
    const QIcon italicIcon = QIcon::fromTheme("italic", QIcon(":/images/italic.png"));
    italicAct = new QAction(italicIcon, tr("Italic(&I)"), this);
    italicAct->setShortcuts(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Itaclic the text"));
    connect(italicAct, &QAction::triggered, this, &MainWindow::ItaliseTypeface);
    styleMenu->addAction(italicAct);
    styleToolBar->addAction(italicAct);

    //underline Icon
    const QIcon underlineIcon = QIcon::fromTheme("underline", QIcon(":/images/underline.png"));
    underlineAct = new QAction(underlineIcon, tr("underline(&U)"), this);
    underlineAct->setShortcuts(QKeySequence::Underline);
    underlineAct->setStatusTip(tr("underline the text"));
    connect(underlineAct, &QAction::triggered, this, &MainWindow::UnderlineTypeface);
    styleMenu->addAction(underlineAct);
    styleToolBar->addAction(underlineAct);

    //Font box
    typedef void (QComboBox::*QComboStringSignal)(const QString &);
    fontBox = new QFontComboBox(styleToolBar);
    fontBox->setFontFilters(QFontComboBox::ScalableFonts);
    fontBox->setCurrentFont(font);
    fontBox->setFont(font);
    fontBox->setWritingSystem(QFontDatabase::SimplifiedChinese);
    styleToolBar->addWidget(fontBox);
    connect(fontBox, static_cast<QComboStringSignal>(&QComboBox::activated), this, &MainWindow::FontBox);

    //Text type
    textSize = new QComboBox(styleToolBar);
    textSize->setObjectName("textSize");
    styleToolBar->addWidget(textSize);
    textSize->setEditable(true);
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    foreach (int size, standardSizes)
        textSize->addItem(QString::number(size));
    textSize->setCurrentText(QString::number(font.pointSizeF()));
    connect(textSize, static_cast<QComboStringSignal>(&QComboBox::activated), this, &MainWindow::size);
    styleMenu->addSeparator();

    QPixmap FontColorIcon(16, 16);
    FontColorIcon.fill(Qt::black);
    fontcolorAct = styleMenu->addAction(FontColorIcon, tr("Font Color(&F)..."), this, &MainWindow::SetFontcolor);
    fontcolorAct->setStatusTip(tr("Change the font color"));
    styleToolBar->addAction(fontcolorAct);

    QPixmap BgColorIcon(16, 16);
    BgColorIcon.fill(Qt::white);
    bgcolorAct = styleMenu->addAction(BgColorIcon, tr("Background Color(&G)..."), this, &MainWindow::BackGroundColor);
    bgcolorAct->setStatusTip(tr("Change the background color"));
    styleToolBar->addAction(bgcolorAct);

    //convert menu
    QMenu *convertMenu= menuBar()->addMenu(tr("Convert(&C)"));
    QToolBar *convertToolBar = addToolBar(tr("Convert"));    //add File in tool bar

    //transfer Icon
    const QIcon tohexIcon = QIcon::fromTheme("tohex", QIcon(":/images/Transfer.png"));
    QAction *toHexAct = new QAction(tohexIcon,tr("Hex(&H)"), this);
    toHexAct->setStatusTip(tr("Hex the file"));
    connect(toHexAct, &QAction::triggered, this, &MainWindow::HexTransfer);
    convertMenu->addAction(toHexAct);
    convertToolBar->addAction(toHexAct);

    //help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("File(&H)"));

    QAction *aboutAct = helpMenu->addAction(tr("About(&A)"), this, &MainWindow::About);
    aboutAct->setStatusTip(tr("About"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About&Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::CreateStatusBar()//create the status bar on the window
{
    statusBar()->showMessage(tr("ready"));
}

void MainWindow::ReadSettings() //initialize settings
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::LoadFile(const QString &file_Name)//load file
{
    QFile file(file_Name);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Text Reader"),
                             tr("can not read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(file_Name), file.errorString()));
        return;
    }

    QByteArray data = file.readAll();
    QTextStream in(&data);
    in.setAutoDetectUnicode(false);
    in.setCodec("utf-8");
    content = in.readAll();
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(content);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    isHex=false;
    SetCurrentFile(file_Name);
    statusBar()->showMessage(tr("File already Opened"), 2000);
    file.close();
}

void MainWindow::SetCurrentFile(const QString &fileName)//set the Current file
{
    curFile = fileName;

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "Text Reader";
    setWindowFilePath(shownName);
}

QString MainWindow::StrippedName(const QString &fullFileName)//stripped the name.
{
    return QFileInfo(fullFileName).fileName();
}
