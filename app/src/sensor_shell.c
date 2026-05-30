#include <bugis_driver.h>
#include <stdlib.h>
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

static int cmd_set(const struct shell *sh, size_t argc, char **argv) {
  char *end;
  long  v = strtol(argv[1], &end, 10);

  if (*end != '\0' || v < 0 || v > UINT8_MAX) {
    shell_error(sh, "value must be 0-255");
    return -EINVAL;
  }

  uint8_t u8  = (uint8_t)v;
  int     ret = bugis_set_val(sensor_dev, &u8);

  uint8_t get;
  bugis_get_val(sensor_dev, &get);

  if (ret < 0) {
    shell_error(sh, "set failed: %d", ret);
  } else {
    shell_print(sh, "set ok: %d", get);
  }
  return ret;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_cmds, SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch", cmd_fetch),
    SHELL_CMD(read, NULL, "Call sensor_channel_get and print result", cmd_read),
    SHELL_CMD(info, NULL, "Print device name and ready state", cmd_info),
    SHELL_CMD_ARG(set, NULL, "Call bugis_set_val <value> (0-255)", cmd_set, 2,
                  0),
    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sensor_cmds, "Sensor shell commands", NULL);
