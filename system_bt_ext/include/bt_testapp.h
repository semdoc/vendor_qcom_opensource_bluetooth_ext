/*
 * Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *        * Redistributions in binary form must reproduce the above copyright
 *            notice, this list of conditions and the following disclaimer in the
 *            documentation and/or other materials provided with the distribution.
 *        * Neither the name of The Linux Foundation nor
 *            the names of its contributors may be used to endorse or promote
 *            products derived from this software without specific prior written
 *            permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.    IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef TEST_APP_INTERFACE
#ifndef ANDROID_INCLUDE_BT_TESTAPP_H
#define ANDROID_INCLUDE_BT_TESTAPP_H
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <linux/capability.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <private/android_filesystem_config.h>
#include <android/log.h>
#include <hardware/bluetooth.h>
#include "l2c_api.h"
#include "sdp_api.h"
#include "gatt_api.h"
#include "gap_api.h"
#include "mca_api.h"
#include <hardware/hardware.h>
#include "btm_api.h"

__BEGIN_DECLS

typedef void (tREMOTE_DEVICE_NAME_CB) (void *p1);

enum {
    SUCCESS,
    FAIL
};

typedef enum {
    DUMMY,
    ALL,
    SPP,
    FTP,
    OPP,
    MAP,
    PBAP,
    DUN,
    NOT_SUPPORTED,
}profileName;
typedef enum {
    TEST_APP_L2CAP,
    TEST_APP_RFCOMM,
    TEST_APP_MCAP,
    TEST_APP_GATT,
    TEST_APP_GAP,
    TEST_APP_SMP
} test_app_profile;
typedef struct {

    /** set to sizeof(Btl2capInterface) */
    size_t          size;
    /** Register the L2cap callbacks  */
    bt_status_t (*Init)(tL2CAP_APPL_INFO* callbacks);
    bt_status_t (*RegisterPsm)(uint16_t psm, bool conn_type, uint16_t sec_level);
    bt_status_t (*Deregister)(uint16_t psm);
    uint16_t    (*AllocatePsm)(void);
    uint16_t    (*Connect)(uint16_t psm, RawAddress *bd_addr);
    bool        (*ConnectRsp)(RawAddress p_bd_addr, uint8_t id, uint16_t lcid, uint16_t result, uint16_t status);
    uint16_t    (*ErtmConnectReq)(uint16_t psm, RawAddress p_bd_addr, tL2CAP_ERTM_INFO *p_ertm_info);
    bool        (*ErtmConnectRsp)(RawAddress p_bd_addr, uint8_t id, uint16_t lcid,
                                                uint16_t result, uint16_t status,
                                                tL2CAP_ERTM_INFO *p_ertm_info);
    bool        (*ConfigReq)(uint16_t cid, tL2CAP_CFG_INFO *p_cfg);
    bool        (*ConfigRsp)(uint16_t cid, tL2CAP_CFG_INFO *p_cfg);
    bool        (*DisconnectReq)(uint16_t cid);
    bool        (*DisconnectRsp)(uint16_t cid);
    uint8_t     (*DataWrite)(uint16_t cid, char *p_data, uint32_t len);
    bool        (*Ping)(RawAddress p_bd_addr, tL2CA_ECHO_RSP_CB *p_cb);
    bool        (*Echo)(RawAddress p_bd_addr, BT_HDR *p_data, tL2CA_ECHO_DATA_CB *p_callback);
    bool        (*SetIdleTimeout)(uint16_t cid, uint16_t timeout, bool is_global);
    bool        (*SetIdleTimeoutByBdAddr)(RawAddress bd_addr, uint16_t timeout);
    uint8_t     (*SetDesireRole)(uint8_t new_role);
	void        (*SetSecConnOnlyMode)(bool secvalue);
    uint16_t    (*LocalLoopbackReq)(uint16_t psm, uint16_t handle, RawAddress p_bd_addr);
    uint16_t    (*FlushChannel)(uint16_t lcid, uint16_t num_to_flush);
    bool        (*SetAclPriority)(RawAddress bd_addr, uint8_t priority);
    bool        (*FlowControl)(uint16_t cid, bool data_enabled);
    bool        (*SendTestSFrame)(uint16_t cid, bool rr_or_rej, uint8_t back_track);
    bool        (*SetTxPriority)(uint16_t cid, tL2CAP_CHNL_PRIORITY priority);
    bool        (*RegForNoCPEvt)(tL2CA_NOCP_CB *p_cb, RawAddress p_bda);
    bool        (*SetChnlDataRate)(uint16_t cid, tL2CAP_CHNL_DATA_RATE tx, tL2CAP_CHNL_DATA_RATE rx);
    bool        (*SetFlushTimeout)(RawAddress bd_addr, uint16_t flush_tout);
    uint8_t     (*DataWriteEx)(uint16_t cid, BT_HDR *p_data, uint16_t flags);
    bool        (*SetChnlFlushability)(uint16_t cid, bool is_flushable);
    bool        (*GetPeerFeatures)(RawAddress bd_addr, uint32_t *p_ext_feat, uint8_t *p_chnl_mask);
    bool        (*GetBDAddrbyHandle)(uint16_t handle, RawAddress bd_addr);
    uint8_t     (*GetChnlFcrMode)(uint16_t lcid);
    uint16_t    (*SendFixedChnlData)(uint16_t fixed_cid, RawAddress rem_bda, BT_HDR *p_buf);
    void        (*Cleanup)(void);
    bt_status_t (*RegisterLePsm) (uint16_t le_psm, bool ConnType, uint16_t SecLevel,
                                    uint8_t enc_key_size);
    bt_status_t (*LeDeregister)(uint16_t psm);
    uint16_t    (*LeConnect) (uint16_t le_psm , RawAddress address, tL2CAP_LE_CFG_INFO *p_cfg);
    bool        (*LeConnectRsp) (RawAddress p_bd_addr, uint8_t id, uint16_t lcid, uint16_t result,
                             uint16_t status, tL2CAP_LE_CFG_INFO *p_cfg);
    bool        (*LeFlowControl) (uint16_t lcid, uint16_t credits);
    void        (*LeFreeBuf)(BT_HDR *p_buf);
} btl2cap_interface_t;

typedef struct
{
    size_t    size;
    void (*Init)(void);
    tMCA_HANDLE (*Register)(tMCA_REG *p_reg, tMCA_CTRL_CBACK *p_cback);
    void        (*Deregister)(tMCA_HANDLE handle);
    tMCA_RESULT (*CreateDep)(tMCA_HANDLE handle, tMCA_DEP *p_dep, tMCA_CS *p_cs);
    tMCA_RESULT (*DeleteDep)(tMCA_HANDLE handle, tMCA_DEP dep);
    tMCA_RESULT (*ConnectReq)(tMCA_HANDLE handle, RawAddress bd_addr,
                                          uint16_t ctrl_psm,
                                          uint16_t sec_mask);
    tMCA_RESULT (*DisconnectReq)(tMCA_CL mcl);
    tMCA_RESULT (*CreateMdl)(tMCA_CL mcl, tMCA_DEP dep, uint16_t data_psm,
                                         uint16_t mdl_id, uint8_t peer_dep_id,
                                         uint8_t cfg, const tMCA_CHNL_CFG *p_chnl_cfg);
    tMCA_RESULT (*CreateMdlRsp)(tMCA_CL mcl, tMCA_DEP dep,
                                            uint16_t mdl_id, uint8_t cfg, uint8_t rsp_code,
                                            const tMCA_CHNL_CFG *p_chnl_cfg);
    tMCA_RESULT (*CloseReq)(tMCA_DL mdl);
    tMCA_RESULT (*ReconnectMdl)(tMCA_CL mcl, tMCA_DEP dep, uint16_t data_psm,
                                            uint16_t mdl_id, const tMCA_CHNL_CFG *p_chnl_cfg);
    tMCA_RESULT (*ReconnectMdlRsp)(tMCA_CL mcl, tMCA_DEP dep,
                                               uint16_t mdl_id, uint8_t rsp_code,
                                               const tMCA_CHNL_CFG *p_chnl_cfg);
    tMCA_RESULT (*DataChnlCfg)(tMCA_CL mcl, const tMCA_CHNL_CFG *p_chnl_cfg);
    tMCA_RESULT (*Abort)(tMCA_CL mcl);
    tMCA_RESULT (*Delete)(tMCA_CL mcl, uint16_t mdl_id);
    tMCA_RESULT (*WriteReq)(tMCA_DL mdl, BT_HDR *p_pkt);
    uint16_t (*GetL2CapChannel) (tMCA_DL mdl);
}btmcap_interface_t;

typedef struct
{
    size_t    size;
    //GATT common APIs (Both client and server)
    tGATT_IF (*Register) (tBT_UUID *p_app_uuid128, tGATT_CBACK *p_cb_info);
    void (*Deregister) (tGATT_IF gatt_if);
    void (*StartIf) (tGATT_IF gatt_if);
    bool (*Connect) (tGATT_IF gatt_if, RawAddress bd_addr, bool is_direct,tBT_TRANSPORT transport);
    tGATT_STATUS (*Disconnect) (uint16_t conn_id);
    bool (*Listen) (tGATT_IF gatt_if, bool start, RawAddress& bd_addr);

    //GATT Client APIs
    tGATT_STATUS (*cConfigureMTU) (uint16_t conn_id, uint16_t  mtu);
    tGATT_STATUS (*cDiscover) (uint16_t conn_id, tGATT_DISC_TYPE disc_type, tGATT_DISC_PARAM *p_param );
    tGATT_STATUS (*cRead) (uint16_t conn_id, tGATT_READ_TYPE type, tGATT_READ_PARAM *p_read);
    tGATT_STATUS (*cWrite) (uint16_t conn_id, tGATT_WRITE_TYPE type, tGATT_VALUE *p_write);
    tGATT_STATUS (*cExecuteWrite) (uint16_t conn_id, bool is_execute);
    tGATT_STATUS (*cSendHandleValueConfirm) (uint16_t conn_id, uint16_t handle);
    void (*cSetIdleTimeout)(RawAddress bd_addr, uint16_t idle_tout);
    void (*cSetVisibility) (uint16_t disc_mode, uint16_t conn_mode);

    //GATT Server APIs
    //TODO - Add api on the need basis

}btgatt_test_interface_t;

typedef struct
{
    size_t    size;
    void (*init)(void);
    bool (*Register) (tSMP_CALLBACK *p_cback);
    tSMP_STATUS (*Pair) (RawAddress bd_addr);
    bool (*PairCancel) (RawAddress bd_addr);
    void (*SecurityGrant)(RawAddress bd_addr, uint8_t res);
    void (*PasskeyReply) (RawAddress bd_addr, uint8_t res, uint32_t passkey);
    bool (*Encrypt) (uint8_t *key, uint8_t key_len, uint8_t *plain_text, uint8_t pt_len, tSMP_ENC *p_out);
}btsmp_interface_t;
typedef struct
{
    size_t    size;
    void (*Gap_AttrInit)();
    void (*Gap_BleAttrDBUpdate)(RawAddress bd_addr, uint16_t int_min, uint16_t int_max, uint16_t latency, uint16_t sp_tout);
}btgap_interface_t;

/** Bluetooth RFC tool commands */
typedef enum {
    RFC_TEST_CLIENT =1,
    RFC_TEST_FRAME_ERROR,
    RFC_TEST_ROLE_SWITCH,
    RFC_TEST_SERVER,
    RFC_TEST_DISCON,
    RFC_TEST_CLIENT_TEST_MSC_DATA, //For PTS test case BV 21 and 22
    RFC_TEST_WRITE_DATA
}rfc_test_cmd_t;


typedef struct {
    RawAddress bdadd;
    uint8_t     scn; //Server Channel Number
}bt_rfc_conn_t;

typedef struct {
    RawAddress bdadd;
    uint8_t     role; //0x01 for master
}bt_role_sw;

typedef union {
    bt_rfc_conn_t  conn;
    uint8_t        server;
    bt_role_sw     role_switch;
}tRfcomm_test;

typedef struct {
    rfc_test_cmd_t param;
    tRfcomm_test   data;
}tRFC;

typedef struct {
    size_t          size;
    bt_status_t (*init)( tL2CAP_APPL_INFO* callbacks );
    void  (*rdut_rfcomm)( uint8_t server );
    void  (*rdut_rfcomm_test_interface)( tRFC *input);
    bt_status_t (*connect)( RawAddress *bd_addr );
    void  (*cleanup)( void );
} btrfcomm_interface_t;

#endif

__END_DECLS

#endif /* ANDROID_INCLUDE_BT_TESTAPP_H */
