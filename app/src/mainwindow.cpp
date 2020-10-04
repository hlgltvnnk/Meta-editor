#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Img.h"

#include <tag.h>
#include <fileref.h>
#include <id3v2tag.h>
#include <mpegfile.h>
#include <attachedpictureframe.h>

MainWindow::MainWindow(QString path, bool flag, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(QSize(638, 580));

    connect(ui->Back_2, &QPushButton::clicked, this, &MainWindow::open_main);
    connect(ui->Back, &QPushButton::clicked, this, &MainWindow::open_main);
    connect(ui->Dir, &QPushButton::clicked, this, &MainWindow::open_dir);
    connect(ui->Change, &QPushButton::clicked, this, &MainWindow::open_change);

    connect(ui->Accept, &QPushButton::clicked, this, &MainWindow::change_meta);
    connect(ui->PathEd, &QPushButton::clicked, this, &MainWindow::set_path);
    connect(ui->PhotoEd, &QPushButton::clicked, this, &MainWindow::set_photo);

    connect(ui->SearchBut, &QPushButton::clicked, this, &MainWindow::call_search);
    connect(ui->SortBut, &QPushButton::clicked, this, &MainWindow::call_sort);
    connect(ui->ChDir, &QPushButton::clicked, this, &MainWindow::new_tabel);

    connect(ui->Color_set, &QPushButton::clicked, this, &MainWindow::change_color);
    connect(ui->Right, &QPushButton::clicked, this, &MainWindow::next_song);
    connect(ui->Left, &QPushButton::clicked, this, &MainWindow::prev_song);

    ui->Left->setWindowFlags( Qt::FramelessWindowHint );
    ui->Left->setAttribute( Qt::WA_TranslucentBackground );
    ui->Right->setAttribute( Qt::WA_TranslucentBackground );

    ui->Songtab->setWindowFlags( Qt::FramelessWindowHint );
    ui->Songtab->setAttribute( Qt::WA_TranslucentBackground );

    ui->Song->setAttribute( Qt::WA_TranslucentBackground );
    ui->Artist->setAttribute( Qt::WA_TranslucentBackground );
    ui->Album->setAttribute( Qt::WA_TranslucentBackground );
    ui->Genre->setAttribute( Qt::WA_TranslucentBackground );
    ui->Number->setAttribute( Qt::WA_TranslucentBackground );
    ui->Path->setAttribute( Qt::WA_TranslucentBackground );
    ui->Year->setAttribute( Qt::WA_TranslucentBackground );
    ui->PathEdRes->setAttribute( Qt::WA_TranslucentBackground );
    ui->PhotoEdRes->setAttribute( Qt::WA_TranslucentBackground );
    ui->label_4->setAttribute( Qt::WA_TranslucentBackground);
    ui->label_5->setAttribute( Qt::WA_TranslucentBackground);

    ui->Scroll->setCurrentIndex(2);
    if (flag)
        table = new Table(path);
    else {
        table = new Table(nullptr);
        QMessageBox::critical(this, "", "Cannot open directory");
    }
    show_table(); 
}

MainWindow::~MainWindow()
{
    delete table;
    delete ui;
}

void MainWindow::change_color() {
    if(ui->Color_set->isChecked()) {
       ui->page->setStyleSheet("background-image: url(resources/1.png);");
       ui->page_2->setStyleSheet("background-image: url(resources/3.png);");
       ui->page_3->setStyleSheet("background-image: url(resources/2.png);");
    }
    else {
        ui->page->setStyleSheet("background-image: url(resources/1.11.png);");
        ui->page_2->setStyleSheet("background-image: url(resources/2.2.png);");
        ui->page_3->setStyleSheet("background-image: url(resources/2.3.png);");
    }
}

void MainWindow::next_song() {
    int index = table->songs.indexOf(filename) + 1;
    if (index < table->songs.size()) {
        filename = table->songs[index];
        table->filename = filename;
        show_meta();
    }
}

void MainWindow::prev_song() {
    int index = table->songs.indexOf(filename) - 1;
    if (index >= 0) {
        filename = table->songs[index];
        table->filename = filename;
        show_meta();
    }
}

//////////////////////////////////////

void dotted_text(QString title, QString dir, QLabel *label) {
    int width = label->width() - 2;
    if (dir.isEmpty())
        label->setText(title + "is empty :(");
    else {
        QString text = label->fontMetrics().elidedText(title + dir, Qt::ElideRight, width);
        label->setText(text);
        label->setWordWrap(true);
        }
}

void MainWindow::new_tabel() {   
    delete table;
    QDir dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "~",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
        
    if(!dir.isEmpty()) {
        table = new Table(dir.canonicalPath() + "/");
        show_table();
    }
}

void MainWindow::show_table() {
    int col = -1;
    ui->Songtab->clear();
    //ui->Songtab->setSizeHint(400);
    ui->Songtab->setRowCount(table->songs.size());
    ui->Songtab->setColumnCount(1);
    ui->Songtab->verticalHeader()->setVisible(false);
    ui->Songtab->horizontalHeader()->setVisible(false);
    ui->Songtab->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Songtab->setStyleSheet("QTableView {selection-background-color: grey;}");
    ui->Songtab->setSelectionMode(QAbstractItemView::SingleSelection);
    while(col + 1 < table->pointers.size()) {
        table->pointers[col + 1]->setTextAlignment( Qt::AlignCenter );
        ui->Songtab->setItem(col, 1, table->pointers[col + 1]);
        col++;
    }
    connect(ui->Songtab, SIGNAL( cellDoubleClicked (int, int) ),
        this, SLOT(reset_song(int, int))); 
}
////////////////////////////////

QPixmap MainWindow::get_image() {
    TagLib::MPEG::File file((table->curentDir + filename).toUtf8().data());
    TagLib::ID3v2::Tag *m_tag = file.ID3v2Tag(true);
    TagLib::ID3v2::FrameList frameList = m_tag->frameList("APIC");
    if(frameList.isEmpty())
        return QPixmap();
    TagLib::ID3v2::AttachedPictureFrame *coverImg = 
        static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());
    QImage coverQImg;
    coverQImg.loadFromData((const uchar *) coverImg->picture().data(), coverImg->picture().size());
    return QPixmap::fromImage(coverQImg);
}

void MainWindow::show_meta() {
    dotted_text("Artist: ", table->get_tag("Artist", filename), ui->Artist);
    dotted_text("Album: ", table->get_tag("Album", filename), ui->Album);
    dotted_text("Song: ", table->get_tag("Song", filename), ui->Song);
    dotted_text("Genre: ", table->get_tag("Genre", filename), ui->Genre);
    dotted_text("File path: ", table->get_tag("Path", filename), ui->Path);
    dotted_text("Year: ", table->get_tag("Year", filename), ui->Year);
    dotted_text("Track: ", table->get_tag("Number", filename), ui->Number);
    
    QPixmap pix = get_image();
    if (pix.isNull())
        pix.load("resources/photo.png");
    QPixmap p = pix.scaled(200, 200, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    
    QBitmap map(p.width(), p.height());     //my pixmap is 100x100
    map.fill(Qt::color0);
    QPainter painter( &map );
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(0, 0, 200, 200, 20, 20);
    painter.drawRect(0, 20, 100, 100);
    p.setMask(map);
    ui->Photo->setPixmap(p);
}

bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void MainWindow::change_meta() {
    TagLib::FileRef ref((table->curentDir + filename).toUtf8().data());
    TagLib::Tag *tag = ref.tag();
    ui->Scroll->setCurrentIndex(0);

    if (!ui->ArtistEd->text().isEmpty())
        tag->setArtist(ui->ArtistEd->text().toUtf8().constData());
    if (!ui->AlbumEd->text().isEmpty()) 
       tag->setAlbum(ui->AlbumEd->text().toUtf8().constData());
    if (!ui->SongEd->text().isEmpty())
        tag->setTitle(ui->SongEd->text().toUtf8().constData());
    if (!ui->GenreEd->text().isEmpty())
        tag->setGenre(ui->GenreEd->text().toUtf8().constData());
    if (!ui->YearEd->text().isEmpty()) {
        if (is_number(ui->YearEd->text().toUtf8().constData()))
            tag->setYear(ui->YearEd->text().toInt());
        else QMessageBox::critical(this, "", "Year must contain only digits");
    }
    if(!ui->NumberEd->text().isEmpty()) {
        if (is_number(ui->NumberEd->text().toUtf8().constData()))
            tag->setTrack(ui->NumberEd->text().toInt());
        else QMessageBox::critical(this, "", "Track must contain only digits");
    }
    
    if (!temporaryDir.isEmpty() && temporaryDir!= table->curentDir) {
        if(rename((table->curentDir + filename).toLocal8Bit().data(), (temporaryDir + filename).toLocal8Bit().data()) == 0) {
            delete table;
            table = new Table(temporaryDir);
            show_table();
        }
        else QMessageBox::critical(this, "", "Operation failed. Choose enother file destinstion");;
    }
    ref.save();
    if(!image.isEmpty())
        ImageFile::getImageFile(image.toUtf8().constData())->setArt((table->curentDir + filename).toUtf8().constData());
    //photo

    ui->ArtistEd->setText("");
    ui->AlbumEd->setText("");
    ui->SongEd->setText("");
    ui->GenreEd->setText("");
    ui->YearEd->setText("");
    ui->PathEdRes->setText("");
    ui->PhotoEdRes->setText("");
    show_meta();
}

////////////////////////////////
void MainWindow::set_path() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        temporaryDir = dir + "/";
        dotted_text("", dir, ui->PathEdRes);
    }

}

void MainWindow::set_photo() {
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/jana", tr("Image Files (*.jpeg)"));
    if (!fileName.isEmpty()) {
        dotted_text("",fileName, ui->PhotoEdRes);
        image = fileName;
    }
}
