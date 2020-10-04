#pragma once

#include "Table.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QString path, bool flag, QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    Table *table;
    QString filename;

    QString temporaryDir;
    QString image;

private:
    void setImage();
    void show_meta();
    void main_screen(bool flag);
    QPixmap get_image();
    //void change_page(int page);
    void show_table();
public slots:
    void change_color();
    void reset_song(int row , int col);
    void new_tabel();
    void change_meta();
    void set_path();
    void set_photo();
    void open_change();
    void open_dir();
    void open_main();
    void call_search();
    void call_sort();
    void next_song();
    void prev_song();
};
