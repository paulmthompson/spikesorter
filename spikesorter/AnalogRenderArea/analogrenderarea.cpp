// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "analogrenderarea.hpp"

#include <QPainter>
#include <QPainterPath>

#include <QDebug>
#include <QElapsedTimer>

#include <random>

AnalogRenderArea::AnalogRenderArea(QWidget *parent)
        : QWidget(parent)
{
    debug = true;
    track_horizontal = true;

    createVirtualData(32,30000 * 5);

    XAxisProps = AnalogRenderXAxisProps();
    XAxisProps.setXAxisProps(30000.0, 30000 * 5);

    this->analog_line_offsets = std::vector<float>(1);
    calculate_analog_line_offsets();

    antialiased = false;
    pixmap.load(":/images/qt-logo.png");

    this->pen = QPen(Qt::white); // Outline of shapes and lines
    this->brush = QBrush(Qt::NoBrush); // Fill pattern of shapes

    this->analog_path.reserve(XAxisProps.getSamplesToShow());

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    setMouseTracking(true);

}

QSize AnalogRenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize AnalogRenderArea::sizeHint() const
{
    return QSize(400, 200);
}

void AnalogRenderArea::resizeEvent(QResizeEvent* event) {

    calculate_analog_line_offsets();

    update();
}

void AnalogRenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void AnalogRenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

void AnalogRenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void AnalogRenderArea::setHorizontalZoom(int64_t n_samples) {

    XAxisProps.setSamplesToShow(n_samples);

    //Reserve painter path
    this->analog_path.reserve(n_samples);

    update();
}

void AnalogRenderArea::setCenterSample(int64_t sample) {

    XAxisProps.setCenterSample(sample);

    update();
}

void AnalogRenderArea::calculate_analog_line_offsets() {

    float spacing = static_cast<float>(height()) / (this->analog_line_offsets.size() + 1);
    for (int x = 0; x < this->analog_line_offsets.size(); x++) {
        analog_line_offsets[x] = spacing * (x + 1);
    }
}

float AnalogRenderArea::calculate_horizontal_scale() {
    //Calculates the scale in pixels / samples

    return static_cast<float>(this->width()) / static_cast<float>(this->XAxisProps.getSamplesToShow());
}

void AnalogRenderArea::setVerticalZoom(int64_t n_lines_to_show) {

    this->analog_line_offsets = std::vector<float>(n_lines_to_show);

    calculate_analog_line_offsets();

    update();
}

void AnalogRenderArea::mouseMoveEvent(QMouseEvent *event){

    int64_t first_sample = this->XAxisProps.getFirstSample();

    float sample_under_mouse_event = 1 / calculate_horizontal_scale() * static_cast<float>(event->pos().x());

    if (this->debug) {
        qDebug() << event->pos();
        qDebug() << "This corresponds to sample " << sample_under_mouse_event + first_sample;
    }

    //this->last_mouse_event_coords = event->pos();

    update();
}


void AnalogRenderArea::paintEvent(QPaintEvent * /* event */)
{

    QElapsedTimer paintTimer;
    paintTimer.start();

    QPainter painter(this);

    drawBackground(painter);

    drawAnalogLines(painter);

    if (this->track_horizontal) {
        drawMouseVerticalLine(painter);
    }

    qint64 elapsed_ns = paintTimer.nsecsElapsed();
    qint64 elapsed_us = elapsed_ns/1000L;

    qDebug() << "Time to paint : " << elapsed_us << "us";
}

void AnalogRenderArea::drawBackground(QPainter& painter)
{
    // Black background
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::SolidPattern);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void AnalogRenderArea::drawMouseVerticalLine(QPainter& painter) {

    painter.save();

    painter.setPen(Qt::white);
    painter.drawLine(last_mouse_event_coords.x(),0,last_mouse_event_coords.x(),height());

    painter.restore();
}

void AnalogRenderArea::drawAnalogLines(QPainter& painter) {

    painter.setPen(this->pen);
    painter.setBrush(this->brush);
    if (antialiased) {
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

    // Calculate the horizontal scale factor
    qreal scale_x = calculate_horizontal_scale();
    qreal global_y_gain = 1.0;

    for (int x = 0; x < this->analog_line_offsets.size(); x += 1) {

        // Save painter before each line that is drawn
        painter.save();

        float y_offset = this->analog_line_offsets[x];

        qreal local_y_gain = global_y_gain + 0.0;

        painter.translate(0.0,y_offset);
        painter.scale(scale_x,local_y_gain);

        drawAnalogLine(painter,this->virtual_data[x]);

        painter.restore();
    }

}

void AnalogRenderArea::drawAnalogLine(QPainter& painter, std::vector<float>& data) {

    int first_ind = this->XAxisProps.getFirstSample();

    analog_path.moveTo(0, data[first_ind]);

    for (int x = 0; x < this->XAxisProps.getSamplesToShow(); x += 1 ) {
        analog_path.lineTo(x, data[first_ind + x]);
    }

    painter.drawPath(analog_path);

    analog_path.clear();
}

void AnalogRenderArea::createVirtualData(int n_channels, int n_samples) {

    std::random_device random_device;
    std::mt19937 random_engine{random_device()};
    std::uniform_real_distribution distribution100 = std::uniform_real_distribution<float>(-10,10);

    virtual_data = std::vector<std::vector<float>>(n_channels);

    auto gen = [&distribution100, &random_engine](){
        return distribution100(random_engine);
    };
    for (int i = 0; i< virtual_data.size(); i ++) {
        virtual_data[i] = std::vector<float>(n_samples);
        std::generate(begin(virtual_data[i]),end(virtual_data[i]),gen);
    }
}
