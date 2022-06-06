/*
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    servo.c
 *
 * Purpose: 
 *
 * Functions:
 *      bcm_common_ptp_servo_configuration_get
 *      bcm_common_ptp_servo_configuration_set
 *      bcm_common_ptp_servo_status_get
 *      bcm_common_ptp_phase_offset_set
 *      bcm_common_ptp_phase_offset_get
 *
 *      _bcm_ptp_servo_start
 *      _bcm_ptp_servo_restart
 *      _bcm_ptp_servo_stop
 *      _bcm_ptp_servo_ipdv_configuration_get
 *      _bcm_ptp_servo_ipdv_configuration_set
 *      _bcm_ptp_servo_performance_get
 *      _bcm_ptp_servo_ipdv_performance_get
 *      _bcm_ptp_servo_performance_data_clear
 */

#if defined(INCLUDE_PTP)

#include <bcm/ptp.h>
#include <bcm_int/common/ptp.h>
#include <bcm_int/ptp_common.h>
#include <bcm/error.h>

static const bcm_ptp_port_identity_t portid_all = 
    {{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}, PTP_IEEE1588_ALL_PORTS};

/*
 * Function:
 *      _bcm_ptp_servo_start
 * Purpose:
 *      Start PTP time synchronization servo.
 * Parameters:
 *      unit      - (IN) Unit number.
 *      ptp_id    - (IN) PTP stack ID.
 *      clock_num - (IN) PTP clock number.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_start(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 payload[PTP_MGMTMSG_PAYLOAD_MIN_PROPRIETARY_MSG_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;
    
    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    /* 
     * Make payload.
     *    Octet 0...5 : Custom management message key/identifier. 
     *                  BCM<null><null><null>.
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    
    rv = _bcm_ptp_management_message_send(unit, ptp_id, clock_num, 
            &portid, PTP_MGMTMSG_CMD, PTP_MGMTMSG_ID_SERVO_START, 
            payload, PTP_MGMTMSG_PAYLOAD_MIN_PROPRIETARY_MSG_SIZE_OCTETS, 
            resp, &resp_len);
    
    return rv;
}

/*
 * Function:
 *      _bcm_ptp_servo_restart
 * Purpose:
 *      Restart PTP time synchronization servo.
 * Parameters:
 *      unit      - (IN) Unit number.
 *      ptp_id    - (IN) PTP stack ID.
 *      clock_num - (IN) PTP clock number.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_restart(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num)
{   
    /* Note: this is currently a No-op in the servo */
    int rv = BCM_E_UNAVAIL;
    
    uint8 payload[PTP_MGMTMSG_PAYLOAD_MIN_PROPRIETARY_MSG_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;
    
    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    /* 
     * Make payload.
     *    Octet 0...5 : Custom management message key/identifier. 
     *                  BCM<null><null><null>.
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    
    rv = _bcm_ptp_management_message_send(unit, ptp_id, clock_num, 
            &portid, PTP_MGMTMSG_CMD, PTP_MGMTMSG_ID_SERVO_RESTART, 
            payload, PTP_MGMTMSG_PAYLOAD_MIN_PROPRIETARY_MSG_SIZE_OCTETS, 
            resp, &resp_len);
    
    return rv;
}

/*
 * Function:
 *      _bcm_ptp_servo_stop
 * Purpose:
 *      Stop PTP time synchronization servo.
 * Parameters:
 *      unit      - (IN) Unit number.
 *      ptp_id    - (IN) PTP stack ID.
 *      clock_num - (IN) PTP clock number.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_stop(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num)
{   

    /* Firmware does not support SERVO STOP. */
    return BCM_E_UNAVAIL;
}

/*
 * Function:
 *      bcm_common_ptp_servo_configuration_get
 * Purpose:
 *      Get PTP servo configuration properties.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      config    - (OUT) PTP servo configuration properties.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
bcm_common_ptp_servo_configuration_get(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    bcm_ptp_servo_config_t *config)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;

    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }
    
    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id, 
            clock_num, &portid, 
            PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_SERVO_CONFIGURATION, 
            0, 0, resp, &resp_len))) {
        return rv;
    }
    
    
    /*
     * Parse response.
     *    Octet 0...5   : Custom management message key/identifier. 
     *                    BCM<null><null><null>.
     *    Octet 6...9   : Oscillator type.    
     *    Octet 10...13 : PTP transport mode.
     *    Octet 14...17 : Phase Mode.
     *    Octet 18...21 : Freq. correction sampled hourly (times 1E+12).
     *    Octet 22...29 : Timestamp of freq. correction measurement (sec).
     *    Octet 30...33 : Timestamp of freq. correction measurement (nsec).
     *    Octet 34...37 : Flags
     *    Octet 38...45 : RFU
     *    Octet 46...49 : RFU
     *    Octet 50...53 : Bridge time.    
     *    Octet 54...57 : Filter Profile
     */
    i = 6;    
    config->osc_type = (bcm_ptp_osc_type_t)_bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32); 
    config->transport_mode = (bcm_ptp_transport_mode_t)_bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);
    config->ptp_phase_mode = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);
    config->freq_corr = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);   
    config->freq_corr_time.seconds = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);   
    config->freq_corr_time.nanoseconds = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);  
    config->flags = _bcm_ptp_uint32_read(resp+i);
    config->servo = (config->flags & 0x0f);
    config->flags >>= 4;
    i += sizeof(uint32);   
    /* RFU */
    i += sizeof(uint64);   
    /* RFU */
    i += sizeof(uint32);   
    config->bridge_time = _bcm_ptp_uint32_read(resp+i);
    if (resp_len >= PTP_MGMTMSG_PAYLOAD_SERVO_CONFIGURATION_SIZE_OCTETS) {
        i += sizeof(uint32);
        config->filter_profile = _bcm_ptp_uint32_read(resp+i);
    } else {
        config->filter_profile = 0;
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_ptp_servo_configuration_set
 * Purpose:
 *      Set PTP servo configuration properties.
 * Parameters:
 *      unit      - (IN) Unit number.
 *      ptp_id    - (IN) PTP stack ID.
 *      clock_num - (IN) PTP clock number.
 *      config    - (IN) PTP servo configuration properties.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
bcm_common_ptp_servo_configuration_set(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    bcm_ptp_servo_config_t *config)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 payload[PTP_MGMTMSG_PAYLOAD_SERVO_CONFIGURATION_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;
    
    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }
    
    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    /*
     * Make payload.
     *    Octet 0...5   : Custom management message key/identifier. 
     *                    BCM<null><null><null>.
     *    Octet 6...9   : Oscillator type.    
     *    Octet 10...13 : PTP transport mode.
     *    Octet 14...17 : Phase Mode.
     *    Octet 18...21 : Freq. correction sampled hourly (times 1E+12).
     *    Octet 22...29 : Timestamp of freq. correction measurement (sec).
     *    Octet 30...33 : Timestamp of freq. correction measurement (nsec).
     *    Octet 34...37 : Freq. calibration from factory (times 1E+12).
     *    Octet 38...45 : Timestamp of freq. calibration at factory (sec).
     *    Octet 46...49 : Timestamp of freq. calibration at factory (nsec).
     *    Octet 50...53 : Bridge time.    
     *    Octet 54...57 : filter profile
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    i += 3;
    _bcm_ptp_uint32_write(payload+i, config->osc_type);
    i += sizeof(uint32);
    _bcm_ptp_uint32_write(payload+i, config->transport_mode);
    i += sizeof(uint32);
    _bcm_ptp_uint32_write(payload+i, config->ptp_phase_mode);
    i += sizeof(uint32);
    _bcm_ptp_uint32_write(payload+i, config->freq_corr);
    i += sizeof(uint32);
    _bcm_ptp_uint64_write(payload+i, config->freq_corr_time.seconds);
    i += sizeof(uint64);
    _bcm_ptp_uint32_write(payload+i, config->freq_corr_time.nanoseconds);
    i += sizeof(uint32);
    _bcm_ptp_uint32_write(payload+i, (config->servo | (config->flags << 4)));
    i += sizeof(uint32);
    /* RFU */
    i += sizeof(uint64);
    /* RFU */
    i += sizeof(uint32);
    _bcm_ptp_uint32_write(payload+i, config->bridge_time);
    i += sizeof(uint32);
    _bcm_ptp_uint32_write(payload+i, config->filter_profile);
    
    rv = _bcm_ptp_management_message_send(unit, ptp_id, clock_num, 
            &portid, PTP_MGMTMSG_SET, PTP_MGMTMSG_ID_SERVO_CONFIGURATION, 
            payload, PTP_MGMTMSG_PAYLOAD_SERVO_CONFIGURATION_SIZE_OCTETS, 
            resp, &resp_len);
    
    return rv;
}

/*
 * Function:
 *      bcm_common_ptp_servo_threshold_get
 * Purpose:
 *      Get PTP servo threshold properties.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      threshold    - (OUT) PTP servo configuration properties.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int
bcm_common_ptp_servo_threshold_get(
    int unit,
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    bcm_ptp_servo_threshold_t *threshold)
{
    int rv = BCM_E_UNAVAIL;
 
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS;
    int i = 0;

    bcm_ptp_port_identity_t portid; 
 
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id,
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id,
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }

    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id,
            clock_num, &portid,
            PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_SERVO_THRESHOLD,
            0, 0, resp, &resp_len))) {
        return rv;
    }

    if (resp_len < PTP_MGMTMSG_PAYLOAD_SERVO_THRESHOLD_SIZE_OCTETS) {
        /* Internal error : message too short to contain correct data. */
        return BCM_E_INTERNAL;
    }

    /*
     * Parse response.
     *    Octet 0...5   : Custom management message key/identifier.
     *                    BCM<null><null><null>.
     *    Octet 6...9   : Frequency Lock Threshold.
     *    Octet 10...13 : Frequency Unlock Threshold.
     *    Octet 14...17 : Phase Lock Threshold.
     *    Octet 18...21 : Phase Unlock Threshold.
     */
    i = 6;
    threshold->bcmPtpServoThresholdFreqLock = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32); 
    threshold->bcmPtpServoThresholdFreqUnlock = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32); 
    threshold->bcmPtpServoThresholdPhaseLock = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32); 
    threshold->bcmPtpServoThresholdPhaseUnlock = _bcm_ptp_uint32_read(resp+i);
    
    return rv;
}


/*
 * Function:
 *      bcm_common_ptp_servo_threshold_set
 * Purpose:
 *      Set PTP servo threshold properties.
 * Parameters:
 *      unit      - (IN) Unit number.
 *      ptp_id    - (IN) PTP stack ID.
 *      clock_num - (IN) PTP clock number.
 *      threshold - (IN) PTP servo threshold properties.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int
bcm_common_ptp_servo_threshold_set(
    int unit,
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    bcm_ptp_servo_threshold_t *threshold)
{
    int rv = BCM_E_UNAVAIL;
 
    uint8 payload[PTP_MGMTMSG_PAYLOAD_SERVO_THRESHOLD_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS;
    int i = 0;

    bcm_ptp_port_identity_t portid;

    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id,
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id,
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
 
    /*
     * Make payload.
     *    Octet 0...5   : Custom management message key/identifier.
     *    Octet 6...9   : Frequency Lock Threshold.
     *    Octet 10...13 : Frequency Unlock Threshold.
     *    Octet 14...17 : Phase Lock Threshold.
     *    Octet 18...21 : Phase Unlock Threshold.
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    i +=3;
    _bcm_ptp_uint32_write(payload+i, threshold->bcmPtpServoThresholdFreqLock);
    i += sizeof(uint32); 
    _bcm_ptp_uint32_write(payload+i, threshold->bcmPtpServoThresholdFreqUnlock);
    i += sizeof(uint32); 
    _bcm_ptp_uint32_write(payload+i, threshold->bcmPtpServoThresholdPhaseLock);
    i += sizeof(uint32); 
    _bcm_ptp_uint32_write(payload+i, threshold->bcmPtpServoThresholdPhaseUnlock);

    rv = _bcm_ptp_management_message_send(unit, ptp_id, clock_num,
            &portid, PTP_MGMTMSG_SET, PTP_MGMTMSG_ID_SERVO_THRESHOLD,
            payload, PTP_MGMTMSG_PAYLOAD_SERVO_THRESHOLD_SIZE_OCTETS,
            resp, &resp_len);
 
    return rv;
}

/*
 * Function:
 *      _bcm_ptp_servo_ipdv_configuration_get
 * Purpose:
 *      Get PTP servo IP packet delay variation (IPDV) configuration properties.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      config    - (OUT) PTP servo IPDV configuration properties.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_ipdv_configuration_get(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    _bcm_ptp_ipdv_config_t *config)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;

    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }
    
    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id, 
            clock_num, &portid, 
            PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_SERVO_IPDV_CONFIGURATION, 
            0, 0, resp, &resp_len))) {
        return rv;
    }
    
    if (resp_len < PTP_MGMTMSG_PAYLOAD_SERVO_IPDV_CONFIGURATION_SIZE_OCTETS) {
        /* Internal error : message too short to contain correct data. */
        return BCM_E_INTERNAL;
    }
    
    /*
     * Parse response.
     *    Octet 0...5   : Custom management message key/identifier. 
     *                    BCM<null><null><null>.
     *    Octet 6...7   : IPDV observation interval.    
     *    Octet 8...11  : IPDV threshold (nsec).
     *    Octet 12...13 : IPDV pacing factor.
     */
    i = 6;
    config->observation_interval = _bcm_ptp_uint16_read(resp+i);
    i += sizeof(uint16);   
    config->threshold = (int32)_bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);   
    config->pacing_factor = _bcm_ptp_uint16_read(resp+i);
    
    
    return rv;
}

/*
 * Function:
 *      _bcm_ptp_servo_ipdv_configuration_set
 * Purpose:
 *      Set PTP servo IP packet delay variation (IPDV) configuration properties.
 * Parameters:
 *      unit      - (IN) Unit number.
 *      ptp_id    - (IN) PTP stack ID.
 *      clock_num - (IN) PTP clock number.
 *      config    - (IN) PTP servo IPDV configuration properties.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_ipdv_configuration_set(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    const _bcm_ptp_ipdv_config_t config)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 payload[PTP_MGMTMSG_PAYLOAD_SERVO_IPDV_CONFIGURATION_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;
    
    bcm_ptp_port_identity_t portid;     
        
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }
    
    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    /*
     * Parse response.
     *    Octet 0...5   : Custom management message key/identifier. 
     *                    BCM<null><null><null>.
     *    Octet 6...7   : IPDV observation interval.    
     *    Octet 8...11  : IPDV threshold (nsec).
     *    Octet 12...13 : IPDV pacing factor.
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    i += 3;
    _bcm_ptp_uint16_write(payload+i, config.observation_interval);
    i += sizeof(uint16);    
    _bcm_ptp_uint32_write(payload+i, (uint32)config.threshold);
    i += sizeof(uint32);    
    _bcm_ptp_uint16_write(payload+i, config.pacing_factor);
    
    rv = _bcm_ptp_management_message_send(unit, ptp_id, clock_num, 
            &portid, PTP_MGMTMSG_SET, PTP_MGMTMSG_ID_SERVO_IPDV_CONFIGURATION, 
            payload, PTP_MGMTMSG_PAYLOAD_SERVO_IPDV_CONFIGURATION_SIZE_OCTETS, 
            resp, &resp_len);
    
    return rv;
}

/*
 * Function:
 *      _bcm_ptp_servo_performance_get
 * Purpose:
 *      Get PTP servo performance data / metrics.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      perf      - (OUT) PTP servo performance data / metrics.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_performance_get(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    _bcm_ptp_servo_performance_t *perf)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;

    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }
    
    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id, 
            clock_num, &portid, 
            PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_SERVO_PERFORMANCE, 
            0, 0, resp, &resp_len))) {
        return rv;
    }
    
    if (resp_len < PTP_MGMTMSG_PAYLOAD_SERVO_PERFORMANCE_SIZE_OCTETS) {
        /* Internal error : message too short to contain correct data. */
        return BCM_E_INTERNAL;
    }
    
    /*
     * Parse response.
     *    Octet 0...5     : Custom management message key/identifier. 
     *                      BCM<null><null><null>.
     *    Octet 6         : FLL state enumeration.
     *    Octet 7...10    : FLL state duration (sec).
     *    Octet 11...18   : Forward flow weight.
     *    Octet 19...22   : Forward flow transient-free (900 sec window).
     *    Octet 23...26   : Forward flow transient-free (3600 sec window).
     *    Octet 27...34   : Forward flow transactions used (%).
     *    Octet 35...42   : Forward flow operational min TDEV (nsec).
     *    Octet 43...50   : Forward Mafie.
     *    Octet 51...58   : Forward flow min cluster width (nsec).
     *    Octet 59...66   : Forward flow mode width (nsec).
     *    Octet 67...74   : Reverse flow weight.
     *    Octet 75...78   : Reverse flow transient-free (900 sec window).
     *    Octet 79...82   : Reverse flow transient-free (3600 sec window).
     *    Octet 83...90   : Reverse flow transactions used (%).
     *    Octet 91...98   : Reverse flow operational min TDEV (nsec).
     *    Octet 99...106  : Reverse Mafie.
     *    Octet 107...114 : Reverse flow min cluster width (nsec).
     *    Octet 115...122 : Reverse flow mode width (nsec).
     *    Octet 123...130 : Frequency correction (ppb).
     *    Octet 131...138 : Phase correction (ppb).
     *    Octet 139...146 : Output TDEV estimate (nsec).
     *    Octet 147...154 : Output MDEV estimate (ppb).
     *    Octet 155...162 : Residual phase error (nsec).
     *    Octet 163...170 : Minimum round trip delay (nsec).
     *    Octet 171...172 : Sync packet rate (pkts/sec).
     *    Octet 173...174 : Delay packet rate (pkts/sec).
     *    Octet 175...182 : Forward IPDV % below threshold.
     *    Octet 183...190 : Forward maximum IPDV (usec).
     *    Octet 191...198 : Forward interpacket jitter (usec).
     *    Octet 199...206 : Reverse IPDV % below threshold.
     *    Octet 207...214 : Reverse maximum IPDV (usec).
     *    Octet 215...222 : Reverse interpacket jitter (usec). 
     */
    i = 6;
    perf->status.fll_state = (bcm_ptp_fll_state_t)resp[i++];
    perf->status.fll_state_dur = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);
    perf->fwd_weight = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_trans_free_900 = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);
    perf->fwd_trans_free_3600 = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);
    perf->fwd_pct = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_min_Tdev = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_Mafie = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_min_cluster_width = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_mode_width = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_weight = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_trans_free_900 = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);
    perf->rev_trans_free_3600 = _bcm_ptp_uint32_read(resp+i);
    i += sizeof(uint32);
    perf->rev_pct = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_min_Tdev = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_Mafie = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_min_cluster_width = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_mode_width = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->freq_correction = _bcm_ptp_int64_read(resp+i);
    i += sizeof(uint64);
    perf->phase_correction = _bcm_ptp_int64_read(resp+i);
    i += sizeof(uint64);
    perf->tdev_estimate = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->mdev_estimate = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->residual_phase_error = _bcm_ptp_int64_read(resp+i);
    i += sizeof(uint64);
    perf->min_round_trip_delay = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_pkt_rate = _bcm_ptp_uint16_read(resp+i);
    i += sizeof(uint16);
    perf->rev_pkt_rate = _bcm_ptp_uint16_read(resp+i);
    i += sizeof(uint16);
    perf->ipdv_data.fwd_pct = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->ipdv_data.fwd_max = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->ipdv_data.fwd_jitter = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->ipdv_data.rev_pct = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->ipdv_data.rev_max = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->ipdv_data.rev_jitter = _bcm_ptp_uint64_read(resp+i);

    /* Get TDEV/MDEV from CTDEV if not available from the servo: */
    if ((COMPILER_64_HI(perf->tdev_estimate) == 0xffffffff) &&
        (COMPILER_64_LO(perf->tdev_estimate) == 0xffffffff)) {
        /* Get TDEV estimate with tau = 128 */
        if (_bcm_ptp_ctdev_get(unit, ptp_id, clock_num, 128, &perf->tdev_estimate) == BCM_E_NONE) {
            /* Estimate MDEV: TDEV * sqrt(3) / tau ~= TDEV / 74  */
            perf->mdev_estimate = _bcm_ptp_llu_div(perf->tdev_estimate, 74);
        } else {
            /* No CTDEV data */
            COMPILER_64_ZERO(perf->tdev_estimate);
            COMPILER_64_ZERO(perf->mdev_estimate);
        }
    }
    
    return rv;
}

/*
 * Function:
 *      _bcm_ptp_servo_ipdv_performance_get
 * Purpose:
 *      Get PTP servo IP packet delay variation (IPDV) performance data / metrics.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      perf      - (OUT) PTP servo IPDV performance data / metrics.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_ipdv_performance_get(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    _bcm_ptp_ipdv_performance_t *perf)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;

    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }
    
    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id, 
            clock_num, &portid, 
            PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_SERVO_IPDV_PERFORMANCE, 
            0, 0, resp, &resp_len))) {
        return rv;
    }
    
    if (resp_len < PTP_MGMTMSG_PAYLOAD_SERVO_IPDV_PERFORMANCE_SIZE_OCTETS) {
        /* Internal error : message too short to contain correct data. */
        return BCM_E_INTERNAL;
    }
    
    /*
     * Parse response.
     *    Octet 0...5   : Custom management message key/identifier. 
     *                    BCM<null><null><null>.
     *    Octet 6...13  : Forward IPDV % below threshold.
     *    Octet 14...21 : Forward maximum IPDV (usec).
     *    Octet 22...29 : Forward interpacket jitter (usec).
     *    Octet 30...37 : Reverse IPDV % below threshold.
     *    Octet 38...45 : Reverse maximum IPDV (usec).
     *    Octet 46...53 : Reverse interpacket jitter (usec). 
     */
    i = 6;
    perf->fwd_pct = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_max = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->fwd_jitter = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_pct = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_max = _bcm_ptp_uint64_read(resp+i);
    i += sizeof(uint64);
    perf->rev_jitter = _bcm_ptp_uint64_read(resp+i);
    
    return rv;
}

/*
 * Function:
 *      _bcm_ptp_servo_performance_data_clear
 * Purpose:
 *      Clear (reset) PTP servo performance data / metrics.
 * Parameters:
 *      unit      - (IN) Unit number.
 *      ptp_id    - (IN) PTP stack ID.
 *      clock_num - (IN) PTP clock number.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
_bcm_ptp_servo_performance_data_clear(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 payload[PTP_MGMTMSG_PAYLOAD_MIN_PROPRIETARY_MSG_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;
    
    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    /* 
     * Make payload.
     *    Octet 0...5 : Custom management message key/identifier. 
     *                  BCM<null><null><null>.
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    
    rv = _bcm_ptp_management_message_send(unit, ptp_id, clock_num, 
            &portid, PTP_MGMTMSG_CMD, PTP_MGMTMSG_ID_SERVO_COUNTERS_CLEAR, 
            payload, PTP_MGMTMSG_PAYLOAD_MIN_PROPRIETARY_MSG_SIZE_OCTETS, 
            resp, &resp_len);
    
    return rv;
}

/*
 * Function:
 *      bcm_common_ptp_servo_status_get
 * Purpose:
 *      Get PTP servo state and status information.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      status    - (OUT) PTP servo state and status information.
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int 
bcm_common_ptp_servo_status_get(
    int unit, 
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    bcm_ptp_servo_status_t *status)
{   
    int rv = BCM_E_UNAVAIL;
    
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS; 
    int i = 0;

    bcm_ptp_port_identity_t portid;     
    
    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id, 
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;   
    }
    
    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id, 
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }
    
    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id, 
            clock_num, &portid, 
            PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_SERVO_STATUS, 
            0, 0, resp, &resp_len))) {
        return rv;
    }
    
    if (resp_len < PTP_MGMTMSG_PAYLOAD_SERVO_STATUS_SIZE_OCTETS) {
        /* Internal error : message too short to contain correct data. */
        return BCM_E_INTERNAL;
    }
    
    /*
     * Parse response.
     *    Octet 0...5     : Custom management message key/identifier. 
     *                      BCM<null><null><null>.
     *    Octet 6         : FLL state enumeration.
     *    Octet 7...10    : FLL state duration (sec).
     *    Octet 11...14        : Locked Status enumeration.
     */
    i = 6;

    status->fll_state = (bcm_ptp_fll_state_t)resp[i++];
    status->fll_state_dur = _bcm_ptp_uint32_read(resp+i);
    i += 4;
    status->lock_status = (bcm_ptp_servo_lock_status_t)resp[i];

    return rv;
}

/*
 * Function:
 *      bcm_common_ptp_phase_offset_set
 * Purpose:
 *      Set phase offsets for path asymmetry compensation
 * Parameters:
 *      unit - Unit number 
 *      ptp_id - PTP stack ID
 *      clock_num - PTP clock number
 *      *offset - PTP servo phase holdover state offsets
 * Returns:
 *      int
 * Notes:
 */

int
bcm_common_ptp_phase_offset_set(
     int unit,
     bcm_ptp_stack_id_t ptp_id,
     int clock_num,
     const bcm_ptp_phase_offset_t *offset) 
{
    int rc;
    bcm_ptp_phase_holdover_state_t state;
    state.phase_slew_rate_ppb = 0;
    state.sync_phase_offset_ns = 0;
    state.reported_phase_offset  = offset->reported_phase_offset;
    state.delta_phase_offset     = offset->delta_phase_offset;
    state.fixed_phase_offset     = offset->fixed_phase_offset;
    state.use_fixed_phase_offset = offset->use_fixed_phase_offset;

    rc = _bcm_ptp_phase_holdover_set(unit, ptp_id, clock_num, &state);
    
    return (rc);
}

/*
 * Function:
 *      bcm_common_ptp_phase_offset_get
 * Purpose:
 *      Get phase offsets for path asymmetry compensation
 * Parameters:
 *      unit - Unit number 
 *      ptp_id - PTP stack ID
 *      clock_num - PTP clock number
 *      *offset - PTP servo phase holdover state offsets
 * Returns:
 *      int
 * Notes:
 */

int
bcm_common_ptp_phase_offset_get(
     int unit,
     bcm_ptp_stack_id_t ptp_id,
     int clock_num,
     bcm_ptp_phase_offset_t *offset) 
{
    int rc;
    bcm_ptp_phase_holdover_state_t state;


    rc = _bcm_ptp_phase_holdover_get(unit, ptp_id, clock_num, &state);

    offset->reported_phase_offset = state.reported_phase_offset;
    offset->delta_phase_offset = state.delta_phase_offset;
    offset->fixed_phase_offset = state.fixed_phase_offset;
    offset->use_fixed_phase_offset = state.use_fixed_phase_offset;

    return (rc);
}


/*
 * Function:
 *      _bcm_ptp_phase_holdover_get
 * Purpose:
 *      Get phase offsets for path asymmetry compensation.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      state     - (OUT) PTP servo phase holdover state
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int
_bcm_ptp_phase_holdover_get(
    int unit,
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    bcm_ptp_phase_holdover_state_t *state)
{
    int rv;
    int i;

    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS;

    bcm_ptp_port_identity_t portid;

    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id,
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id,
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }

    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id,
            clock_num, &portid,
            PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_PHASE_OFFSET,
            0, 0, resp, &resp_len))) {
        return rv;
    }

    /*
     * Parse response.
     *    Octet 0...5   : Custom management message key/identifier.
     *                    BCM<null><null><null>.
     *    Octet 6...13  : Reported phase offset.
     *    Octet 14...21 : Delta offset.
     *    Octet 22...29 : Fixed phase offset.
     *    Octet 30...33 : Use fixed phase offset flag.
     *    Octet 34...37 : Phase slew rate - maximum phase offset correction rate.
     *    Octet 38...41 : Sync phase offst - maximum phase offset correction
     *                    via rate-limited phase slew.
     */
    i = 6;
    state->reported_phase_offset = _bcm_ptp_int64_read(resp + i);
    i += 8;
    state->delta_phase_offset = _bcm_ptp_int64_read(resp + i);
    i += 8;
    state->fixed_phase_offset = _bcm_ptp_int64_read(resp + i);
    i += 8;
    state->use_fixed_phase_offset = _bcm_ptp_uint32_read(resp + i);
    i += 4;
    state->phase_slew_rate_ppb = _bcm_ptp_uint32_read(resp + i);
    i += 4;
    state->sync_phase_offset_ns = _bcm_ptp_uint32_read(resp + i);

    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_ptp_phase_holdover_set
 * Purpose:
 *      Set phase offsets for path asymmetry compensation.
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      ptp_id    - (IN)  PTP stack ID.
 *      clock_num - (IN)  PTP clock number.
 *      state     - (IN) PTP servo phase holdover state
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 */
int
_bcm_ptp_phase_holdover_set(
    int unit,
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    const bcm_ptp_phase_holdover_state_t *state)
{
    int rv;
    int i;

    uint8 payload[PTP_MGMTMSG_PAYLOAD_PHASE_OFFSET_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS;

    bcm_ptp_port_identity_t portid;

    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id,
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id,
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }

    /*
     * Make payload.
     *    Octet 0...5   : Custom management message key/identifier.
     *                    BCM<null><null><null>.
     *    Octet 6...13 : Delta offset.
     *    Octet 14...21 : Fixed phase offset.
     *    Octet 22...25 : Use fixed phase offset flag.
     *    Octet 26...29 : Phase slew rate - maximum phase offset correction rate.
     *    Octet 30...33 : Sync phase offst - maximum phase offset correction
     *                    via rate-limited phase slew.
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    payload[i++] = '\0';
    payload[i++] = '\0';
    payload[i++] = '\0';

    _bcm_ptp_int64_write(payload + i, state->delta_phase_offset);
    i += 8;
    _bcm_ptp_int64_write(payload + i, state->fixed_phase_offset);
    i += 8;
    if (state->use_fixed_phase_offset) {
        _bcm_ptp_uint32_write(payload + i, 1);
    } else {
        _bcm_ptp_uint32_write(payload + i, 0);
    }
    i += 4;
    _bcm_ptp_uint32_write(payload + i, state->phase_slew_rate_ppb);
    i += 4;
    _bcm_ptp_uint32_write(payload + i, state->sync_phase_offset_ns);

    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id,
            clock_num, &portid,
            PTP_MGMTMSG_SET, PTP_MGMTMSG_ID_PHASE_OFFSET,
            payload, PTP_MGMTMSG_PAYLOAD_PHASE_OFFSET_SIZE_OCTETS,
            resp, &resp_len))) {
        return rv;
    }

    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_ptp_freq_corr_get
 * Purpose:
 *      Gets current frequency offsets
 * Parameters:
 *      unit            - (IN) Unit number.
 *      ptp_id          - (IN) PTP stack ID.
 *      clock_num       - (IN) PTP clock number.
 *      freq_corr_ppt   - (IN) Servo-issued frequency correction
 *      offset_freq_ppt - (IN) User-specified frequency offset
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 *      The current frequency offset is (freq_corr_ppt + offset_freq_ppt)
 */
int
_bcm_ptp_freq_corr_get(
    int unit,
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    int32 *freq_corr_ppt,
    int32 *offset_freq_ppt)
{
    int rv;

    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS;
    uint8 *curs;

    bcm_ptp_port_identity_t portid;

    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id,
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id,
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }

    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id,
            clock_num, &portid, PTP_MGMTMSG_GET, PTP_MGMTMSG_ID_FREQUENCY,
            0, 0, resp, &resp_len))) {
        return rv;
    }

    /*
     * Parse response.
     *    Octet 0...5   : Custom management message key/identifier.
     *                    BCM<null><null><null>.
     *    Octet 6...9   : Frequency Correction in PPT (* 1e-12)
     *    Octet 10...13 : Frequency Offset from Servo's correction in PPT
     */
    if (resp_len < 6 + 4 + 4) {
        return BCM_E_FAIL;
    }

    curs = resp + 6;     /* after BCM\0\0\0 */
    *freq_corr_ppt = _bcm_ptp_uint32_read(curs);  curs += 4;
    *offset_freq_ppt = _bcm_ptp_uint32_read(curs);  curs += 4;

    return BCM_E_NONE;
}


/*
 * Function:
 *      _bcm_ptp_freq_corr_set
 * Purpose:
 *      Gets current frequency offsets
 * Parameters:
 *      unit            - (IN) Unit number.
 *      ptp_id          - (IN) PTP stack ID.
 *      clock_num       - (IN) PTP clock number.
 *      freq_corr_ppt   - (IN) Servo-issued frequency correction
 *      offset_freq_ppt - (IN) User-specified frequency offset
 * Returns:
 *      BCM_E_XXX - Function status.
 * Notes:
 *      The current frequency offset is (freq_corr_ppt + offset_freq_ppt)
 *      If the servo is updating its frequency correction estimate, the
 *      user-supplied 'freq_corr_ppt' value will be overwritten with that
 *      new estimate.
 */
int
_bcm_ptp_freq_corr_set(
    int unit,
    bcm_ptp_stack_id_t ptp_id,
    int clock_num,
    int32 freq_corr_ppt,
    int32 freq_offset_ppt)
{
    int rv;
    int i;

    uint8 payload[PTP_MGMTMSG_PAYLOAD_PHASE_OFFSET_SIZE_OCTETS] = {0};
    uint8 resp[PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS];
    int resp_len = PTP_MGMTMSG_RESP_MAX_SIZE_OCTETS;

    bcm_ptp_port_identity_t portid;

    if (BCM_FAILURE(rv = _bcm_ptp_function_precheck(unit, ptp_id,
            clock_num, PTP_CLOCK_PORT_NUMBER_DEFAULT))) {
        return rv;
    }

    if (BCM_FAILURE(rv = bcm_common_ptp_clock_port_identity_get(unit, ptp_id,
            clock_num, PTP_IEEE1588_ALL_PORTS, &portid))) {
        /* On error, target message to (all clocks, all ports) portIdentity. */
        portid = portid_all;
    }

    /*
     * Make payload.
     *    Octet 0...5   : Custom management message key/identifier.
     *                    BCM<null><null><null>.
     *    Octet 6...9   : Frequency Correction in PPT (* 1e-12)
     *    Octet 10...13 : Frequency Offset from Servo's correction in PPT
     */
    i = 0;
    payload[i++] = 'B';
    payload[i++] = 'C';
    payload[i++] = 'M';
    payload[i++] = '\0';
    payload[i++] = '\0';
    payload[i++] = '\0';

    _bcm_ptp_uint32_write(payload + i, freq_corr_ppt);   i += 4;
    _bcm_ptp_uint32_write(payload + i, freq_offset_ppt); i += 4;

    if (BCM_FAILURE(rv = _bcm_ptp_management_message_send(unit, ptp_id,
            clock_num, &portid,
            PTP_MGMTMSG_SET, PTP_MGMTMSG_ID_FREQUENCY,
            payload, PTP_MGMTMSG_PAYLOAD_FREQUENCY_SIZE_OCTETS,
            resp, &resp_len))) {
        return rv;
    }

    return BCM_E_NONE;
}

#endif /* defined(INCLUDE_PTP)*/
