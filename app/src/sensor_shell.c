#include <bugis_driver.h>
#include <zephyr/device.h>
#include <zephyr/shell/shell.h>

static const struct device *sensor_dev =
    DEVICE_DT_GET(DT_NODELABEL(bugis_driver0));

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv) {
  int ret = sensor_sample_fetch(sensor_dev);

  if (ret < 0) {
    shell_error(sh, "fetch failed: %d", ret);
  } else {
    shell_print(sh, "fetch ok");
  }
  return ret;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv) {
  struct sensor_value val;
  int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

  if (ret < 0) {
    shell_error(sh, "read failed: %d", ret);
  } else {
    shell_print(sh, "val: %d", val.val);
  }
  return ret;
}

static int cmd_info(const struct shell *sh, size_t argc, char **argv) {
  shell_print(sh, "name: %s", sensor_dev->name);
  shell_print(sh, "ready: %s", device_is_ready(sensor_dev) ? "yes" : "no");
  return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_cmds,
    SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch", cmd_fetch),
    SHELL_CMD(read, NULL, "Call sensor_channel_get and print result",
              cmd_read),
    SHELL_CMD(info, NULL, "Print device name and ready state", cmd_info),
    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sensor_cmds, "Sensor shell commands", NULL);
