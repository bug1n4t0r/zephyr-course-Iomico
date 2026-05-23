
#include <bugis_driver.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{

    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(bugis_driver0));
    struct sensor_value val;
    struct sensor_value *my_data = (struct sensor_value *)driver->data;
    uint8_t zahl = 0;

    while (1)
    {
        bugis_set_val(driver, &zahl);
        sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        sensor_sample_fetch(driver);
        bugis_get_val(driver, &zahl);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        LOG_INF("%d", my_data->val);
        zahl++;
    }
    return 0;
}
