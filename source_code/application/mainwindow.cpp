

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
    SetCurrentFile("文本阅读器");
    statusBar()->showMessage(tr("就绪"), 2000);
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
            tr(" <b>文本阅读器</b> 是面向对象程序设计的期末大程,由李牧、蔡旭科、李永杰三人小组完成。\n"
               "运行环境：Win 10, Qt 5.6。"));
}

void MainWindow::CreateActions()
{
    //file menu
    QMenu *fileMenu = menuBar()->addMenu(tr("文件(&F)")); //add File in Menu bar;
    QToolBar *fileToolBar = addToolBar(tr("文件"));    //add File in tool bar

    //open Icon
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/folder.png"));
    openAct = new QAction(openIcon, tr("打开(&O)..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("打开一个文本文件。"));
    connect(openAct, &QAction::triggered, this, &MainWindow::OpenFile);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
    //close Icon
    const QIcon closeIcon = QIcon::fromTheme("document-close", QIcon(":/images/close.png"));
    closeAct = new QAction(closeIcon, tr("关闭(&C)..."), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("关闭当前文件。"));
    connect(closeAct, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(closeAct);
    fileToolBar->addAction(closeAct);
    fileMenu->addSeparator();
    //exit Icon
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    exitAct = fileMenu->addAction(exitIcon, tr("退出(&E)"), this, &QWidget::close);
    exitAct->setStatusTip(tr("退出程序。"));
    exitAct->setShortcuts(QKeySequence::Quit);

    //format menu
    QMenu *styleMenu = menuBar()->addMenu(tr("格式(&S)"));
    QToolBar *styleToolBar = addToolBar(tr("格式"));
    //bold Icon
    const QIcon boldIcon = QIcon::fromTheme("bold", QIcon(":/images/bold.png"));
    boldAct = new QAction(boldIcon, tr("加粗(&B)"), this);
    boldAct->setShortcuts(QKeySequence::Bold);
    boldAct->setStatusTip(tr("文本加粗显示。"));
    connect(boldAct, &QAction::triggered, this, &MainWindow::BoldTypeface);
    styleMenu->addAction(boldAct);
    styleToolBar->addAction(boldAct);
    //italic Icon
    const QIcon italicIcon = QIcon::fromTheme("italic", QIcon(":/images/italic.png"));
    italicAct = new QAction(italicIcon, tr("斜体(&I)"), this);
    italicAct->setShortcuts(QKeySequence::Italic);
    italicAct->setStatusTip(tr("文本斜体显示。"));
    connect(italicAct, &QAction::triggered, this, &MainWindow::ItaliseTypeface);
    styleMenu->addAction(italicAct);
    styleToolBar->addAction(italicAct);

    //underline Icon
    const QIcon underlineIcon = QIcon::fromTheme("underline", QIcon(":/images/underline.png"));
    underlineAct = new QAction(underlineIcon, tr("下划线(&U)"), this);
    underlineAct->setShortcuts(QKeySequence::Underline);
    underlineAct->setStatusTip(tr("文本下划线显示。"));
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
    fontcolorAct = styleMenu->addAction(FontColorIcon, tr("字体颜色(&F)..."), this, &MainWindow::SetFontcolor);
    fontcolorAct->setStatusTip(tr("改变字体颜色。"));
    styleToolBar->addAction(fontcolorAct);

    QPixmap BgColorIcon(16, 16);
    BgColorIcon.fill(Qt::white);
    bgcolorAct = styleMenu->addAction(BgColorIcon, tr("背景颜色(&G)..."), this, &MainWindow::BackGroundColor);
    bgcolorAct->setStatusTip(tr("改变背景颜色。"));
    styleToolBar->addAction(bgcolorAct);

    //convert menu
    QMenu *convertMenu= menuBar()->addMenu(tr("转换(&C)"));
    QToolBar *convertToolBar = addToolBar(tr("转换"));    //add File in tool bar

    //transfer Icon
    const QIcon tohexIcon = QIcon::fromTheme("tohex", QIcon(":/images/Transfer.png"));
    QAction *toHexAct = new QAction(tohexIcon,tr("十六进制(&H)"), this);
    toHexAct->setStatusTip(tr("文本十六进制显示。"));
    connect(toHexAct, &QAction::triggered, this, &MainWindow::HexTransfer);
    convertMenu->addAction(toHexAct);
    convertToolBar->addAction(toHexAct);

    //help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("帮助(&H)"));

    QAction *aboutAct = helpMenu->addAction(tr("关于(&A)"), this, &MainWindow::About);
    aboutAct->setStatusTip(tr("关于 文本阅读器。"));

    QAction *aboutQtAct = helpMenu->addAction(tr("关于&Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::CreateStatusBar()//create the status bar on the window
{
    statusBar()->showMessage(tr("就绪。"));
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
        QMessageBox::warning(this, tr("文本阅读器"),
                             tr("无法读取文件 %1:\n%2.")
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
    statusBar()->showMessage(tr("文件已打开"), 2000);
    file.close();
}

void MainWindow::SetCurrentFile(const QString &fileName)//set the Current file
{
    curFile = fileName;

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "文本阅读器";
    setWindowFilePath(shownName);
}

QString MainWindow::StrippedName(const QString &fullFileName)//stripped the name.
{
    return QFileInfo(fullFileName).fileName();
}
