// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "analogrenderarea.hpp"

#include <QPainter>
#include <QPainterPath>

#include <QDebug>
#include <QElapsedTimer>

AnalogRenderArea::AnalogRenderArea(QWidget *parent)
        : QWidget(parent)
{
    debug = false;
    track_horizontal = true;

    x_axis_width_in_samples = 10000;

    analog_line_offsets = {static_cast<float>(height()) / 2};

    antialiased = false;
    pixmap.load(":/images/qt-logo.png");

    this->pen = QPen(Qt::white); // Outline of shapes and lines
    this->brush = QBrush(Qt::white); // Fill pattern of shapes

    this->analog_path.reserve(x_axis_width_in_samples);

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

    this->x_axis_width_in_samples = n_samples;

    //Reserve painter path
    this->analog_path.reserve(n_samples);

    update();
}

void AnalogRenderArea::calculate_analog_line_offsets() {

    float spacing = static_cast<float>(height()) / (this->analog_line_offsets.size() + 1);
    for (int x = 0; x < this->analog_line_offsets.size(); x++) {
        analog_line_offsets[x] = spacing * (x + 1);
    }
}
void AnalogRenderArea::setVerticalZoom(int64_t n_lines_to_show) {

    this->analog_line_offsets = std::vector<float>(n_lines_to_show);

    calculate_analog_line_offsets();

    update();
}

void AnalogRenderArea::mouseMoveEvent(QMouseEvent *event){

    if (this->debug) {
        qDebug() << event->pos();
    }

    this->last_mouse_event_coords = event->pos();

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
    qreal scale_x = this->x_axis_width_in_samples / this->width();

    for (int x = 0; x < this->analog_line_offsets.size(); x += 1) {

        // Save painter before each line that is drawn
        painter.save();

        painter.scale(scale_x,1.0);

        float y_offset = this->analog_line_offsets[0];

        drawAnalogLine(painter,y_offset);

        painter.restore();
    }

}

void AnalogRenderArea::drawAnalogLine(QPainter& painter, float y_offset) {

    analog_path.moveTo(0, y_offset);

    for (int x = 0; x < this->x_axis_width_in_samples; x += 1 ) {
        analog_path.lineTo(x, y_offset);
    }

    painter.drawPath(analog_path);

    analog_path.clear();
}
