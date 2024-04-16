
#include "AnalogRenderYAxisProps.hpp"

AnalogRenderYAxisProps::AnalogRenderYAxisProps()
{
    _n_channels_total = 0;
    _scroll_bar_position = 1;
};

void AnalogRenderYAxisProps::setYAxisProps(int64_t n_channels_total, int64_t n_channels_display)
{
    _n_channels_total = n_channels_total;
    _n_channels_to_show = n_channels_display;

    _per_channel_gain = std::vector<float>(n_channels_total, 0.0);

    _calculate_line_offsets();
    _calculate_channel_bounds();
    _calculate_which_channels_to_show();

};

void AnalogRenderYAxisProps::setChannelsToDisplay(int64_t n_channels_to_display)
{
    _n_channels_to_show = n_channels_to_display;
    _calculate_line_offsets();
    _calculate_channel_bounds();
    _calculate_which_channels_to_show();
};

void AnalogRenderYAxisProps::setScrollBarPosition(int64_t position)
{

    //Error checking
    if ((position < 0) || (position > _n_channels_total)) {
        std::cout
            << "Scroll bar position received by AnalogRenderYAxis doesn't make sense"
            << std::endl;
    }

    _scroll_bar_position = position;
    _calculate_which_channels_to_show();
};

void AnalogRenderYAxisProps::_calculate_line_offsets()
{

    _line_offsets = std::vector<float>(_n_channels_to_show);

    float spacing = 1.0 / (_line_offsets.size() + 1);
    for (int x = 0; x < _line_offsets.size(); x++) {
        _line_offsets[x] = spacing * (x + 1);
    }
};

void AnalogRenderYAxisProps::_calculate_channel_bounds()
{

    _lower_channel_bounds = std::vector<float>(_n_channels_to_show);

    float spacing = 1.0 / static_cast<float>(_lower_channel_bounds.size());
    for (int x = 0; x < _lower_channel_bounds.size(); x++) {
        _lower_channel_bounds[x] = spacing * static_cast<float>(x + 1);
    }
};

void AnalogRenderYAxisProps::_calculate_which_channels_to_show()
{

    _which_channels_to_show = std::vector<int64_t>(_n_channels_to_show, 0);

    int start_channel = _scroll_bar_position;

    if ((start_channel + _n_channels_to_show) > _n_channels_total) {
        start_channel = _n_channels_total - _n_channels_to_show;
    }

    for (int i = 0; i < _n_channels_to_show; i++) {
        _which_channels_to_show[i] = start_channel + i - 1;
    }
};
