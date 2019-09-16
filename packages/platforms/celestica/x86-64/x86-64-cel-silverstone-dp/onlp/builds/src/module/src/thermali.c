#include <unistd.h>
#include <onlplib/mmap.h>
#include <onlplib/file.h>
#include <onlp/platformi/thermali.h>
#include <fcntl.h>
#include "platform.h"

static onlp_thermal_info_t thermal_info[] = {
    {},
    {{ONLP_THERMAL_ID_CREATE(1), "Temp_CPU", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(2), "TEMP_BB_U3", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(3), "TEMP_SW_U16", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(4), "TEMP_SW_U52", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(5), "TEMP_FAN_U17", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(6), "TEMP_FAN_U52", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(7), "PSUL_Temp1", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(8), "PSUL_Temp2", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(9), "PSUR_Temp1", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
    {{ONLP_THERMAL_ID_CREATE(10), "PSUR_Temp2", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
     {{ONLP_THERMAL_ID_CREATE(11), "TEMP_SW_Internal", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
     {{ONLP_THERMAL_ID_CREATE(12), "SW_U45_Temp", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
     {{ONLP_THERMAL_ID_CREATE(13), "SW_U72_Temp", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
     {{ONLP_THERMAL_ID_CREATE(14), "SW_U87_Temp", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
     {{ONLP_THERMAL_ID_CREATE(15), "SW_U103_Temp", 0},
     ONLP_THERMAL_STATUS_PRESENT,
     ONLP_THERMAL_CAPS_ALL,
     0,
     ONLP_THERMAL_THRESHOLD_INIT_DEFAULTS},
};

int onlp_thermali_init(void)
{
    return ONLP_STATUS_OK;
}

int onlp_thermali_info_get(onlp_oid_t id, onlp_thermal_info_t *info_p)
{
    int thermal_id;
    int thermal_status = 0;
    int temp, warn, err, shutdown;

    thermal_id = ONLP_OID_ID_GET(id);
    memcpy(info_p, &thermal_info[thermal_id], sizeof(onlp_thermal_info_t));

    /* Get thermal temperature. */
    thermal_status = getSensorInfo(thermal_id, &temp, &warn, &err, &shutdown);
    if (thermal_status == -1)
    {
        info_p->status = ONLP_THERMAL_STATUS_FAILED;
    }
    else
    {
        info_p->status = ONLP_THERMAL_STATUS_PRESENT;
        info_p->mcelsius = temp;
        info_p->thresholds.warning = warn;
        info_p->thresholds.error = err;
        info_p->thresholds.shutdown = shutdown;
    }

    return ONLP_STATUS_OK;
}
