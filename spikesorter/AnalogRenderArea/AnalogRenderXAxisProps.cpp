
#include "AnalogRenderXAxisProps.hpp"

AnalogRenderXAxisProps::AnalogRenderXAxisProps()
{
    _sampling_rate = 30000.0;
    _n_samples_total = 0;

    _center_sample = 0;

    _n_samples_to_show = 10000;
    _calculate_first_and_last_sample_to_show();
}

void AnalogRenderXAxisProps::setXAxisProps(float sampling_rate, int n_samples_total)
{
    _n_samples_total = n_samples_total;
    _sampling_rate = sampling_rate;

    _calculate_first_and_last_sample_to_show();
};

void AnalogRenderXAxisProps::setSamplesToShow(int64_t n_samples)
{
    _n_samples_to_show = n_samples;
    _calculate_first_and_last_sample_to_show();
};

void AnalogRenderXAxisProps::setCenterSample(int64_t sample)
{
    _center_sample = sample;
    _calculate_first_and_last_sample_to_show();
};

void AnalogRenderXAxisProps::_calculate_first_and_last_sample_to_show()
{

    if (_n_samples_total == 0) {
        return;
    }

    int64_t left_edge_sample = _center_sample - _n_samples_to_show / 2;
    int64_t right_edge_sample = _center_sample + _n_samples_to_show / 2 - 1;

    if (left_edge_sample < 0) {
        left_edge_sample = 0;
        right_edge_sample = _n_samples_to_show - 1;
    }
    if (right_edge_sample >= _n_samples_total) {
        right_edge_sample = _n_samples_total - 1;
        left_edge_sample = right_edge_sample - _n_samples_to_show;
        if (left_edge_sample < 0) {
            left_edge_sample = 0;
        }
    }

    _first_sample_to_show = left_edge_sample;
    _last_sample_to_show = right_edge_sample;

}
