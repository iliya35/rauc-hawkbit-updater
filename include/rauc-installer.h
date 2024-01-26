/**
 * SPDX-License-Identifier: LGPL-2.1-only
 * SPDX-FileCopyrightText: 2018-2020 Lasse K. Mikkelsen <lkmi@prevas.dk>, Prevas A/S (www.prevas.com)
 */

#ifndef __RAUC_INSTALLER_H__
#define __RAUC_INSTALLER_H__

#include <glib.h>

enum BundleType {
        BUNDLE_TYPE_OS,
        BUNDLE_TYPE_APP
};

/**
 * @brief struct that contains the context of an Rauc installation.
 */
struct install_context {
        gchar *bundle;                /**< Rauc bundle file to install */
        enum BundleType bundle_type;      /**< type of the software bundle: APP or OS */                
        gchar *auth_header;           /**< Authentication header for bundle streaming */
        gboolean ssl_verify;          /**< Whether to ignore server cert verification errors */
        GSourceFunc notify_event;     /**< Callback function */
        GSourceFunc notify_complete;  /**< Callback function */
        GMutex status_mutex;          /**< Mutex used for accessing status_messages */
        GQueue status_messages;       /**< Queue of status messages from Rauc DBUS */
        gint status_result;           /**< The result of the installation */
        GMainLoop *mainloop;          /**< The installation GMainLoop  */
        GMainContext *loop_context;   /**< GMainContext for the GMainLoop */
        gboolean keep_install_context; /**< Whether the installation thread should free this struct or keep it */
};

/** @brief Converts the string representation of the update package type to the 
  * corresponding BundleType enumeration value.
  *
  * @param[in] part A string representing the update package type. Expected values: "os" or "bApp"
  *
  * @return BundleType The enum value corresponding to the passed string. 
  *         Returns BUNDLE_TYPE_APP by default if the string does not match the expected values.
  */
enum BundleType get_bundle_type(const gchar *part);


/**
 * @brief RAUC install bundle
 *
 * @param[in] bundle RAUC bundle file (.raucb) to install.
 * @param[in] auth_header Authentication header on HTTP streaming installation or NULL on normal
 *                        installation.
 * @param[in] ssl_verify Whether to ignore server cert verification errors.
 * @param[in] on_install_notify Callback function to be called with status info during
 *                              installation.
 * @param[in] on_install_complete Callback function to be called with the result of the
 *                                installation.
 * @param[in] wait Whether to wait until install thread finished or not.
 * @return for wait=TRUE, TRUE if installation succeeded, FALSE otherwise; for
 *         wait=FALSE TRUE is always returned immediately
 */
gboolean rauc_install(const gchar *bundle, const gchar *sw_type, const gchar *auth_header, gboolean ssl_verify,
                GSourceFunc on_install_notify, GSourceFunc on_install_complete, gboolean wait);

#endif // __RAUC_INSTALLER_H__
