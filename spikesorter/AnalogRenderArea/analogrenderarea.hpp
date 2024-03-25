//
// Created by wanglab on 3/22/2024.
//
// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SPIKESORTER_ANALOGRENDERAREA_HPP
#define SPIKESORTER_ANALOGRENDERAREA_HPP

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QMouseEvent>

class AnalogRenderArea : public QWidget
{
Q_OBJECT

public:

    explicit AnalogRenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setHorizontalZoom(int64_t n_samples);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    bool debug;

    bool track_horizontal;

    QPen pen;
    QBrush brush;

    bool antialiased;
    QPixmap pixmap;
    QPoint last_mouse_event_coords;

    int64_t x_axis_width_in_samples;

    void drawBackground(QPainter& painter);
    void drawMouseVerticalLine(QPainter& painter);
    void drawAnalogLines(QPainter& painter);
    void drawAnalogLine(QPainter& painter);
};


#endif //SPIKESORTER_ANALOGRENDERAREA_HPP
