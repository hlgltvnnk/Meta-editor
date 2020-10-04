#pragma once

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <QVector>
#include <QStringList>
#include <QDir>
#include <QTableWidget>
#include <QApplication>
#include <QAbstractTableModel>
#include <QFileDialog>
#include <QtWidgets>
// #include "ui_mainwindow.h"

#include <taglib_config.h>
#include <tfile.h>
#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>
#include <id3v2tag.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <mpegfile.h>
#include <attachedpictureframe.h>
#include <unsynchronizedlyricsframe.h>
#include <synchronizedlyricsframe.h>

namespace Ui {
class MainWindow;
}

class Table : public QObject {

public:
    Table(QString path);
    ~Table();
    QString curentDir;
    QVector<QTableWidgetItem *> pointers;
    QStringList songs;
    QString filename;

    void fill_table();
    QString get_tag(std::string item, QString name);
public slots:
    void fill_dir();
    void reset_song(int row , int col);
    void item_find(std::string item, QString search);
    void item_sort(std::string item);
};