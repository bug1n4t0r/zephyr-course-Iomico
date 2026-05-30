#ifndef __BUGISDRIVER_H_
#define __BUGISDRIVER_H_

#include <stdint.h>
#include <zephyr/device.h>

enum sensor_channel {
  SENSOR_CHAN_AMBIENT_TEMP,
  SENSOR_CHAN_ALL
};

struct sensor_value {
  int8_t val;
};

typedef int (*sensor_sample_fetch_t)(const struct device *dev,
                                     enum sensor_channel  chan);

typedef int (*sensor_channel_get_t)(const struct device *dev,
                                    enum sensor_channel  chan,
                                    struct sensor_value *val);

typedef int (*bugis_set_value_t)(const struct device *dev, uint8_t *val);
typedef int (*bugis_get_value_t)(const struct device *dev, uint8_t *val);

__subsystem struct bugis_driver_api {
  bugis_set_value_t     set_val;
  bugis_get_value_t     get_val;
  sensor_sample_fetch_t sample_fetch;
  sensor_channel_get_t  channel_get;
};

static inline int bugis_set_val(const struct device *dev, uint8_t *val) {
  const struct bugis_driver_api *api =
      (const struct bugis_driver_api *)dev->api;
  return api->set_val(dev, val);
}

static inline int bugis_get_val(const struct device *dev, uint8_t *val) {
  const struct bugis_driver_api *api =
      (const struct bugis_driver_api *)dev->api;
  return api->get_val(dev, val);
}

static inline int sensor_channel_get(const struct device *dev,
                                     enum sensor_channel  chan,
                                     struct sensor_value *val) {
  const struct bugis_driver_api *api =
      (const struct bugis_driver_api *)dev->api;
  return api->channel_get(dev, chan, val);
}

static inline int sensor_sample_fetch(const struct device *dev) {
  const struct bugis_driver_api *api =
      (const struct bugis_driver_api *)dev->api;
  return api->sample_fetch(dev, SENSOR_CHAN_ALL);
}

#endif