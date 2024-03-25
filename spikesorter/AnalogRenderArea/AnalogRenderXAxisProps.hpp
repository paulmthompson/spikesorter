#ifndef ANALOGRENDERXAXISPROPS_HPP
#define ANALOGRENDERXAXISPROPS_HPP

#include <cstdint>

class AnalogRenderXAxisProps {
public:
    AnalogRenderXAxisProps() {
        sampling_rate = 30000.0;
        n_samples_total = 0;

        center_sample = 0;

        n_samples_to_show = 10000;
        calculate_first_and_last_sample_to_show();
    }
    void setXAxisProps(float sampling_rate, int n_samples_total) {
        this->n_samples_total = n_samples_total;
        this->sampling_rate = sampling_rate;

        calculate_first_and_last_sample_to_show();
    };
    void setSamplesToShow(int64_t n_samples) {
        n_samples_to_show = n_samples;
        calculate_first_and_last_sample_to_show();
    };
    void setCenterSample(int64_t sample) {
        center_sample = sample;
    };
    int64_t getSamplesToShow() {
        return last_sample_to_show - first_sample_to_show;
    };
    int64_t getFirstSample() {
        return this->first_sample_to_show;
    }
private:
    float sampling_rate;
    int64_t n_samples_total;

    int64_t n_samples_to_show;

    int64_t center_sample;
    int64_t first_sample_to_show;
    int64_t last_sample_to_show;

    void calculate_first_and_last_sample_to_show() {

        if (this->n_samples_total == 0) {
            return;
        }

        int64_t left_edge_sample = center_sample - n_samples_to_show / 2;
        int64_t right_edge_sample = center_sample + n_samples_to_show / 2 - 1;

        if (left_edge_sample < 0) {
            left_edge_sample = 0;
            right_edge_sample = n_samples_to_show - 1;
        }
        if (right_edge_sample >= n_samples_total) {
            right_edge_sample = n_samples_total - 1;
            left_edge_sample = right_edge_sample - n_samples_to_show;
            if (left_edge_sample < 0) {
                left_edge_sample = 0;
            }
        }

        first_sample_to_show = left_edge_sample;
        last_sample_to_show = right_edge_sample;

    }
};

#endif // ANALOGRENDERXAXISPROPS_HPP
