#ifndef ANALOGRENDERYAXISPROPS_HPP
#define ANALOGRENDERYAXISPROPS_HPP

#include <cstdint>
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
    AnalogRenderYAxisProps();

    void setYAxisProps(int64_t n_channels_total, int64_t n_channels_display);

    /**
     * @brief setChannelsToDisplay
     *
     *
     *
     * @param n_channels_to_display
     */
    void setChannelsToDisplay(int64_t n_channels_to_display);

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

    void setScrollBarPosition(int64_t position);

private:

    int64_t _n_channels_total;
    int64_t _n_channels_to_show;

    int64_t _scroll_bar_position;

    std::vector<int64_t> _which_channels_to_show;
    std::vector<float> _per_channel_gain;
    std::vector<float> _line_offsets;
    std::vector<float> _lower_channel_bounds;

    void _calculate_line_offsets();

    void _calculate_channel_bounds();

    void _calculate_which_channels_to_show();

};

#endif // ANALOGRENDERYAXISPROPS_HPP
