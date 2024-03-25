#ifndef ANALOGRENDERYAXISPROPS_HPP
#define ANALOGRENDERYAXISPROPS_HPP

#include <cstdint>
#include <vector>

class AnalogRenderYAxisProps {
public:
    AnalogRenderYAxisProps() {
        n_channels_total = 0;

    };
    void setYAxisProps(int64_t n_channels_total, int64_t n_channels_display) {
        this->n_channels_total = n_channels_total;
        this->n_channels_to_show = n_channels_display;

        this->per_channel_gain = std::vector<float>(n_channels_total, 0.0);

        calculate_line_offsets();
        calculate_channel_bounds();
        calculate_which_channels_to_show();

    };
    void setChannelsToDisplay(int64_t n_channels_to_display) {
        this->n_channels_to_show = n_channels_to_display;
        calculate_line_offsets();
        calculate_channel_bounds();
        calculate_which_channels_to_show();
    };
    float getLineOffset(int channel) {
        return line_offsets[channel];
    };
    float getLowerChannelBound(int channel) {
        return lower_channel_bounds[channel];
    }
    float getChannelGain(int data_channel) {
        return this->per_channel_gain[data_channel];
    }
    int64_t getNChannelsToDisplay() {
        return this->n_channels_to_show;
    };
    int64_t getDataChannelFromDisplayChannel(int display_channel) {
        return this->which_channels_to_show[display_channel];
    }

private:

    void calculate_line_offsets() {

        this->line_offsets = std::vector<float>(this->n_channels_to_show);

        float spacing = 1.0 / (this->line_offsets.size() + 1);
        for (int x = 0; x < this->line_offsets.size(); x++) {
            line_offsets[x] = spacing * (x + 1);
        }
    };
    void calculate_channel_bounds() {

        this->lower_channel_bounds = std::vector<float>(this->n_channels_to_show);

        float spacing = 1.0 / static_cast<float>(this->lower_channel_bounds.size());
        for (int x = 0; x < this->lower_channel_bounds.size(); x++) {
            lower_channel_bounds[x] = spacing * static_cast<float>(x + 1);
        }
    };

    void calculate_which_channels_to_show() {

        this->which_channels_to_show = std::vector<int64_t>(this->n_channels_to_show,0);
        for (int i = 0; i < this->n_channels_to_show; i++) {
            this->which_channels_to_show[i] = i;
        }
    };

    int64_t n_channels_total;
    int64_t n_channels_to_show;
    std::vector<int64_t> which_channels_to_show;
    std::vector<float> per_channel_gain;
    std::vector<float> line_offsets;
    std::vector<float> lower_channel_bounds;
};

#endif // ANALOGRENDERYAXISPROPS_HPP
