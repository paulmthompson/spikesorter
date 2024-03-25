#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions(); // Creates callback functions

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions() {
    connect(ui->channels_to_view,SIGNAL(valueChanged(int)),this,SLOT(setChannelDisplayNum(int)));
    connect(ui->sample_scroll_bar,SIGNAL(valueChanged(int)),this,SLOT(setCenterSample(int)));
}

void MainWindow::setChannelDisplayNum(int n_channels_to_display) {

    // qDebug() << "Reset view number to " << n_channels_to_display;

    ui->analog_render_area->setVerticalZoom(n_channels_to_display);
}

void MainWindow::setCenterSample(int center_sample) {

    // qDebug() << "Scrolling to " << center_sample;

    ui->analog_render_area->setCenterSample(center_sample);


}
