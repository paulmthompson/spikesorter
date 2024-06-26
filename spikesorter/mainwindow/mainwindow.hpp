#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:


private:
    Ui::MainWindow *ui;
    void createActions();


private slots:
    void setChannelDisplayNum(int n_channels_to_display);
    void setCenterSample(int center_sample);
    void setHorizontalZoom(int n_samples_to_display);
    void setCenterChannel(int center_channel);
    void setGain(double gain);
};

#endif // MAINWINDOW_H
