// #include <zephyr/drivers/sensor.h>
#include <bugis_driver.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT bugis_driver

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(heartbeat_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(bugis_driver, LOG_LEVEL_INF);

static bool led_state = true;

static struct sensor_value bugis_val = {.val = 0};

static int channel_get_bugis_impl(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    gpio_pin_set_dt(&led, 0);
    led_state = (bool)gpio_pin_get_dt(&led);
    LOG_INF("LED state: %s (channel get)", led_state ? "ON" : "OFF");
    return 0;
}

static int sample_fetch_bugis_impl(const struct device *dev,
                                   enum sensor_channel chan)
{
    gpio_pin_set_dt(&led, 1);
    led_state = (bool)gpio_pin_get_dt(&led);
    LOG_INF("LED state: %s (sample fetch)", led_state ? "ON" : "OFF");
    return 0;
}

static int set_value(const struct device *dev, uint8_t *val)
{
    bugis_val.val = *val;
    LOG_INF("Value is set!");
    return 0;
}

static int get_value(const struct device *dev, uint8_t *val)
{
    *val = bugis_val.val;
    LOG_INF("Got Value!");
    return 0;
}

static DEVICE_API(bugis, api_bugis_driver) = {
    .channel_get = channel_get_bugis_impl,
    .sample_fetch = sample_fetch_bugis_impl,
    .set_val = set_value,
    .get_val = get_value};

static int init(const struct device *dev)
{
    LOG_INF("Driver Initialized!");

    if (!gpio_is_ready_dt(&led))
        return -ENODEV;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
        return -ENODEV;

    return 0;
}

#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, &bugis_val, NULL, POST_KERNEL, 80, &api_bugis_driver);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);