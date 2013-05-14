#ifndef CBS_SCSI_H
#define CBS_SCSI_H

#include "cbs_types.h"

#define SCSI_STATUS_GOOD              0x00
#define SCSI_STATUS_CHECK             0x02
#define SCSI_STATUS_BUSY              0x08
#define SCSI_STATUS_INTERMEDIATE      0x10
#define SCSI_STATUS_CONFLICT          0x18
#define SCSI_STATUS_TASK_SET_FULL     0x28
#define SCSI_STATUS_ACA               0x30

#define KEY_NO_SENSE            0x00
#define KEY_RECOVERED_ERROR     0x01
#define KEY_NOT_READY           0x02
#define KEY_MEDIUM_ERROR        0x03
#define KEY_HARDWARE_ERROR      0x04
#define KEY_ILLEGAL_REQUEST     0x05
#define KEY_UNIT_ATTENTION      0x06
#define KEY_DATA_PROTECT        0x07
#define KEY_FIRMWARE_ERROR      0x09
#define KEY_ABORTED_COMMAND     0x0B
#define KEY_EQUAL               0x0C
#define KEY_VOLUME_OVERFLOW     0x0D
#define KEY_MISCOMPARE          0x0E

/*
 * packed Additional sense code (ms byte) and 
 * Additional sense code qualifier (ls byte) 
 * (ASC == additional sense code)
 */
#define ASC_NO_ADDT_SENSE_INFO                  0x0000
#define ASC_NO_IDX_SECTOR_SIGNAL                0x0100
#define ASC_NO_SEEK_COMPLETE                    0x0200
#define ASC_PERIPHERAL_DEVICE_WR_FAULT          0x0300

#define ASC_LUN_NR_NOT_REPORTABLE               0x0400
#define ASC_LUN_NR_BECOMING_RDY                 0x0401
#define ASC_LUN_NR_START_UNIT_REQ               0x0402
#define ASC_LUN_NR_MANUAL_INTERVENTION_REQ      0x0403
#define ASC_LUN_NR_FMT_IN_PROGRESS              0x0404
#define ASC_LUN_NR_OPERATION_IN_PROGRESS        0x0407
#define ASC_LUN_NA_TP_IN_TRANSIT                0x040A
#define ASC_LUN_NA_TP_IN_STANDBY                0x040B
#define ASC_LUN_NA_TP_IN_UNAVAILABLE            0x040C

#define ASC_DRIVE_COMMS_FAIL                    0x0800
#define ASC_DRIVE_COMMS_TIMEOUT                 0x0801

#define ASC_TRACK_FOLLOWING_ERR                 0x0900
#define ASC_HEAD_SEL_FAULT                      0x0904

#define ASC_ERR_LOG_OVERFLOW                    0x0a00

#define ASC_WR_ERR                              0x0c00
#define ASC_WR_ERR_REALLOCED                    0x0c01
#define ASC_WR_ERR_REALLOC_FAIL                 0x0c02

#define ASC_CRC_OR_ECC_ERR                      0x1000

#define ASC_RD_ERR                              0x1100
#define ASC_RD_ERR_RETRIES_EXHAUSTED            0x1101
#define ASC_RD_ERR_ERROR_TOO_LONG               0x1102
#define ASC_RD_ERR_MULTIPE                      0x1103
#define ASC_RD_ERR_REALLOC_FAIL                 0x1104

#define ASC_ADDR_MARK_NOT_FOUND                 0x1200
#define ASC_RCVRD_DATA_WO_ECC                   0x1201
#define ASC_RCVRD_DATA_W_ECC                    0x1202

#define ASC_RECORD_NOT_FOUND                    0x1401
#define ASC_MECHANICAL_POS_ERR                  0x1501
#define ASC_DATA_SYNC_MISSING_OR_INCORRECT      0x1600

#define ASC_RCVRD_RETRY                         0x1701
#define ASC_RCVRD_POS_OFFSET                    0x1702
#define ASC_RCVRD_NEG_OFFSET                    0x1703
#define ASC_RCVRD_WO_ECC_AUTO_REALLOC           0x1706

#define ASC_RCVRD_W_ECC_WO_RETRY                0x1800
#define ASC_RCVRD_W_ECC_W_RETRY                 0x1801
#define ASC_RCVRD_W_ECC_RETRY_REALLOCED         0x1802
#define ASC_RCVRD_W_ECC_RETRY_REASSIGN          0x1805

#define ASC_DEFECT_LIST_ERR                     0x1900
#define ASC_PARAM_LIST_LEN_ERR                  0x1a00
#define ASC_DEFECT_LIST_NOT_FOUND               0x1c00
#define ASC_MISCOMPARE_DURING_VERIFY            0x1d00

#define ASC_INVALID_CMD_OP_CODE                 0x2000
#define ASC_LOGICAL_BLOCK_OUT_OF_RANGE          0x2100
#define ASC_INVALID_FIELD_IN_CDB                0x2400
#define ASC_LUN_UNSUPPORTED                     0x2500

/* invalid field parameter (IFP)  sub defines */
#define ASC_IFP_IN_PARAM_LIST                   0x2600
#define ASC_IFP_PARAM_UNSUPPORTED               0x2601
#define ASC_IFP_PARAM_VAL_INVALID               0x2602
#define ASC_IFP_PARAM_THRESHOLD_UNSUPPORTED     0x2603
#define ASC_INVALD_RELEASE_OF_PR                0x2604
#define ASC_IFP_TMS_FW_TAG                      0x2697
#define ASC_IFP_CSUM                            0x2698
#define ASC_IFP_FW_TAG                          0x2699

#define ASC_WR_PROTECTED                        0x2700
#define ASC_WR_PROTECTED_HARDWARE               0x2701
#define ASC_WR_PROTECTED_SOFTWARE_LUN           0x2702
#define ASC_WR_PROTECTED_ASSOCIATED             0x2703
#define ASC_WR_PROTECTED_PERSISTENT             0x2704
#define ASC_WR_PROTECTED_PERMANENT              0x2705
#define ASC_WR_PROTECTED_CONDITIONAL            0x2706

#define ASC_POWER_ON_RESET                      0x2900
#define ASC_POWER_ON_RESET1                     0x2901
#define ASC_SCSI_BUS_RESET                      0x2902
#define ASC_BUS_DEVICE_RESET                    0x2903
#define ASC_DEVICE_INTERNAL_RESET               0x2904
#define ASC_TRNCVR_CHNGD_TO_SE                  0x2905
#define ASC_TRNCVR_CHNGD_TO_LVD                 0x2906
#define ASC_I_T_NEXUS_LOSS_OCCURRED             0x2907

#define ASC_PARAMS_CHANGED                      0x2a00
#define ASC_MODE_PARAMS_CHANGED                 0x2a01
#define ASC_LOG_PARAMS_CHANGED                  0x2a02
#define ASC_RESERVATIONS_PREEMPTED              0x2a03
#define ASC_RESERVATIONS_RELEASED               0x2a04
#define ASC_REGISTRATIONS_PREEMPTED             0x2a05

#define ASC_CMD_SEQ_ERR                         0x2c00
#define ASC_TAG_CMD_CLRD_BY_OTHER_INIT          0x2f00

#define ASC_MEDIUM_FMT_CORRUPTED                0x3100
#define ASC_FMT_FAIL                            0x3101

#define ASC_NO_DEFECT_SPARE_LOCATION_AVAIL      0x3200
#define ASC_DEFECT_LIST_UPDATE_ERR              0x3201
#define ASC_TOO_MANY_DEFECTS_ON_ONE_TRACK       0x3202

#define ASC_ENC_SERVICES_FAILURE                0x3500
#define ASC_ENC_SERVICES_UNSUPPORTED_FUNCTION   0x3501
#define ASC_ENC_SERVICES_UNAVAILABLE            0x3502
#define ASC_ENC_SERVICES_TRANSFER_FAILURE       0x3503
#define ASC_ENC_SERVICES_TRANSFER_REFUSED       0x3504

#define ASC_PARAM_ROUNDED                       0x3700

#define ASC_MICROCODE_CHANGED                   0x3f01
#define ASC_CHANGED_OPERATING_DEFINITION        0x3f02
#define ASC_INQUIRY_PARAM_CHANGED               0x3f03

/* 40NN Diagnostic error on component where NN = 80-ff component */
#define ASC_DRAM_PARITY_ERR                     0x4001
#define ASC_CHECKSUM_ERROR_ON_DOWNLOAD          0x4085
 
#define ASC_POWER_ON_OR_SELF_TEST_FAIL          0x4200
 
#define ASC_MSG_RJT_ERR                         0x4300
 
#define ASC_INTERNAL_TARGET_ERR                 0x4400
 
#define ASC_SEL_OR_RESEL_FAIL                   0x4500

#define ASC_SCSI_BUS_PARITY_ERR                 0x4700
#define ASC_SCSI_DATA_PHASE_CRC_ERR             0x4701 
#define ASC_SCSI_ST_DATA_PHASE_CRC_ERR          0x4702
#define ASC_SCSI_IU_PHASE_CRC_ERR               0x4703

#define ASC_INITIATOR_DETECTED_ERR              0x4800
 
#define ASC_INVALID_MSG                         0x4900
 
#define ASC_CMD_PHASE_ERROR                     0x4A00
 
#define ASC_DATA_PHASE_ERROR                    0x4b00
#define ASC_ILLEGAL_TARGET_TAG                  0x4b01
#define ASC_TOO_MUCH_WRITE_DATA                 0x4b02
#define ASC_ACK_NAK_TIMEOUT                     0x4b03
#define ASC_NAK_RECEIVED                        0x4b04
#define ASC_DATA_OFFSET_ERROR                   0x4b05
#define ASC_INIT_RESPONSE_TIMEOUT               0x4b06
#define ASC_OVERLAPPED_CMDS_ATTEMPTED           0x4e00
#define ASC_INSUFFICIENT_RESERVATION_RESOURCES  0x5502
#define ASC_INSUFFICIENT_RESOURCES              0x5503
#define ASC_INSUFFICIENT_REGISTRATION_RESOURCES 0x5504


#define ASC_LOG_EXCEPTION                       0x5b00
#define ASC_THRESHOLD_CONDITION_MET             0x5b01
#define ASC_LOG_PARAM_VAL_AT_MAXIMUM            0x5b02

#define ASC_SERVO_RPL_STATUS_CHANGE             0x5c00
#define ASC_SERVO_RPL_SPINDLES_SYNC             0x5c01
#define ASC_SERVO_RPL_SPINDLES_NOT_SYNC         0x5c02
 
#define ASC_FAIL_PREDICT_THRESHOLD_EXCEEDED     0x5d00
#define ASC_PRD_FAIL_HW_GENERAL                 0x5d10
#define ASC_PRD_FAIL_HW_DRIVE_ERR_RATE          0x5d11
#define ASC_PRD_FAIL_HW_DATA_ERR_RATE           0x5d12
#define ASC_PRD_FAIL_HW_TOO_MANY_BLOCK_REASSIGN 0x5d14
#define ASC_PRD_FAIL_HW_ACCESS_TIME_TOO_HIGH    0x5d15
#define ASC_PRD_FAIL_HW_START_TIME_TOO_HIGH     0x5d16
#define ASC_PRD_FAIL_CTRL_GENERAL               0x5d20
#define ASC_PRD_FAIL_DATAC_GENERAL              0x5d30
#define ASC_PRD_FAIL_SERVO_GENERAL              0x5d40
#define ASC_PRD_FAIL_SPINDLE_GENERAL            0x5d50
#define ASC_PRD_FAIL_FIRMWARE_GENERAL           0x5d60

#define ASC_IDLE_CONDITION_BY_COMMAND           0x5e03
#define ASC_IDLE_CONDITION_BY_TIMER             0x5e01
#define ASC_STANDBY_CONDITION_BY_COMMAND        0x5d04
#define ASC_STANDBY_CONDITION_BY_TIMER          0x5d02

#define ASC_VOLTAGE_FAULT                       0x6500

#define ASC_GENERAL_FW_ERR_QUALIFIER            0x8000
#define ASC_FC_FIFO_ERR_DURING_RD_TXFER         0x8080
#define ASC_FC_FIFO_ERR_DURING_WR_TXFER         0x8081
#define ASC_FC_IO_EDC_ERROR                     0x8086
#define ASC_REASSIGN_POWER_FAIL_RECOVERY_FAIL   0x8100

#define ASC_RT_INTERNAL_ASCQ_START              0x9100
#define ASC_RT_INTERNAL_REBUILD_STRIPE_FAILED   0x9100
#define ASC_RT_INTERNAL_CMD_FAILED              0x9101
#define ASC_RT_INTERNAL_XFER_FAILED             0x9102
#define ASC_RT_INTERNAL_RESOURCE_FAILED         0x9103
#define ASC_RT_INTERNAL_WRONG_PARAM             0x9104
#define ASC_RT_INTERNAL_LINK_DOWN               0x9105
#define ASC_RT_INTERNAL_DEVICE_GONE             0x9106   


/*
 * These are encodings of SCSI status, Key, Additional Sense Code and Qualifier
 * created to make error recovery more manageable.
 */
enum sense_contexts{ 
    SENSE_CTX_NONE,
    SENSE_CTX_TASK_FULL,
    SENSE_CTX_ILLEGAL_CDB,
    SENSE_CTX_NOT_READY,
    SENSE_CTX_HARDWARE_ERR,
    SENSE_CTX_WRITE_PROTECT,
    SENSE_CTX_LUN_UNSUPPORTED,
    SENSE_CTX_RESERVE_CONF,
    SENSE_CTX_RESET_POWER_ON,
    SENSE_CTX_RESET_DIRECTED,
    SENSE_CTX_MEDIUM_ERR_RD,
    SENSE_CTX_MEDIUM_ERR_WR,
    SENSE_CTX_MEDIUM_ERR_AMNF,
    SENSE_CTX_TASK_SET_CLEARED,
    SENSE_CTX_VOL_OVERFLOW,
    SENSE_CTX_FC_ERR,
    SENSE_CTX_SCSI_PARITY_ERR,  
    SENSE_CTX_SCSI_DATA_ERR, 
    SENSE_CTX_SCSI_ST_ERR, 
    SENSE_CTX_SCSI_IU_ERR, 
    SENSE_CTX_MSG_RJT_ERR,
    SENSE_CTX_INITIATOR_ERR, 
    SENSE_CTX_RESERVES_PREEMPTED,
    SENSE_CTX_DOWNLOAD_FAILED,
    /* START: for persistent reserve */
    SENSE_CTX_INSUFFICIENT_REG_RESOURCES,
    SENSE_CTX_PARAMETER_LIST_LEN_ERR,
    SENSE_CTX_INVALID_RELEASE_OF_PR,
    SENSE_CTX_IFP_IN_PARAM_LIST, 
    SENSE_CTX_TP_IN_TRANSIT,
    SENSE_CTX_TP_IN_STANDY,
    SENSE_CTX_TP_IN_UNAVAILABLE,
    /* END: for persistent reserve */
    SENSE_CTX_INTERNAL_REBUILD_FAILED, // start of the internal sense
    SENSE_CTX_INTERNAL_CMD_FAILED,
    SENSE_CTX_INTERNAL_XFER_FAILED,
    SENSE_CTX_INTERNAL_RESOURCE_FAILED,
    SENSE_CTX_INTERNAL_WRONG_PARAM,
    SENSE_CTX_INTERNAL_LINK_DOWN,
    SENSE_CTX_INTERNAL_DEVICE_GONE,         
    nb_sense_contexts
};

/* this is the expanded scsi sense info structure.Normally the 
 * size of scsi sense info is 18 bytes. In order to cope with more
 * info and comply with the size of qlogic status iocb scsi sense field,
 * we expand the structure.
 */
typedef struct scsi_sense_info {
    uint8 error_code: 7;            /* error code */
    uint8 valid: 1;                 /* indicates (among other things) that the
                                     * info field contains valid information */

    uint8 segment;                  /* reserved (set to 0) */

    uint8 key: 4;                   /* general error category */
    uint8 rsv1: 1;
    uint8 ili: 1;                   /* (depending on other things) incorrect
                                     * length indcator (read and write long
                                     * only) */
    uint8 rsv2: 2;

    uint8 info[4];                   /* info field contains valid information */

    uint8 addt_len;                 /* additional sense length */

    uint32 csi;                     /* command specific information csi[0] ==
                                     * MSB */

    uint8 addt_sc;                  /* additional sense code */
    uint8 addt_sc_qualifier;        /* additional sense code qualifier */
    uint8 rsv3;
    
    uint8 sks0: 7;                  /* sense key specific */
    uint8 sksv: 1;                  /* sense key specific valid */
    uint8 sks1;                     /* sense key specific */
    uint8 sks2;                     /* sense key specific */

    /* expandtion */
    uint8 uniq_sense_data0;         /* product unique sense data */
    uint8 uniq_sense_data1;         /* product unique sense data */
    uint8 rsvds[12];                /* sense data in status iocb is 32byte long */
} scsi_sense_info_t;

#define SCSI_SENSE_LEN  18


#endif
