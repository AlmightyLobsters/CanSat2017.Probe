/**
 *---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : Bluetooth Serial
 * File        : cb_bt_serial.h
 *
 * Description : Data management for RFCOMM based profiles such and Serial 
 *               Port Profile (SPP).
 *
 *-------------------------------------------------------------------------*/

/**
 * @file cb_bt_serial.h
 * @brief Data management for RFCOMM based profiles such and Serial 
 * Port Profile (SPP).
 */

#ifndef _CB_BT_SERIAL_H_
#define _CB_BT_SERIAL_H_

#include "cb_comdefs.h"
#include "bt_types.h"
#include "cb_bt_conn_man.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
* DEFINES
*=========================================================================*/
#define cbBSE_OK                       0
#define cbBSE_ERROR                   -1
#define cbBSE_NO_DATA                 -2

/*===========================================================================
* TYPES
*=========================================================================*/

typedef void (*cbBSE_DataAvailEvt)(
    cbBCM_Handle handle);

typedef void (*cbBSE_WriteCnf)(
    cbBCM_Handle handle,
    cb_int32 status,
    cb_uint32 nBytes,
    cb_int32 tag);

typedef struct
{
    cbBSE_DataAvailEvt        pfDataEvt;
    cbBSE_WriteCnf            pfWriteCnf;
}cbBSE_Callback;

/*===========================================================================
* FUNCTIONS
*=========================================================================*/
/**
 * Initialization of Bluetooth serial manager. Called during stack
 * initialization. Shall not be called by application.
 * 
 * @return None
 */
extern void cbBSE_init(void);

/**
 * Open a data channel.
 * 
 * @param   handle      Connection handle
 * @param   pCallback   Callback for data events.
 * @return   If the operation is successful cbBSE_OK is returned.
 */
extern cb_int32 cbBSE_open(
    cbBCM_Handle handle,
    cbBSE_Callback *pCallback);

/**
 * Send data to remote device. A data confirmation event is generated when
 * the data has been sent and a cbBSE_write call can be done.
 * 
 * Detailed description optionally verbose.
 * @param   handle  Connection handle
 * @param   pBuf    Data pointer
 * @param   nBytes  nBytes Size of data to be sent.
 * @param   tag     Tag passed as argument in corresponding data confirmation callback.
 * @return  If the operation is successful cbBSE_OK is returned.
 */
extern cb_int32 cbBSE_write(
    cbBCM_Handle handle,
    cb_uint8 *pBuf,
    cb_uint32 nBytes,
    cb_int32 tag);

/**
 * Get received data.
 * 
 * @param handle Connection handle
 * @param   ppBuf    Pointer to data buffer
 * @param   pLength  Pointer to buffer length variable.
 * @return  cbBSE_OK is returned if data is available. If no data is available
 *          then cbBSE_NO_DATA is returned.
 */
extern cb_int32 cbBSE_getReadBuf(
    cbBCM_Handle handle,
    cb_uint8 **ppBuf,
    cb_uint32 *pLength);

/**
 * Notify that received data has been handled and underlying buffers 
 * can be freed.
 *
 * @param handle Connection handle
 * @param nBytes Number of bytes consumed.
 * @return If the operation is successful cbBSE_OK is returned.
 */
extern cb_int32 cbBSE_readBufConsumed(
    cbBCM_Handle handle,
    cb_uint32 nBytes);

/**
 * Read max frame size for a data channel.
 * 
 * @param   handle      Connection handle
 * @param   pFrameSize  Max frame size for connection.
 * @return  If the operation is successful cbBSE_OK is returned.
 */
extern cb_int32 cbBSE_frameSize(cbBCM_Handle handle, cb_uint32 *pFrameSize);

/**
 * Bluetooth serial message handling. Shall not be called by application.
 * 
 * @param msgId Message id
 * @param pData Pointer to message data
 * @return None
 */
extern void cbBSE_handleMsg(cb_uint32 msgId, void* pData);

#ifdef __cplusplus
}
#endif

#endif
