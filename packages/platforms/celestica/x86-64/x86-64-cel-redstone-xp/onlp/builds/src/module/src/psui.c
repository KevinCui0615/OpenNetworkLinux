/************************************************************
 * <bsn.cl fy=2014 v=onl>
 *
 *           Copyright 2014 Big Switch Networks, Inc.
 *           Copyright 2017 Celestica Corporation.
 *
 * Licensed under the Eclipse Public License, Version 1.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *        http://www.eclipse.org/legal/epl-v10.html
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the
 * License.
 *
 * </bsn.cl>
 ************************************************************
 *
 *
 *
 ***********************************************************/
#include <onlp/platformi/psui.h>
#include <onlplib/mmap.h>
#include <stdio.h>
#include <string.h>
#include "platform_lib.h"


#define PSU_NODE_MAX_INT_LEN  8
#define PSU_NODE_MAX_PATH_LEN 64

#define I2C_PSU_MODEL_NAME_LEN 9

#define VALIDATE(_id)                           \
    do {                                        \
        if(!ONLP_OID_IS_PSU(_id)) {             \
            return ONLP_STATUS_E_INVALID;       \
        }                                       \
    } while(0)

static int 
psu_status_info_get(int id, char *node, int *value)
{
    int ret = 0;
    char buf[PSU_NODE_MAX_INT_LEN + 1] = {0};
    char node_path[PSU_NODE_MAX_PATH_LEN] = {0};
    
    *value = 0;

    if (PSU1_ID == id) {
        sprintf(node_path, "%s%s", PSU1_AC_PMBUS_PREFIX, node);
    }
    else if (PSU2_ID == id) {
        sprintf(node_path, "%s%s", PSU2_AC_PMBUS_PREFIX, node);
    }
    
    ret = deviceNodeReadString(node_path, buf, sizeof(buf), 0);

    if (ret == 0) {
        *value = atoi(buf);
    }

    return ret;
}

static int get_psu_type(int id, char* modelname, int modelname_len)
{
    char *node = NULL;
    char  model_name[I2C_PSU_MODEL_NAME_LEN + 1] = {0};
    

    /* Check AC model name */
    node = (id == PSU1_ID) ? PSU1_AC_PMBUS_NODE(psu_mfr_model) : PSU2_AC_PMBUS_NODE(psu_mfr_model);

    if (deviceNodeReadString(node, model_name, sizeof(model_name), 0) != 0) {
        return PSU_TYPE_UNKNOWN;
    }

    if (modelname) {
        strncpy(modelname, model_name, modelname_len-1);
    }

    return 0;
}


static int
psu_ym2651y_pmbus_info_get(int id, char *node, int *value)
{
    int  ret = 0;
    char buf[PSU_NODE_MAX_INT_LEN + 1]    = {0};
    char node_path[PSU_NODE_MAX_PATH_LEN] = {0};
    
    *value = 0;

    if (PSU1_ID == id) {
        sprintf(node_path, "%s%s", PSU1_AC_PMBUS_PREFIX, node);
    }
    else {
        sprintf(node_path, "%s%s", PSU2_AC_PMBUS_PREFIX, node);
    }

    ret = deviceNodeReadString(node_path, buf, sizeof(buf), 0);

    if (ret == 0) {
        *value = atoi(buf);
    }

    return ret;
}

int
onlp_psui_init(void)
{
    return ONLP_STATUS_OK;
}

static int
psu_ym2651y_info_get(onlp_psu_info_t* info)
{
    int val   = 0;
    int index = ONLP_OID_ID_GET(info->hdr.id);
    
    /* Set capability
     */
    info->caps = ONLP_PSU_CAPS_AC;
    
	if (info->status & ONLP_PSU_STATUS_FAILED) {
	    return ONLP_STATUS_OK;
	}

    /* Set the associated oid_table */
    info->hdr.coids[0] = ONLP_FAN_ID_CREATE(index + CHASSIS_FAN_COUNT);
    info->hdr.coids[1] = ONLP_THERMAL_ID_CREATE(index + CHASSIS_THERMAL_COUNT);

    /* Read voltage, current and power */
    if (psu_ym2651y_pmbus_info_get(index, "psu_v_out", &val) == 0) {
        info->mvout = val;
        info->caps |= ONLP_PSU_CAPS_VOUT;
    }

	if (psu_ym2651y_pmbus_info_get(index, "psu_v_in", &val) == 0) {
        info->mvin = val;
        info->caps |= ONLP_PSU_CAPS_VIN;
    }

    if (psu_ym2651y_pmbus_info_get(index, "psu_i_out", &val) == 0) {
        info->miout = val;
        info->caps |= ONLP_PSU_CAPS_IOUT;
    }

	if (psu_ym2651y_pmbus_info_get(index, "psu_i_in", &val) == 0) {
        info->miin = val;
        info->caps |= ONLP_PSU_CAPS_IIN;
    }

    if (psu_ym2651y_pmbus_info_get(index, "psu_p_out", &val) == 0) {
        info->mpout = val;
        info->caps |= ONLP_PSU_CAPS_POUT;
    }

	if (psu_ym2651y_pmbus_info_get(index, "psu_p_in", &val) == 0) {
        info->mpin = val;
        info->caps |= ONLP_PSU_CAPS_PIN;
    }

    return ONLP_STATUS_OK;
}

/*
 * Get all information about the given PSU oid.
 */
static onlp_psu_info_t pinfo[] =
{
    { }, /* Not used */
    {
        { ONLP_PSU_ID_CREATE(PSU1_ID), "PSU-1", 0 },
    },
    {
        { ONLP_PSU_ID_CREATE(PSU2_ID), "PSU-2", 0 },
    }
};

int
onlp_psui_info_get(onlp_oid_t id, onlp_psu_info_t* info)
{
    int val   = 0;
    int ret   = ONLP_STATUS_OK;
    int index = ONLP_OID_ID_GET(id);

    VALIDATE(id);

    memset(info, 0, sizeof(onlp_psu_info_t));
    *info = pinfo[index]; /* Set the onlp_oid_hdr_t */

    /* Get the present state */
    if (psu_status_info_get(index, "psu_temp1_input", &val) != 0) {
        printf("Unable to read PSU(%d) node(psu_present)\r\n", index);
    }

    if (val <= 0) {
        info->status &= ~ONLP_PSU_STATUS_PRESENT;
        return ONLP_STATUS_OK;
    }
    info->status |= ONLP_PSU_STATUS_PRESENT;


    /* Get power good status */

    if (psu_status_info_get(index, "psu_v_out", &val) != 0) {
        printf("Unable to read PSU(%d) node(psu_power_good)\r\n", index);
    }
    
    if (val <= 0) {
        info->status |=  ONLP_PSU_STATUS_FAILED;
    }else{
        info->mvout = val;
    }


    /* Get PSU type
     */
	get_psu_type(index, info->model, sizeof(info->model));
	ret = psu_ym2651y_info_get(info);

    return ret;
}

int
onlp_psui_ioctl(onlp_oid_t pid, va_list vargs)
{
    return ONLP_STATUS_E_UNSUPPORTED;
}

