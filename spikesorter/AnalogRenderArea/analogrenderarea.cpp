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

    x_axis_width_in_samples = 1000;

    antialiased = false;
    pixmap.load(":/images/qt-logo.png");

    this->pen = QPen(Qt::white); // Outline of shapes and lines
    this->brush = QBrush(Qt::white); // Fill pattern of shapes

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

    // Calculate the number of lines based on vertical zoom, and y positions of those
    uint16_t n_lines_to_draw = 1;

    // Calculate the horizontal scale factor
    qreal scale_x = this->x_axis_width_in_samples / this->width();

    for (int x = 0; x < n_lines_to_draw; x += 1) {

        // Save painter before each line that is drawn
        painter.save();

        painter.scale(scale_x,1.0);

        drawAnalogLine(painter);

        painter.restore();
    }

}

void AnalogRenderArea::drawAnalogLine(QPainter& painter) {

    QPainterPath path;

    qreal y_offset = height() / 2;

    path.moveTo(0, y_offset);

    for (int x = 0; x < this->x_axis_width_in_samples; x += 1 ) {
        path.lineTo(x, y_offset);
    }

    painter.drawPath(path);
}
