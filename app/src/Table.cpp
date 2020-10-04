#include "Table.h"
#include "ui_mainwindow.h"

Table::Table(QString path) : curentDir(path) {
    if (!path.isEmpty()) {
        QDir dir(curentDir);
        // songs = dir.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.ogg",QDir::Files);    
        songs = dir.entryList(QStringList() << "*.mp3" <<"*.flac" << "*.ogg",QDir::Files); 
        fill_table();
    }
}

Table::~Table() {
    if (!pointers.isEmpty()) {
        int size = pointers.size();
        for(int i = 0; i < size; i++)
            delete pointers[i];
        pointers.clear();
    }
}

QString Table::get_tag(std::string item, QString name) {
    TagLib::FileRef ref((curentDir + name).toUtf8().data());
    TagLib::Tag *tag = ref.tag();
    QString res;
    if (!ref.isNull() && tag) {
        if (item == "Artist")
            res =  QString(tag->artist().toCString());
        if (item == "Album")
            res =  QString(tag->album().toCString());
        if (item == "Song")
            res =  QString(tag->title().toCString());
        if (item == "Genre")
            res =  QString(tag->genre().toCString());
        if (item == "Path")
            res =  QString(curentDir + filename);
        if (item == "Year")
            res =  QString::number(tag->year());
        if(item == "Number")
            res =  QString::number(tag->track());
    }
    return res;
}

void Table::fill_table() {
        if (!pointers.isEmpty()) {
            int size = pointers.size();
            for(int i = 0; i < size; i++)
                delete pointers[i];
            pointers.clear();
        }
        for (QString &filename: songs) 
            pointers.push_back(new QTableWidgetItem(filename));
}

void Table::item_find(std::string item, QString search) {
    QVector <QString> names;
    songs.clear();
    QDir dir(curentDir);
    // songs = dir.entryList(QStringList() << "*.mp3" << "*.wav" << "*.flac" << "*.ogg",QDir::Files);  
    songs = dir.entryList(QStringList() << "*.mp3" << "*.flac" << "*.ogg",QDir::Files);  

    if(!search.isEmpty()) {
        foreach(QString filename, songs) {
            if (get_tag(item, filename) == search) 
                names.push_back(filename);
        }
        songs.clear();
        foreach(QString filename, names)
            songs<<filename;
    }
    names.clear();
    fill_table();
}

void Table::item_sort(std::string item) {
    if(!songs.isEmpty()) {
        if (item == "Ascending")
            std::sort(songs.rbegin(), songs.rend(), [](auto a, auto b) {
                return a > b;
            });
        else if (item == "Descending")
            std::sort(songs.rbegin(), songs.rend(), [](auto a, auto b) {
                return a < b;
            });
        else {
            std::sort(songs.rbegin(), songs.rend(), [&item, this](auto a, auto b) {
                    return this->get_tag(item, a) > this->get_tag(item, b);
            });
        }
    }
    fill_table();
}
