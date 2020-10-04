#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::open_change() {
    ui->Scroll->setCurrentIndex(1);
}

void MainWindow::open_main() {
    if(!table->curentDir.isEmpty() && !table->filename.isEmpty())
        ui->Scroll->setCurrentIndex(0);
    else QMessageBox::critical(this, "", "Choose song to start");
}

void MainWindow::open_dir() {
    ui->Search->setText("");
    QString path = table->curentDir;
    delete table;
    table = new Table(path);
    table->filename = filename;
    show_table();
    ui->Scroll->setCurrentIndex(2);
}

void MainWindow::call_search() {
    table->item_find(ui->SortType->itemText(ui->SortType->currentIndex()).toUtf8().constData(), ui->Search->text());
    show_table();
}

void MainWindow::call_sort() {
    table->item_sort(ui->SortType->itemText(ui->SortType->currentIndex()).toUtf8().constData());
    show_table();
}

void MainWindow::reset_song(int row , int col) {
    ui->Scroll->setCurrentIndex(0);
    filename = ui->Songtab->item(row, col)->text();
    table->filename = filename;
    show_meta();
}
