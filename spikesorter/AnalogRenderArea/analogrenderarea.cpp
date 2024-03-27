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
    _debug = false;
    _track_horizontal = true;
    _canvas_label_width = 30.0;
    _global_gain = 1.0;

    int virtual_data_n_channel = 32;
    int sample_rate = 30000;
    int channels_to_display = 1;

    createVirtualData(virtual_data_n_channel,sample_rate * 5);

    _x_axis_props = AnalogRenderXAxisProps();
    _x_axis_props.setXAxisProps(sample_rate, sample_rate * 5);

    _y_axis_props = AnalogRenderYAxisProps();
    _y_axis_props.setYAxisProps(virtual_data_n_channel, channels_to_display);

    _antialiased = false;
    _pixmap.load(":/images/qt-logo.png");

    _pen = QPen(Qt::white); // Outline of shapes and lines
    _brush = QBrush(Qt::NoBrush); // Fill pattern of shapes

    _analog_path.reserve(_x_axis_props.getSamplesToShow());

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

    update();
}

void AnalogRenderArea::setPen(const QPen &pen)
{
    _pen = pen;
    update();
}

void AnalogRenderArea::setBrush(const QBrush &brush)
{
    _brush = brush;
    update();
}

void AnalogRenderArea::setAntialiased(bool antialiased)
{
    _antialiased = antialiased;
    update();
}

void AnalogRenderArea::setHorizontalZoom(int64_t n_samples) {

    _x_axis_props.setSamplesToShow(n_samples);

    //Reserve painter path
    _analog_path.reserve(n_samples);

    update();
}

void AnalogRenderArea::setCenterSample(int64_t sample) {

    _x_axis_props.setCenterSample(sample);

    update();
}

void AnalogRenderArea::setCenterChannel(int64_t channel) {

    _y_axis_props.setScrollBarPosition(channel);

    update();
}

void AnalogRenderArea::setGain(float gain) {

    _global_gain = gain;

    update();
}

float AnalogRenderArea::calculate_horizontal_scale() {
    //Calculates the scale in pixels / samples

    float canvas_width = static_cast<float>(this->width()) - _canvas_label_width; // Reserve 30 pixels for labels

    return  canvas_width / static_cast<float>(_x_axis_props.getSamplesToShow());
}

void AnalogRenderArea::setVerticalZoom(int64_t n_lines_to_show) {

    _y_axis_props.setChannelsToDisplay(n_lines_to_show);

    update();
}

void AnalogRenderArea::mouseMoveEvent(QMouseEvent *event){

    int64_t first_sample = _x_axis_props.getFirstSample();

    float sample_under_mouse_event = getMousePositionInSamples(event);

    if (_debug) {
        qDebug() << event->pos();
        qDebug() << "This corresponds to sample " << sample_under_mouse_event + first_sample;
        qDebug() << "Nearest channel is " << getNearestChannelToMouse(event);
    }

    _last_mouse_event_coords = event->pos();

    update();
}

float AnalogRenderArea::getMousePositionInSamples(QMouseEvent *event) {
    float mouse_x = static_cast<float>(event->pos().x());

    if (mouse_x < _canvas_label_width) {
        return 0.0;
    } else {
        return (mouse_x - _canvas_label_width) / calculate_horizontal_scale();
    }
}

int AnalogRenderArea::getNearestChannelToMouse(QMouseEvent *event) {

    float mouse_y_pos = static_cast<float>(event->pos().y());

    int channel_to_check = 0;

    while(channel_to_check < _y_axis_props.getNChannelsToDisplay()) {

        float lower_bound = _y_axis_props.getLowerChannelBound(channel_to_check) * static_cast<float>(height());

        if (mouse_y_pos < lower_bound) {
            break;
        } else {
            channel_to_check += 1;
        }
    }
    return _y_axis_props.getDataChannelFromDisplayChannel(channel_to_check);
}

void AnalogRenderArea::paintEvent(QPaintEvent * /* event */)
{

    QElapsedTimer paintTimer;
    paintTimer.start();

    QPainter painter(this);

    drawBackground(painter);

    drawAnalogLines(painter);

    if (_track_horizontal) {
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
    painter.drawLine(
            _last_mouse_event_coords.x(),
            0,
            _last_mouse_event_coords.x(),
            height()
        );

    painter.restore();
}

void AnalogRenderArea::drawAnalogLines(QPainter& painter) {

    painter.setPen(_pen);
    painter.setBrush(_brush);
    if (_antialiased) {
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

    // Calculate the horizontal scale factor
    qreal scale_x = calculate_horizontal_scale();
    qreal global_y_gain = 1.0;

    for (int x = 0; x < _y_axis_props.getNChannelsToDisplay(); x += 1) {

        // Save painter before each line that is drawn
        painter.save();

        int data_channel = _y_axis_props.getDataChannelFromDisplayChannel(x);

        float y_offset = _y_axis_props.getLineOffset(x) * static_cast<float>(height());
        float this_channel_gain = _y_axis_props.getChannelGain(data_channel);

        //Scaling by Y gain here seems to be very slow compared to just multiplying
        //Path values by gain. NOt sure why
        qreal local_y_gain = global_y_gain + this_channel_gain;

        painter.translate(_canvas_label_width, y_offset);
        painter.scale(scale_x,local_y_gain);

        drawAnalogLine(painter,_virtual_data[data_channel]);

        painter.restore();
    }

    drawChannelLabels(painter);

}

void AnalogRenderArea::drawChannelLabels(QPainter& painter) {

    QFont font = painter.font() ;
    font.setPixelSize(16);

    for (int x = 0; x < _y_axis_props.getNChannelsToDisplay(); x += 1) {

        // Save painter before each line that is drawn
        painter.save();

        painter.setFont(font);

        int data_channel = _y_axis_props.getDataChannelFromDisplayChannel(x);

        float y_offset = _y_axis_props.getLineOffset(x) * static_cast<float>(height());

        painter.drawText(
            QPointF(10.0,y_offset),
            QString::fromStdString(std::to_string(data_channel))
            );

        painter.restore();
    }
}

void AnalogRenderArea::drawAnalogLine(QPainter& painter, std::vector<float>& data) {

    int first_ind = _x_axis_props.getFirstSample();

    _analog_path.moveTo(0, data[first_ind] * _global_gain);

    for (int x = 0; x < _x_axis_props.getSamplesToShow(); x += 1 ) {
        _analog_path.lineTo(x, data[first_ind + x] * _global_gain);
    }

    painter.drawPath(_analog_path);

    _analog_path.clear();
}

void AnalogRenderArea::createVirtualData(int n_channels, int n_samples) {

    std::random_device random_device;
    std::mt19937 random_engine{random_device()};
    std::uniform_real_distribution distribution100 = std::uniform_real_distribution<float>(-10,10);

    _virtual_data = std::vector<std::vector<float>>(n_channels);

    auto gen = [&distribution100, &random_engine](){
        return distribution100(random_engine);
    };
    for (int i = 0; i < _virtual_data.size(); i ++) {
        _virtual_data[i] = std::vector<float>(n_samples);
        std::generate(begin(_virtual_data[i]), end(_virtual_data[i]), gen);
    }

    // Add spikes
    for (int i = 0; i < _virtual_data.size(); i ++) {
        auto spikes =  std::vector<float>(n_samples);
        std::generate(begin(spikes),end(spikes),gen);
        for (int j = 0; j < n_samples; j++) {
            if (spikes[j] > 9.5) {
                _virtual_data[i][j] += 10.0;
            } else if (spikes[j] < -9.5) {
                _virtual_data[i][j] += -10.0;
            }
        }
    }
}
