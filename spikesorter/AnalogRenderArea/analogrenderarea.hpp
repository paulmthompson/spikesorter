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

/**
 *
 * AnalogRanderArea is used for plotting multiple time-dependent signals
 * Signals can be plotted with vertical offset, or some can be overlaid
 *
 * @brief The AnalogRenderArea class
 */
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
    bool _debug;

    bool _track_horizontal;

    float _canvas_label_width;

    float _global_gain;

    bool _antialiased;

    QPen _pen;
    QBrush _brush;

    QPainterPath _analog_path;

    QPixmap _pixmap;
    QPoint _last_mouse_event_coords;

    std::vector<std::vector<float>> _virtual_data;

    int64_t _x_axis_width_in_samples;
    AnalogRenderXAxisProps _x_axis_props;
    AnalogRenderYAxisProps _y_axis_props;

    float _calculate_horizontal_scale();

    void _drawBackground(QPainter& painter);
    void _drawMouseVerticalLine(QPainter& painter);
    float _getMousePositionInSamples(QMouseEvent *event);
    int _getNearestChannelToMouse(QMouseEvent *event);
    void _drawAnalogLines(QPainter& painter);
    void _drawAnalogLine(QPainter& painter, std::vector<float>& data);
    void _createVirtualData(int n_channels, int n_samples);
    void _drawChannelLabels(QPainter& painter);
};


#endif //SPIKESORTER_ANALOGRENDERAREA_HPP
