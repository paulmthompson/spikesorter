#ifndef ANALOGRENDERYAXISPROPS_HPP
#define ANALOGRENDERYAXISPROPS_HPP

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

/**
 *
 * The AnalogRenderYAxisProps class
 *
 * The AnalogRenderArea plots a subset of M channels
 * from N total channels. AnalogRenderYAxisProps ensures
 * that different M channels can be select and scrolled
 * through.
 *
 */

class AnalogRenderYAxisProps {
public:
    AnalogRenderYAxisProps() {
        _n_channels_total = 0;
        _scroll_bar_position = 1;

    };

    void setYAxisProps(int64_t n_channels_total, int64_t n_channels_display) {
        _n_channels_total = n_channels_total;
        _n_channels_to_show = n_channels_display;

        _per_channel_gain = std::vector<float>(n_channels_total, 0.0);

        calculate_line_offsets();
        calculate_channel_bounds();
        calculate_which_channels_to_show();

    };

    /**
     * @brief setChannelsToDisplay
     *
     *
     *
     * @param n_channels_to_display
     */
    void setChannelsToDisplay(int64_t n_channels_to_display) {
        _n_channels_to_show = n_channels_to_display;
        calculate_line_offsets();
        calculate_channel_bounds();
        calculate_which_channels_to_show();
    };

    float getLineOffset(int channel) {
        return _line_offsets[channel];
    };
    float getLowerChannelBound(int channel) {
        return _lower_channel_bounds[channel];
    }
    float getChannelGain(int data_channel) {
        return _per_channel_gain[data_channel];
    }
    int64_t getNChannelsToDisplay() {
        return _n_channels_to_show;
    };
    int64_t getDataChannelFromDisplayChannel(int display_channel) {
        return _which_channels_to_show[display_channel];
    }

    void setScrollBarPosition(int64_t position) {

        //Error checking
        if ((position < 0) || (position > _n_channels_total)) {
            std::cout
                << "Scroll bar position received by AnalogRenderYAxis doesn't make sense"
                << std::endl;
        }

        _scroll_bar_position = position;
        calculate_which_channels_to_show();
    };

private:

    void calculate_line_offsets() {

        _line_offsets = std::vector<float>(_n_channels_to_show);

        float spacing = 1.0 / (_line_offsets.size() + 1);
        for (int x = 0; x < _line_offsets.size(); x++) {
            _line_offsets[x] = spacing * (x + 1);
        }
    };

    void calculate_channel_bounds() {

        _lower_channel_bounds = std::vector<float>(_n_channels_to_show);

        float spacing = 1.0 / static_cast<float>(_lower_channel_bounds.size());
        for (int x = 0; x < _lower_channel_bounds.size(); x++) {
            _lower_channel_bounds[x] = spacing * static_cast<float>(x + 1);
        }
    };

    void calculate_which_channels_to_show() {

        _which_channels_to_show = std::vector<int64_t>(_n_channels_to_show, 0);

        int start_channel = _scroll_bar_position;

        if ((start_channel + _n_channels_to_show) > _n_channels_total) {
            start_channel = _n_channels_total - _n_channels_to_show;
        }

        for (int i = 0; i < _n_channels_to_show; i++) {
            _which_channels_to_show[i] = start_channel + i - 1;
        }
    };

    int64_t _n_channels_total;
    int64_t _n_channels_to_show;

    int64_t _scroll_bar_position;

    std::vector<int64_t> _which_channels_to_show;
    std::vector<float> _per_channel_gain;
    std::vector<float> _line_offsets;
    std::vector<float> _lower_channel_bounds;
};

#endif // ANALOGRENDERYAXISPROPS_HPP
