#ifndef ANALOGRENDERXAXISPROPS_HPP
#define ANALOGRENDERXAXISPROPS_HPP

#include <cstdint>

class AnalogRenderXAxisProps {
public:
    AnalogRenderXAxisProps();

    void setXAxisProps(float sampling_rate, int n_samples_total);

    void setSamplesToShow(int64_t n_samples);

    void setCenterSample(int64_t sample);

    int64_t getCenterSample() {return _center_sample;};

    int64_t getSamplesToShow() {
        return _last_sample_to_show - _first_sample_to_show;
    };

    int64_t getFirstSample() {return _first_sample_to_show;};

    float getSamplingRate() {return _sampling_rate;};
private:
    float _sampling_rate;
    int64_t _n_samples_total;

    int64_t _n_samples_to_show;

    int64_t _center_sample;
    int64_t _first_sample_to_show;
    int64_t _last_sample_to_show;

    void _calculate_first_and_last_sample_to_show();
};

#endif // ANALOGRENDERXAXISPROPS_HPP
