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
#include <QPainterPath>
#include "AnalogRenderXAxisProps.hpp"
#include "AnalogRenderYAxisProps.hpp"

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
    void setVerticalZoom(int64_t n_lines_to_show);

    void setCenterSample(int64_t sample);
    void setCenterChannel(int64_t channel);
    void setGain(float gain);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    bool debug;

    bool track_horizontal;

    float canvas_label_width;

    float global_gain;

    QPen pen;
    QBrush brush;

    QPainterPath analog_path;

    bool antialiased;
    QPixmap pixmap;
    QPoint last_mouse_event_coords;

    std::vector<std::vector<float>> virtual_data;

    int64_t x_axis_width_in_samples;
    AnalogRenderXAxisProps XAxisProps;
    AnalogRenderYAxisProps YAxisProps;

    float calculate_horizontal_scale();

    void drawBackground(QPainter& painter);
    void drawMouseVerticalLine(QPainter& painter);
    float getMousePositionInSamples(QMouseEvent *event);
    int getNearestChannelToMouse(QMouseEvent *event);
    void drawAnalogLines(QPainter& painter);
    void drawAnalogLine(QPainter& painter, std::vector<float>& data);
    void createVirtualData(int n_channels, int n_samples);
    void drawChannelLabels(QPainter& painter);
};


#endif //SPIKESORTER_ANALOGRENDERAREA_HPP
