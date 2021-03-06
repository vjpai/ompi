/*
 * Copyright (c) 2015      Intel, Inc. All rights reserved.
 * Copyright (c) 2015-2017 Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * Copyright (c) 2015      Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include "opal_config.h"
#include "opal/constants.h"

#include <string.h>
#include <munge.h>

#include "opal_stdint.h"
#include "opal/dss/dss_types.h"
#include "opal/util/error.h"
#include "opal/util/output.h"
#include "opal/util/show_help.h"

#include "opal/mca/sec/base/base.h"
#include "sec_munge.h"

static int init(void);
static void finalize(void);
static int get_my_cred(opal_process_name_t *my_id,
                       opal_sec_cred_t *cred);
static int authenticate(opal_sec_cred_t *cred);

opal_sec_base_module_t opal_sec_munge_module = {
    init,
    finalize,
    get_my_cred,
    authenticate
};

static opal_sec_cred_t my_cred;
static bool initialized = false;
static bool refresh = false;

static int init(void)
{
    int rc;

    opal_output_verbose(2, opal_sec_base_framework.framework_output,
                        "sec: munge init");

    /* attempt to get a credential as a way of checking that
     * the munge server is available - cache the credential
     * for later use */

    if (EMUNGE_SUCCESS != (rc = munge_encode(&my_cred.credential, NULL, NULL, 0))) {
        opal_output_verbose(2, opal_sec_base_framework.framework_output,
                            "sec: munge failed to create credential: %s",
                            munge_strerror(rc));
        return OPAL_ERR_SERVER_NOT_AVAIL;
    }
    /* include the '\0' termination string character */
    my_cred.size = strlen(my_cred.credential)+1;
    initialized = true;

    return OPAL_SUCCESS;
}

static void finalize(void)
{
    if (initialized && !refresh) {
        free(my_cred.credential);
    }
}

static int get_my_cred(opal_process_name_t *my_id,
                       opal_sec_cred_t *cred)
{
    int rc;

    if (initialized) {
        if (!refresh) {
            refresh = true;
        } else {
            /* get a new credential as munge will not
             * allow us to reuse them */
            if (EMUNGE_SUCCESS != (rc = munge_encode(&my_cred.credential, NULL, NULL, 0))) {
                opal_output_verbose(2, opal_sec_base_framework.framework_output,
                                    "sec: munge failed to create credential: %s",
                                    munge_strerror(rc));
                return OPAL_ERR_SERVER_NOT_AVAIL;
            }
            /* include the '\0' termination string character */
            my_cred.size = strlen(my_cred.credential)+1;
        }
        cred->method = strdup("munge");
        cred->credential = my_cred.credential;
        cred->size = my_cred.size;
    } else {
        rc = OPAL_ERROR;
    }

    return OPAL_SUCCESS;
}

static int authenticate(opal_sec_cred_t *cred)
{
    munge_err_t rc;

    opal_output_verbose(2, opal_sec_base_framework.framework_output,
                        "sec: munge validate_cred %s", cred->credential);

    /* parse the inbound string */
    if (EMUNGE_SUCCESS != (rc = munge_decode(cred->credential, NULL, NULL, NULL, NULL, NULL))) {
        opal_output_verbose(2, opal_sec_base_framework.framework_output,
                            "sec: munge failed to decode credential: %s",
                            munge_strerror(rc));
        return OPAL_ERR_AUTHENTICATION_FAILED;
    }

    opal_output_verbose(2, opal_sec_base_framework.framework_output,
                        "sec: munge credential valid");
    return OPAL_SUCCESS;
}

