/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 */

#ifndef __BCM_XFLOW_MACSEC_H__
#define __BCM_XFLOW_MACSEC_H__

#if defined(INCLUDE_XFLOW_MACSEC)

#include <bcm/types.h>
#include <xflow_macsec_defs.h>

#define BCM_XFLOW_MACSEC_SECURE_CHAN_ID_CREATE(flag, chan_id)                  \
                XFLOW_MACSEC_SECURE_CHAN_ID_CREATE(flag, chan_id) 
#define BCM_XFLOW_MACSEC_ENCRYPT_DECRYPT_NONE XFLOW_MACSEC_ENCRYPT_DECRYPT_NONE /* Indicates the
                                                          parameters are not
                                                          specific to encrypt or
                                                          decrypt flow or
                                                          applies to both. */
#define BCM_XFLOW_MACSEC_ENCRYPT            XFLOW_MACSEC_ENCRYPT /* Indicates the
                                                          parameters are for
                                                          encryption. */
#define BCM_XFLOW_MACSEC_DECRYPT            XFLOW_MACSEC_DECRYPT /* Indicates the
                                                          parameters are for
                                                          decryption. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_WITH_ID XFLOW_MACSEC_SECURE_CHAN_WITH_ID /* To be used when secure
                                                          channel needs to be
                                                          created with a
                                                          particular id. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_INFO_INCLUDE_SCI XFLOW_MACSEC_SECURE_CHAN_INFO_INCLUDE_SCI /* Indicates the 64-bit
                                                          SCI should be included
                                                          in the SecTAG during
                                                          encryption. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_INFO_CONTROLLED_PORT XFLOW_MACSEC_SECURE_CHAN_INFO_CONTROLLED_PORT /* Allow all data packets
                                                          and management
                                                          packets. The default
                                                          is uncontrolled port
                                                          which is to discard
                                                          all data packets and
                                                          allow only management
                                                          packets. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_INFO_ENCRYPT_DISABLE XFLOW_MACSEC_SECURE_CHAN_INFO_ENCRYPT_DISABLE /* Do not encrypt the
                                                          packet using the
                                                          Cipher suite. The
                                                          default behavior is to
                                                          encrypt. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_INFO_REPLAY_PROTECT_ENABLE XFLOW_MACSEC_SECURE_CHAN_INFO_REPLAY_PROTECT_ENABLE /* Enable replay protect. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_RANGE_START XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_RANGE_START /* The offset extracted
                                                          from the SVTAG is
                                                          added to
                                                          first_auth_range_offset_start.
                                                          Applicable only for
                                                          Inline Xflow Macsec. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_RANGE_END XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_RANGE_END /* The offset extracted
                                                          from the SVTAG is
                                                          added to
                                                          first_auth_range_offset_end.
                                                          Applicable only for
                                                          Inline Xflow Macsec. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_SECTAG XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_SECTAG /* The offset extracted
                                                          from the SVTAG is
                                                          added to
                                                          sectag_offset.
                                                          Applicable only for
                                                          Inline Xflow Macsec. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_VXLANSEC XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_VXLANSEC /* The offset extracted
                                                          from SVTAG is added to
                                                          the base offsets
                                                          calculated using
                                                          vxlansec_pkt_type. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_ZERO_OUT_SA_INVALID_PKT XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_ZERO_OUT_SA_INVALID_PKT /* In case of any invalid
                                                          SA error, zero out the
                                                          data from
                                                          confidentiality_offset
                                                          onwards before sending
                                                          it out. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_DECRYPT_ADJUST_RANGE_START XFLOW_MACSEC_SECURE_CHAN_DECRYPT_ADJUST_RANGE_START /* Increment
                                                          first_auth_range_offset_start
                                                          based on the number of
                                                          VLAN tags or MPLS
                                                          labels present in the
                                                          ingress packet. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_DECRYPT_ADJUST_RANGE_END XFLOW_MACSEC_SECURE_CHAN_DECRYPT_ADJUST_RANGE_END /* Increment
                                                          first_auth_range_offset_end
                                                          based on the number of
                                                          VLAN tags or MPLS
                                                          labels present in the
                                                          ingress packet. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_RANGE_AUTHENTICATE XFLOW_MACSEC_SECURE_CHAN_RANGE_AUTHENTICATE /* Prior to Sectag,
                                                          authenticate only the
                                                          bytes between
                                                          first_auth_range_offset_start
                                                          and
                                                          first_auth_range_offset_end.
                                                          Default is to
                                                          authenticate all bytes
                                                          prior to Sectag as
                                                          mandated by the IEEE
                                                          spec. Applicable to
                                                          both encrypt and
                                                          decrypt. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_SECURED_DATA XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_SECURED_DATA /* ControlledPort is
                                                          enabled for secured
                                                          data packets. Secured
                                                          data packets are
                                                          allowed and accounted
                                                          in MIBs. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_UNSECURED_DATA XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_UNSECURED_DATA /* ControlledPort is
                                                          enabled for un-secured
                                                          data packets.
                                                          Unsecured data packets
                                                          are allowed and
                                                          accounted in MIBs. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_PROTECT_DISABLE XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_PROTECT_DISABLE /* Do not encrypt or
                                                          authenticate the
                                                          egress packet. Default
                                                          is to authenticate the
                                                          packet. Additionally,
                                                          the packet will be
                                                          encrypted if TCI.E is
                                                          set. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SET_MPLS_BOS_ENABLE XFLOW_MACSEC_SECURE_CHAN_ENCRYPT_SET_MPLS_BOS_ENABLE /* Set the
                                                          Bottom-of-Stack bit in
                                                          the last MPLS label
                                                          before the SecTAG. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_DECRYPT_SA_EXPIRE_DISABLE XFLOW_MACSEC_SECURE_CHAN_DECRYPT_SA_EXPIRE_DISABLE /* Disable both soft and
                                                          hard SA expiry
                                                          notification in the
                                                          decrypt direction. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_DECRYPT_DUPLICATE_REJECT XFLOW_MACSEC_SECURE_CHAN_DECRYPT_DUPLICATE_REJECT /* Reject all secured
                                                          duplicate packets
                                                          within 128 Packet
                                                          Number for all secure
                                                          associations for the
                                                          channel. Applicable
                                                          only if replay protect
                                                          is enabled. */
#define BCM_XFLOW_MACSEC_SECURE_CHAN_DECRYPT_RESET_MPLS_BOS XFLOW_MACSEC_SECURE_CHAN_DECRYPT_RESET_MPLS_BOS /* When set, will reset
                                                          the Bottom-of-Stack
                                                          bit in the last MPLS
                                                          label before SECTAG of
                                                          a received Secure MPLS
                                                          packet whose SECTAG
                                                          and ICV are both
                                                          stripped */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_ZERO_OUT_SA_INVALID_PKT XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_ZERO_OUT_SA_INVALID_PKT /* In case of any invalid
                                                          SA error, zero out the
                                                          data from
                                                          confidentiality_offset
                                                          onwards before sending
                                                          it out. Applicable for
                                                          IPsec configuration. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_SET_MPLS_BOS_ENABLE XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_SET_MPLS_BOS_ENABLE /* Set the
                                                          Bottom-of-Stack bit in
                                                          the last MPLS label
                                                          before the ESP header. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_L3_L4 XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_L3_L4 /* The offset extracted
                                                          from SVTAG is added to
                                                          the base offsets
                                                          calculated using
                                                          l3_l4_pkt_type. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_INFO_REPLAY_PROTECT_ENABLE XFLOW_MACSEC_IPSEC_SECURE_CHAN_INFO_REPLAY_PROTECT_ENABLE /* Enable replay protect. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_PROTECT_DISABLE XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_PROTECT_DISABLE /* Do not encrypt or
                                                          authenticate the
                                                          egress packet. Default
                                                          is to authenticate the
                                                          packet. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_SECURED_DATA XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_SECURED_DATA /* ControlledPort is
                                                          enabled for secured
                                                          data packets. Secured
                                                          data packets are
                                                          allowed and accounted
                                                          in MIBs. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_UNSECURED_DATA XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_CONTROLLED_UNSECURED_DATA /* ControlledPort is
                                                          enabled for un-secured
                                                          data packets.
                                                          Unsecured data packets
                                                          are allowed and
                                                          accounted in MIBs. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_AUTH_AES_GMAC XFLOW_MACSEC_IPSEC_SECURE_CHAN_AUTH_AES_GMAC /* Enable IPsec
                                                          authentication only
                                                          mode (GMAC mode - RFC
                                                          4543). */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_DECRYPT_SA_EXPIRE_DISABLE XFLOW_MACSEC_IPSEC_SECURE_CHAN_DECRYPT_SA_EXPIRE_DISABLE /* Disable both soft and
                                                          hard SA expiry
                                                          notification in the
                                                          decrypt direction. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_DECRYPT_DUPLICATE_REJECT XFLOW_MACSEC_IPSEC_SECURE_CHAN_DECRYPT_DUPLICATE_REJECT /* Reject all secured
                                                          duplicate packets
                                                          within 128 Sequence
                                                          Number for all secure
                                                          associations for the
                                                          channel. Applicable
                                                          only if replay protect
                                                          is enabled. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_INFO_CONTROLLED_PORT XFLOW_MACSEC_IPSEC_SECURE_CHAN_INFO_CONTROLLED_PORT /* Enable all data
                                                          packets on the
                                                          controlled port. Else,
                                                          discard all data
                                                          packets. Encrypt only
                                                          flag. Applicable only
                                                          for IPsec
                                                          configuration. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ALLOCATE_MATCH_ACTION_ENTRY1 XFLOW_MACSEC_IPSEC_SECURE_CHAN_ALLOCATE_MATCH_ACTION_ENTRY1 /* Allocate second match
                                                          action entry (when
                                                          provided both match
                                                          entries are used).
                                                          Applicable only for
                                                          IPsec configuration.
                                                          Decrypt only flag. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_ESP XFLOW_MACSEC_IPSEC_SECURE_CHAN_ENCRYPT_SVTAG_OFFSET_TO_ESP /* Add the offset
                                                          extracted from SVTAG
                                                          to ESP offset. */

typedef enum bcm_xflow_macsec_crypto_e {
    bcmXflowMacsecCryptoAes128GcmIntegrityOnly = xflowMacsecCryptoAes128GcmIntegrityOnly, 
    bcmXflowMacsecCryptoAes128Gcm = xflowMacsecCryptoAes128Gcm, 
    bcmXflowMacsecCryptoAes128GcmXpnIntegrityOnly = xflowMacsecCryptoAes128GcmXpnIntegrityOnly, 
    bcmXflowMacsecCryptoAes128GcmXpn = xflowMacsecCryptoAes128GcmXpn, 
    bcmXflowMacsecCryptoAes256GcmIntegrityOnly = xflowMacsecCryptoAes256GcmIntegrityOnly, 
    bcmXflowMacsecCryptoAes256Gcm = xflowMacsecCryptoAes256Gcm, 
    bcmXflowMacsecCryptoAes256GcmXpnIntegrityOnly = xflowMacsecCryptoAes256GcmXpnIntegrityOnly, 
    bcmXflowMacsecCryptoAes256GcmXpn = xflowMacsecCryptoAes256GcmXpn, 
    bcmXflowMacsecCryptoCount = xflowMacsecCryptoCount 
} bcm_xflow_macsec_crypto_t;

typedef enum bcm_xflow_macsec_mtu_e {
    bcmXflowMacsecMtu0 = xflowMacsecMtu0, 
    bcmXflowMacsecMtu1 = xflowMacsecMtu1, 
    bcmXflowMacsecMtu2 = xflowMacsecMtu2, 
    bcmXflowMacsecMtu3 = xflowMacsecMtu3, 
    bcmXflowMacsecMtuCount = xflowMacsecMtuCount 
} bcm_xflow_macsec_mtu_t;

typedef enum bcm_xflow_macsec_sectag_ethertype_e {
    bcmXflowMacsecSecTagEtype0 = xflowMacsecSecTagEtype0, 
    bcmXflowMacsecSecTagEtype1 = xflowMacsecSecTagEtype1, 
    bcmXflowMacsecSecTagEtype2 = xflowMacsecSecTagEtype2, 
    bcmXflowMacsecSecTagEtype3 = xflowMacsecSecTagEtype3, 
    bcmXflowMacsecSecTagEtypeCount = xflowMacsecSecTagEtypeCount 
} bcm_xflow_macsec_sectag_ethertype_t;

typedef enum bcm_xflow_macsec_secure_chan_vxlansec_hdr_e {
    bcmXflowMacsecSecureChanVxLanSecHdrNoChange = xflowMacsecSecureChanVxLanSecHdrNoChange, 
    bcmXflowMacsecSecureChanVxLanSecHdrUpdateLength = xflowMacsecSecureChanVxLanSecHdrUpdateLength, 
    bcmXflowMacsecSecureChanVxLanSecHdrUpdateDestPort = xflowMacsecSecureChanVxLanSecHdrUpdateDestPort, 
    bcmXflowMacsecSecureChanVxLanSecHdrUpdateAll = xflowMacsecSecureChanVxLanSecHdrUpdateAll, 
    bcmXflowMacsecSecureChanVxLanSecHdrCount = xflowMacsecSecureChanVxLanSecHdrCount 
} bcm_xflow_macsec_secure_chan_vxlansec_hdr_t;

typedef enum bcm_xflow_macsec_secure_chan_vxlansec_pkt_type_e {
    bcmXflowMacsecSecureChanVxLanSecIPv40Vlan = xflowMacsecSecureChanVxLanSecIPv40Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv41Vlan = xflowMacsecSecureChanVxLanSecIPv41Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv42Vlan = xflowMacsecSecureChanVxLanSecIPv42Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv60Vlan = xflowMacsecSecureChanVxLanSecIPv61Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv61Vlan = xflowMacsecSecureChanVxLanSecIPv61Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv62Vlan = xflowMacsecSecureChanVxLanSecIPv62Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv4TCP0Vlan = xflowMacsecSecureChanVxLanSecIPv4TCP0Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv4TCP1Vlan = xflowMacsecSecureChanVxLanSecIPv4TCP1Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv4TCP2Vlan = xflowMacsecSecureChanVxLanSecIPv4TCP2Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv4UDP0Vlan = xflowMacsecSecureChanVxLanSecIPv4UDP0Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv4UDP1Vlan = xflowMacsecSecureChanVxLanSecIPv4UDP1Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv4UDP2Vlan = xflowMacsecSecureChanVxLanSecIPv4UDP2Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv6TCP0Vlan = xflowMacsecSecureChanVxLanSecIPv6TCP0Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv6TCP1Vlan = xflowMacsecSecureChanVxLanSecIPv6TCP1Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv6TCP2Vlan = xflowMacsecSecureChanVxLanSecIPv6TCP2Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv6UDP0Vlan = xflowMacsecSecureChanVxLanSecIPv6UDP0Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv6UDP1Vlan = xflowMacsecSecureChanVxLanSecIPv6UDP1Vlan, 
    bcmXflowMacsecSecureChanVxLanSecIPv6UDP2Vlan = xflowMacsecSecureChanVxLanSecIPv6UDP2Vlan, 
    bcmXflowMacsecSecureChanVxLanSecNoVxLanSec = xflowMacsecSecureChanVxLanSecNoVxLanSec, 
    bcmXflowMacsecSecureChanVxLanSecCount = xflowMacsecSecureChanVxLanSecCount 
} bcm_xflow_macsec_secure_chan_vxlansec_pkt_type_t;

typedef enum bcm_xflow_macsec_secure_chan_l3_l4_hdr_e {
    bcmXflowMacsecSecureChanL3L4HdrNoChange = xflowMacsecSecureChanL3L4HdrNoChange, 
    bcmXflowMacsecSecureChanL3L4HdrUpdateLength = xflowMacsecSecureChanL3L4HdrUpdateLength, 
    bcmXflowMacsecSecureChanL3L4HdrUpdateDestPort = xflowMacsecSecureChanL3L4HdrUpdateDestPort, 
    bcmXflowMacsecSecureChanL3L4HdrUpdateAll = xflowMacsecSecureChanL3L4HdrUpdateAll, 
    bcmXflowMacsecSecureChanL3L4HdrCount = xflowMacsecSecureChanL3L4HdrCount 
} bcm_xflow_macsec_secure_chan_l3_l4_hdr_t;

typedef enum bcm_xflow_macsec_secure_chan_l3_l4_pkt_type_e {
    bcmXflowMacsecSecureChanL3L4IPv40Vlan = xflowMacsecSecureChanL3L4IPv40Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv41Vlan = xflowMacsecSecureChanL3L4IPv41Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv42Vlan = xflowMacsecSecureChanL3L4IPv42Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv60Vlan = xflowMacsecSecureChanL3L4IPv61Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv61Vlan = xflowMacsecSecureChanL3L4IPv61Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv62Vlan = xflowMacsecSecureChanL3L4IPv62Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv4TCP0Vlan = xflowMacsecSecureChanL3L4IPv4TCP0Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv4TCP1Vlan = xflowMacsecSecureChanL3L4IPv4TCP1Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv4TCP2Vlan = xflowMacsecSecureChanL3L4IPv4TCP2Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv4UDP0Vlan = xflowMacsecSecureChanL3L4IPv4UDP0Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv4UDP1Vlan = xflowMacsecSecureChanL3L4IPv4UDP1Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv4UDP2Vlan = xflowMacsecSecureChanL3L4IPv4UDP2Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv6TCP0Vlan = xflowMacsecSecureChanL3L4IPv6TCP0Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv6TCP1Vlan = xflowMacsecSecureChanL3L4IPv6TCP1Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv6TCP2Vlan = xflowMacsecSecureChanL3L4IPv6TCP2Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv6UDP0Vlan = xflowMacsecSecureChanL3L4IPv6UDP0Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv6UDP1Vlan = xflowMacsecSecureChanL3L4IPv6UDP1Vlan, 
    bcmXflowMacsecSecureChanL3L4IPv6UDP2Vlan = xflowMacsecSecureChanL3L4IPv6UDP2Vlan, 
    bcmXflowMacsecSecureChanL3L4NoL3L4 = xflowMacsecSecureChanL3L4NoL3L4, 
    bcmXflowMacsecSecureChanL3L4Count = xflowMacsecSecureChanL3L4Count 
} bcm_xflow_macsec_secure_chan_l3_l4_pkt_type_t;

typedef enum bcm_xflow_macsec_index_type_e {
    bcmXflowMacsecIdTypeSecureChan = xflowMacsecIdTypeSecureChan, 
    bcmXflowMacsecIdTypeSecureAssoc = xflowMacsecIdTypeSecureAssoc, 
    bcmXflowMacsecIdTypePolicy = xflowMacsecIdTypePolicy, 
    bcmXflowMacsecIdTypeFlow = xflowMacsecIdTypeFlow, 
    bcmXflowMacsecIdTypeSubportNum = xflowMacsecIdTypeSubportNum, 
    bcmXflowMacsecIdTypePort = xflowMacsecIdTypePort, 
    bcmXflowMacsecIdTypeInvalid = xflowMacsecIdTypeInvalid, 
    bcmXflowMacsecIdTypeCount = xflowMacsecIdTypeCount 
} bcm_xflow_macsec_index_type_t;

typedef xflow_macsec_instance_id_t bcm_xflow_macsec_instance_id_t;

typedef xflow_macsec_instance_pbmp_t bcm_xflow_macsec_instance_pbmp_t;

typedef xflow_macsec_secure_chan_info_t bcm_xflow_macsec_secure_chan_info_t;

typedef xflow_macsec_secure_chan_id_t bcm_xflow_macsec_secure_chan_id_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_xflow_macsec_secure_chan_create installs a new security channel
 * entry. It returns the secure channel ID.
 */
extern int bcm_xflow_macsec_secure_chan_create(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_secure_chan_info_t *chan_info, 
    int priority, 
    bcm_xflow_macsec_secure_chan_id_t *chan_id);

/* Set MACSec configuration for the specified port. */
extern int bcm_xflow_macsec_secure_chan_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_secure_chan_info_t *chan_info, 
    int priority);

/* Get the security channel configuration for a given SC index. */
extern int bcm_xflow_macsec_secure_chan_get(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_secure_chan_info_t *chan_info, 
    int *priority);

/* 
 * Delete the given security channel. The corresponding SA must be
 * destroyed first.
 */
extern int bcm_xflow_macsec_secure_chan_destroy(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize the secure channel data structure. */
extern void bcm_xflow_macsec_secure_chan_info_t_init(
    bcm_xflow_macsec_secure_chan_info_t *chan_info);

#ifndef BCM_HIDE_DISPATCHABLE

/* Enable the Security Channel. */
extern int bcm_xflow_macsec_secure_chan_enable_set(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    int enable);

/* Check if the Security Channel is enabled. */
extern int bcm_xflow_macsec_secure_chan_enable_get(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    int *enable);

#endif /* BCM_HIDE_DISPATCHABLE */

/* bcm_xflow_macsec_chan_traverse_cb */
typedef int (*bcm_xflow_macsec_chan_traverse_cb)(
    int unit, 
    bcm_xflow_macsec_secure_chan_info_t *chan_info, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* Traverse all secure channels. */
extern int bcm_xflow_macsec_secure_chan_info_traverse(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_chan_traverse_cb callback, 
    void *user_data);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_XFLOW_MACSEC_SECURE_ASSOC_ID_CREATE(flag, assoc_id)                  \
                XFLOW_MACSEC_SECURE_ASSOC_ID_CREATE(flag, assoc_id) 
#define BCM_XFLOW_MACSEC_SECURE_ASSOC_INFO_SET_NEXT_PKT_NUM XFLOW_MACSEC_SECURE_ASSOC_INFO_SET_NEXT_PKT_NUM /* Set the next PN of the
                                                          security association
                                                          to the value given. */
#define BCM_XFLOW_MACSEC_IPSEC_SECURE_ASSOC_INFO_SET_NEXT_PKT_NUM XFLOW_MACSEC_IPSEC_SECURE_ASSOC_INFO_SET_NEXT_PKT_NUM /* Set the next PN of the
                                                          IPsec security
                                                          association to the
                                                          value given. */

typedef xflow_macsec_secure_assoc_info_t bcm_xflow_macsec_secure_assoc_info_t;

typedef xflow_macsec_crypto_aes128_gcm_t bcm_xflow_macsec_crypto_aes128_gcm_t;

typedef xflow_macsec_crypto_aes256_gcm_t bcm_xflow_macsec_crypto_aes256_gcm_t;

typedef uint32 bcm_xflow_macsec_secure_assoc_id_t;

typedef enum bcm_xflow_macsec_secure_assoc_an_control_e {
    bcmXflowMacsecSecureAssocAnNormal = xflowMacsecSecureAssocAnNormal, 
    bcmXflowMacsecSecureAssocAnRollover = xflowMacsecSecureAssocAnRollover, 
    bcmXflowMacsecSecureAssocAnAuto = xflowMacsecSecureAssocAnAuto, 
    bcmXflowMacsecSecureAssocAnCount = xflowMacsecSecureAssocAnCount 
} bcm_xflow_macsec_secure_assoc_an_control_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_xflow_macsec_secure_assoc_create creates a new Security
 * Association for the given Security Channel.
 */
extern int bcm_xflow_macsec_secure_assoc_create(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_secure_assoc_info_t *assoc_info, 
    bcm_xflow_macsec_secure_assoc_id_t *assoc_id);

/* 
 * Sets/resets the value of any of the parameters of a given Security
 * Association.
 */
extern int bcm_xflow_macsec_secure_assoc_set(
    int unit, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id, 
    bcm_xflow_macsec_secure_assoc_info_t *assoc_info);

/* 
 * Retrieve the Security Association configuration and the SC index from
 * the SA index.
 */
extern int bcm_xflow_macsec_secure_assoc_get(
    int unit, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id, 
    bcm_xflow_macsec_secure_assoc_info_t *assoc_info, 
    bcm_xflow_macsec_secure_chan_id_t *chan_id);

/* Deletes the SA entry corresponding to the Security Association. */
extern int bcm_xflow_macsec_secure_assoc_destroy(
    int unit, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize the Security Association data structure. */
extern void bcm_xflow_macsec_secure_assoc_info_t_init(
    bcm_xflow_macsec_secure_assoc_info_t *assoc_info);

/* bcm_xflow_macsec_secure_assoc_traverse_cb */
typedef int (*bcm_xflow_macsec_secure_assoc_traverse_cb)(
    int unit, 
    bcm_xflow_macsec_secure_assoc_info_t *assoc, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* Traverse all the secure association belonging to a Secure channel. */
extern int bcm_xflow_macsec_secure_assoc_traverse(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_secure_assoc_traverse_cb callback, 
    void *user_data);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_ID_CREATE(id)                  \
                XFLOW_MACSEC_POLICY_ID_CREATE(BCM_XFLOW_MACSEC_DECRYPT, id) 
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_WITH_ID XFLOW_MACSEC_POLICY_WITH_ID /* Provide the policy_id
                                                          as input to the API. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_UNTAGGED_FRAME_DENY XFLOW_MACSEC_DECRYPT_POLICY_UNTAGGED_FRAME_DENY /* Deny and account all
                                                          data packet which
                                                          doesn't have a sectag. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_CHECK_ICV XFLOW_MACSEC_DECRYPT_POLICY_CHECK_ICV /* Allow all data and
                                                          control packets to be
                                                          permitted and
                                                          accounted regardless
                                                          of policy violation,
                                                          but do check and
                                                          account (but not drop)
                                                          for ICV violations. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_CHECK_NONE XFLOW_MACSEC_DECRYPT_POLICY_CHECK_NONE /* Allow all data and
                                                          control packets to be
                                                          permitted and
                                                          accounted regardless
                                                          of policy violation
                                                          and do not perform an
                                                          ICV check. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_UNTAGGED_CONTROL_PORT_ENABLE XFLOW_MACSEC_DECRYPT_POLICY_UNTAGGED_CONTROL_PORT_ENABLE /* Allow data packets
                                                          which are untagged
                                                          (i.e., they don't have
                                                          a SecTAG) to be
                                                          processed by the
                                                          MACsec logic. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_TAGGED_CONTROL_PORT_ENABLE XFLOW_MACSEC_DECRYPT_POLICY_TAGGED_CONTROL_PORT_ENABLE /* Allow data
                                                          (non-management)
                                                          packets which are
                                                          tagged (i.e., they
                                                          have a SecTAG) to be
                                                          processed by the
                                                          MACsec logic. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_POINT_TO_POINT_ENABLE XFLOW_MACSEC_DECRYPT_POLICY_POINT_TO_POINT_ENABLE /* Enable the point to
                                                          point mode in the
                                                          Policy table. This
                                                          mode is disabled by
                                                          default. The SCI value
                                                          is only programmed
                                                          when point to point
                                                          mode is enabled. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_FOR_CONTROL_PACKET XFLOW_MACSEC_DECRYPT_POLICY_FOR_CONTROL_PACKET /* Creates Decrypt policy
                                                          for mangement packets. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_CUSTOM_PROTOCOL XFLOW_MACSEC_DECRYPT_POLICY_CUSTOM_PROTOCOL /* The packets contain
                                                          custom protocol tag.
                                                          The sectag is
                                                          identified by
                                                          sectag_offset. 
                                                          Applicable only for
                                                          Inline Xflow Macsec. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_SECTAG_OFFSET_ADJUST XFLOW_MACSEC_DECRYPT_POLICY_SECTAG_OFFSET_ADJUST /* Adjust sectag_offset
                                                          based on the number of
                                                          VLAN Tags and MPLS
                                                          labels. Applicable
                                                          only for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_IPV4_CHKSUM_FAIL_AND_MPLS_BOS_MISS_DENY XFLOW_MACSEC_DECRYPT_POLICY_IPV4_CHKSUM_FAIL_AND_MPLS_BOS_MISS_DENY /* For IPv4 packets,
                                                          tagged control port
                                                          packets with checksum
                                                          fail are dropped. For
                                                          MPLS packets, tagged
                                                          and untagged control
                                                          port packets with MPLS
                                                          BOS not found    are
                                                          dropped. In both
                                                          cases, drop happen
                                                          only when they are not
                                                          copied to CPU. */
#define BCM_XFLOW_MACSEC_DECRYPT_POLICY_INNER_L2_VALID XFLOW_MACSEC_DECRYPT_POLICY_INNER_L2_VALID /* Inner DA and Inner SA
                                                          are in clear and they
                                                          are available before
                                                          SecTag. */
#define BCM_XFLOW_MACSEC_IPSEC_DECRYPT_POLICY_OUTER_IP_VALID XFLOW_MACSEC_IPSEC_DECRYPT_POLICY_OUTER_IP_VALID /* The subport carries
                                                          IPsec flow and
                                                          ipsec_outer_ip_offset
                                                          is valid. Applicable
                                                          only for Inline Xflow
                                                          Macsec Gen 2. */
#define BCM_XFLOW_MACSEC_IPSEC_DECRYPT_POLICY_ESP_OFFSET_ADJUST XFLOW_MACSEC_IPSEC_DECRYPT_POLICY_ESP_OFFSET_ADJUST /* Adjust
                                                          ipsec_esp_offset based
                                                          on the number of VLAN
                                                          tags or MPLS labels.
                                                          Applicable only for
                                                          Inline Xflow Macsec
                                                          Gen 2. */
#define BCM_XFLOW_MACSEC_IPSEC_DECRYPT_POLICY_OUTER_IP_OFFSET_ADJUST XFLOW_MACSEC_IPSEC_DECRYPT_POLICY_OUTER_IP_OFFSET_ADJUST /* Adjust
                                                          ipsec_outer_ip_offset
                                                          based on the number of
                                                          VLAN tags or MPLS
                                                          labels. Applicable
                                                          only for Inline Xflow
                                                          Macsec Gen 2. */
#define BCM_XFLOW_MACSEC_IPSEC_DECRYPT_POLICY XFLOW_MACSEC_IPSEC_DECRYPT_POLICY /* Identify the subport
                                                          (SecY) as an IPsec
                                                          subport. Needed to
                                                          configure
                                                          ipsec_esp_offset. */

typedef enum bcm_xflow_macsec_tag_validate_e {
    bcmXflowMacsecTagValidateBypassMacsec = xflowMacsecTagValidateBypassMacsec, 
    bcmXflowMacsecTagValidateStrict = xflowMacsecTagValidateStrict, 
    bcmXflowMacsecTagValidateCheckICV = xflowMacsecTagValidateCheckICV, 
    bcmXflowMacsecTagValidateCheckNone = xflowMacsecTagValidateCheckNone, 
    bcmXflowMacsecTagValidateDenyAll = xflowMacsecTagValidateDenyAll, 
    bcmXflowMacsecTagValidateCount = xflowMacsecTagValidateCount 
} bcm_xflow_macsec_tag_validate_t;

typedef xflow_macsec_policy_id_t bcm_xflow_macsec_policy_id_t;

typedef xflow_macsec_policy_info_t bcm_xflow_macsec_decrypt_policy_info_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_xflow_macsec_decrypt_policy_create creates a new decrypt policy
 * based on the configured actions. A policy identifier is returned.
 */
extern int bcm_xflow_macsec_decrypt_policy_create(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_decrypt_policy_info_t *policy_info, 
    bcm_xflow_macsec_policy_id_t *policy_id);

/* 
 * Set/Replace one of the parameters of an existing decrypt policy
 * configuration.
 */
extern int bcm_xflow_macsec_decrypt_policy_set(
    int unit, 
    bcm_xflow_macsec_policy_id_t policy_id, 
    bcm_xflow_macsec_decrypt_policy_info_t *policy_info);

/* Get the decrypt policy configuration given the policy_id. */
extern int bcm_xflow_macsec_decrypt_policy_get(
    int unit, 
    bcm_xflow_macsec_policy_id_t policy_id, 
    bcm_xflow_macsec_decrypt_policy_info_t *policy_info);

/* Destroy the decrypt policy configuration given the policy_id. */
extern int bcm_xflow_macsec_decrypt_policy_destroy(
    int unit, 
    bcm_xflow_macsec_policy_id_t policy_id);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize the decrypt policy data structure. */
extern void bcm_xflow_macsec_decrypt_policy_info_t_init(
    bcm_xflow_macsec_decrypt_policy_info_t *policy_info);

#define BCM_XFLOW_MACSEC_DECRYPT_FLOW_ID_CREATE(id)                  \
                XFLOW_MACSEC_FLOW_ID_CREATE(XFLOW_MACSEC_DECRYPT, id) 
#define BCM_XFLOW_MACSEC_FLOW_WITH_ID       XFLOW_MACSEC_FLOW_WITH_ID /* To be used when flows
                                                          need to be created
                                                          with a particular id */
#define BCM_XFLOW_MACSEC_FLOW_TPID_SEL_0    XFLOW_MACSEC_FLOW_TPID_SEL_0 /* Select the first TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_FLOW_TPID_SEL_1    XFLOW_MACSEC_FLOW_TPID_SEL_1 /* Select the second TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_FLOW_TPID_SEL_2    XFLOW_MACSEC_FLOW_TPID_SEL_2 /* Select the third TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_FLOW_TPID_SEL_3    XFLOW_MACSEC_FLOW_TPID_SEL_3 /* Select the fourth TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_NO_TAGS_NO_LABELS  XFLOW_MACSEC_NO_TAGS_NO_LABELS /* Match no vlan tag or
                                                          no mpls label is
                                                          present. */
#define BCM_XFLOW_MACSEC_1_VLAN_TAG_1_MPLS_LABEL XFLOW_MACSEC_1_VLAN_TAG_1_MPLS_LABEL /* Match 1 VLAN tag or 1
                                                          MPLS label. */
#define BCM_XFLOW_MACSEC_2_VLAN_TAG_2_MPLS_LABEL XFLOW_MACSEC_2_VLAN_TAG_2_MPLS_LABEL /* Match 2 VLAN tags or 2
                                                          MPLS labels. */
#define BCM_XFLOW_MACSEC_3_VLAN_TAG_3_MPLS_LABEL XFLOW_MACSEC_3_VLAN_TAG_3_MPLS_LABEL /* Match 3 VLAN tags or 3
                                                          MPLS labels. */
#define BCM_XFLOW_MACSEC_4_VLAN_TAG_4_MPLS_LABEL XFLOW_MACSEC_4_VLAN_TAG_4_MPLS_LABEL /* Match 4 VLAN tags or 4
                                                          MPLS labels. */
#define BCM_XFLOW_MACSEC_GREATER_4_VLAN_TAG_5_MPLS_LABEL XFLOW_MACSEC_GREATER_4_VLAN_TAG_5_MPLS_LABEL /* Match greater than 4
                                                          VLAN Tags or Greater
                                                          than 5 MPLS labels. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNKNOWN_POLICY XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNKNOWN_POLICY /* Decrypt SVTAG SOP
                                                          error type unkown
                                                          policy. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_TAG_CTRL_PORT XFLOW_MACSEC_DECRYPT_SOP_ERROR_TAG_CTRL_PORT /* Decrypt SVTAG SOP
                                                          error type tag
                                                          controlled port. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNTAG_CTRL_PORT XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNTAG_CTRL_PORT /* Decrypt SVTAG SOP
                                                          error type untagged
                                                          controlled port. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_IPV4_MPLS XFLOW_MACSEC_DECRYPT_SOP_ERROR_IPV4_MPLS /* Decrypt SVTAG SOP
                                                          error type ipv4/MPLS
                                                          error. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_INVALID_SECTAG XFLOW_MACSEC_DECRYPT_SOP_ERROR_INVALID_SECTAG /* Decrypt SVTAG SOP
                                                          error type invalid
                                                          sectag. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNKNOWN_SECURE_CHAN XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNKNOWN_SECURE_CHAN /* Decrypt SVTAG SOP
                                                          error type unkown
                                                          secure channel. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNKNOWN_SECURE_ASSOC XFLOW_MACSEC_DECRYPT_SOP_ERROR_UNKNOWN_SECURE_ASSOC /* Decrypt SVTAG SOP
                                                          error type unkown
                                                          secure association. */
#define BCM_XFLOW_MACSEC_DECRYPT_SOP_ERROR_REPLAY_FAILURE XFLOW_MACSEC_DECRYPT_SOP_ERROR_REPLAY_FAILURE /* Decrypt SVTAG SOP
                                                          error type replay
                                                          error. */

typedef xflow_macsec_flow_id_t bcm_xflow_macsec_flow_id_t;

typedef xflow_macsec_flow_info_mpls_t bcm_xflow_macsec_flow_info_mpls_t;

typedef xflow_macsec_flow_udf_param_t bcm_xflow_macsec_flow_udf_param_t;

typedef xflow_macsec_flow_info_t bcm_xflow_macsec_decrypt_flow_info_t;

typedef enum bcm_xflow_macsec_decrypt_flow_pkt_type_e {
    bcmXflowMacsecDecryptFlowAny = xflowMacsecDecryptFlowAny, 
    bcmXflowMacsecDecryptFlowNonMacsec = xflowMacsecDecryptFlowNonMacsec, 
    bcmXflowMacsecDecryptFlowMacSec = xflowMacsecDecryptFlowMacSec, 
    bcmXflowMacsecDecryptFlowManagement = xflowMacsecDecryptFlowManagement, 
    bcmXflowMacsecDecryptFlowKay = xflowMacsecDecryptFlowKay, 
    bcmXflowMacsecDecryptFlowCount = xflowMacsecDecryptFlowCount 
} bcm_xflow_macsec_decrypt_flow_pkt_type_t;

typedef enum bcm_xflow_macsec_flow_frame_type_e {
    bcmXflowMacsecFlowFrameEII = xflowMacsecFlowFrameEII, 
    bcmXflowMacsecFlowFrameSnap = xflowMacsecFlowFrameSnap, 
    bcmXflowMacsecFlowFrameLlc = xflowMacsecFlowFrameLlc, 
    bcmXflowMacsecFlowFrameMpls = xflowMacsecFlowFrameMpls, 
    bcmXflowMacsecFlowFramePBB = xflowMacsecFlowFramePBB, 
    bcmXflowMacsecFlowFrameVNTag = xflowMacsecFlowFrameVNTag, 
    bcmXflowMacsecFlowFrameETag = xflowMacsecFlowFrameETag, 
    bcmXflowMacsecFlowFrameIPv4 = xflowMacsecFlowFrameIPv4, 
    bcmXflowMacsecFlowFrameUDPIPv4 = xflowMacsecFlowFrameUDPIPv4, 
    bcmXflowMacsecFlowFrameTCPIPv4 = xflowMacsecFlowFrameTCPIPv4, 
    bcmXflowMacsecFlowFrameIPv6 = xflowMacsecFlowFrameIPv6, 
    bcmXflowMacsecFlowFrameUDPIPv6 = xflowMacsecFlowFrameUDPIPv6, 
    bcmXflowMacsecFlowFrameTCPIPv6 = xflowMacsecFlowFrameTCPIPv6, 
    bcmXflowMacsecFlowFrameAny = xflowMacsecFlowFrameAny, 
    bcmXflowMacsecFlowFrameCount = xflowMacsecFlowFrameCount 
} bcm_xflow_macsec_flow_frame_type_t;

typedef enum bcm_xflow_macsec_decrypt_flow_etype_e {
    bcmXflowMacsecFlowEtypeAny = xflowMacsecFlowEtypeAny, 
    bcmXflowMacsecFlowEtypeEII = xflowMacsecFlowEtypeEII, 
    bcmXflowMacsecFlowEtypeSnap = xflowMacsecFlowEtypeSnap, 
    bcmXflowMacsecFlowEtypeLlc = xflowMacsecFlowEtypeLlc, 
    bcmXflowMacsecFlowEtypeMpls = xflowMacsecFlowEtypeMpls, 
    bcmXflowMacsecFlowEtypeCount = xflowMacsecFlowEtypeCount 
} bcm_xflow_macsec_decrypt_flow_etype_t;

typedef enum bcm_xflow_macsec_vlan_mpls_tag_status_e {
    bcmXflowMacsecTagAny = xflowMacsecTagAny, 
    bcmXflowMacsecTagUntaggedVlan = xflowMacsecTagUntaggedVlan, 
    bcmXflowMacsecTagSingleVlan = xflowMacsecTagSingleVlan, 
    bcmXflowMacsecTagDoubleVlan = xflowMacsecTagDoubleVlan, 
    bcmXflowMacsecTagOneMplsLabel = xflowMacsecTagOneMplsLabel, 
    bcmXflowMacsecTagTwoMplsLabel = xflowMacsecTagTwoMplsLabel, 
    bcmXflowMacsecTagThreeMplsLabel = xflowMacsecTagThreeMplsLabel, 
    bcmXflowMacsecTagCount = xflowMacsecTagCount 
} bcm_xflow_macsec_vlan_mpls_tag_status_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * The API bcm_xflow_macsec_decrypt_flow_create creates a flow entry in
 * the decrypt TCAM.
 */
extern int bcm_xflow_macsec_decrypt_flow_create(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_decrypt_flow_info_t *flow_info, 
    int priority, 
    bcm_xflow_macsec_flow_id_t *flow_id);

/* Set/Replace one of the parameters in the flow criteria. */
extern int bcm_xflow_macsec_decrypt_flow_set(
    int unit, 
    bcm_xflow_macsec_flow_id_t flow_id, 
    bcm_xflow_macsec_decrypt_flow_info_t *flow_info, 
    int priority);

/* Get the flow configuration given a flow index. */
extern int bcm_xflow_macsec_decrypt_flow_get(
    int unit, 
    bcm_xflow_macsec_flow_id_t flow_id, 
    bcm_xflow_macsec_decrypt_flow_info_t *flow_info, 
    int *priority);

/* Delete a flow configuration given a flow index. */
extern int bcm_xflow_macsec_decrypt_flow_destroy(
    int unit, 
    bcm_xflow_macsec_flow_id_t flow_id);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize the data structure. */
extern void bcm_xflow_macsec_decrypt_flow_info_t_init(
    bcm_xflow_macsec_decrypt_flow_info_t *flow_info);

#ifndef BCM_HIDE_DISPATCHABLE

/* Enable a flow given the flow index. */
extern int bcm_xflow_macsec_decrypt_flow_enable_set(
    int unit, 
    bcm_xflow_macsec_flow_id_t flow_id, 
    int enable);

/* Get the enable status of a decrypt flow. */
extern int bcm_xflow_macsec_decrypt_flow_enable_get(
    int unit, 
    bcm_xflow_macsec_flow_id_t flow_id, 
    int *enable);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef xflow_macsec_id_t bcm_xflow_macsec_id_t;

typedef xflow_macsec_subport_id_t bcm_xflow_macsec_subport_id_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * The API bcm_xflow_macsec_subport_id_get returns a subport identifier
 * when passed a generic id type.
 */
extern int bcm_xflow_macsec_subport_id_get(
    int unit, 
    bcm_xflow_macsec_id_t id, 
    bcm_xflow_macsec_subport_id_t *macsec_subport_id);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef enum bcm_xflow_macsec_control_e {
    bcmXflowMacsecControlPNThreshold = xflowMacsecControlPNThreshold, 
    bcmXflowMacsecControlXPNThreshold = xflowMacsecControlXPNThreshold, 
    bcmXflowMacsecControlMgmtMTU = xflowMacsecControlMgmtMTU, 
    bcmXflowMacsecControlSVTagEnable = xflowMacsecControlSVTagEnable, 
    bcmXflowMacsecControlPTPDestPortGeneral = xflowMacsecControlPTPDestPortGeneral, 
    bcmXflowMacsecControlPTPDestPortEvent = xflowMacsecControlPTPDestPortEvent, 
    bcmXflowMacsecControlPbbTpidBTag = xflowMacsecControlPbbTpidBTag, 
    bcmXflowMacsecControlPbbTpidITag = xflowMacsecControlPbbTpidITag, 
    bcmXflowMacsecControlEtypeNIV = xflowMacsecControlEtypeNIV, 
    bcmXflowMacsecControlEtypePE = xflowMacsecControlEtypePE, 
    bcmXflowMacsecControlEtypeMgmt0 = xflowMacsecControlEtypeMgmt0, 
    bcmXflowMacsecControlEtypeMgmt1 = xflowMacsecControlEtypeMgmt1, 
    bcmXflowMacsecControlEtypeMgmt2 = xflowMacsecControlEtypeMgmt2, 
    bcmXflowMacsecControlEtypeMgmt3 = xflowMacsecControlEtypeMgmt3, 
    bcmXflowMacsecControlEtypeMgmt4 = xflowMacsecControlEtypeMgmt4, 
    bcmXflowMacsecControlEtypeMgmt5 = xflowMacsecControlEtypeMgmt5, 
    bcmXflowMacsecControlEtypeMgmt6 = xflowMacsecControlEtypeMgmt6, 
    bcmXflowMacsecControlEtypeMgmt7 = xflowMacsecControlEtypeMgmt7, 
    bcmXflowMacsecControlVxLANSecDestPort = xflowMacsecControlVxLANSecDestPort, 
    bcmXflowMacsecControlOutDestPort = xflowMacsecControlOutDestPort, 
    bcmXflowMacsecControlMplsEtype0 = xflowMacsecControlMplsEtype0, 
    bcmXflowMacsecControlMplsEtype1 = xflowMacsecControlMplsEtype1, 
    bcmXflowMacsecControlMplsEtype2 = xflowMacsecControlMplsEtype2, 
    bcmXflowMacsecControlMplsEtype3 = xflowMacsecControlMplsEtype3, 
    bcmXflowMacsecControlSVTagTPIDEtype = xflowMacsecControlSVTagTPIDEtype, 
    bcmXflowMacsecControlEncryptFailCopyToCpu = xflowMacsecControlEncryptFailCopyToCpu, 
    bcmXflowMacsecControlDecryptFailCopyToCpu = xflowMacsecControlDecryptFailCopyToCpu, 
    bcmXflowMacsecControlEncryptFailDrop = xflowMacsecControlEncryptFailDrop, 
    bcmXflowMacsecControlDecryptFailDrop = xflowMacsecControlDecryptFailDrop, 
    bcmXflowMacsecControlSelIpInfoAfterMplsBos = xflowMacsecControlSelIpInfoAfterMplsBos, 
    bcmXflowMacsecControlSvtagSopErrorDrop = xflowMacsecControlSvtagSopErrorDrop, 
    bcmXflowMacsecControlXPNThresholdMask0 = xflowMacsecControlXPNThresholdMask0, 
    bcmXflowMacsecControlXPNThresholdMask1 = xflowMacsecControlXPNThresholdMask1, 
    bcmXflowMacsecControlXPNThresholdMask2 = xflowMacsecControlXPNThresholdMask2, 
    bcmXflowMacsecControlXPNThresholdMask3 = xflowMacsecControlXPNThresholdMask3, 
    bcmXflowMacsecIpsecControlUdpDstPortWithNonEsp = xflowMacsecIpsecControlUdpDstPortWithNonEsp, 
    bcmXflowMacsecIpsecControlUdpDstPortWithoutNonEsp = xflowMacsecIpsecControlUdpDstPortWithoutNonEsp, 
    bcmXflowMacsecIpsecControlNatUdpDstPort = xflowMacsecIpsecControlNatUdpDstPort, 
    bcmXflowMacsecIpsecControlNatUdpSrcPort = xflowMacsecIpsecControlNatUdpSrcPort, 
    bcmXflowMacsecIpsecControlESPUdpDstPort = xflowMacsecIpsecControlESPUdpDstPort, 
    bcmXflowMacsecIpsecControlESPUdpSrcPort = xflowMacsecIpsecControlESPUdpSrcPort, 
    bcmXflowMacsecIpsecControlIncrementPadBytes = xflowMacsecIpsecControlIncrementPadBytes, 
    bcmXflowMacsecIpsecControlNextHopDummy = xflowMacsecIpsecControlNextHopDummy, 
    bcmXflowMacsecControlUdpDestPort = xflowMacsecControlUdpDestPort, 
    bcmXflowMacsecControlTcpDestPort = xflowMacsecControlTcpDestPort, 
    bcmXflowMacsecControlCount = xflowMacsecControlCount 
} bcm_xflow_macsec_control_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_xflow_macsec_control_set sets the value in HW for the macsec
 * control type provided.
 */
extern int bcm_xflow_macsec_control_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_control_t type, 
    uint64 value);

/* 
 * bcm_xflow_macsec_control_get sets the value in HW for the macsec
 * control type provided.
 */
extern int bcm_xflow_macsec_control_get(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_control_t type, 
    uint64 *value);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_XFLOW_MACSEC_STAT_SYNC_DISABLE  XFLOW_MACSEC_STAT_SYNC_DISABLE /* Disable sync when
                                                          using xflow macsec
                                                          Stat APIs. */

typedef enum bcm_xflow_macsec_stat_type_e {
    bcmXflowMacsecStatTypeInvalid = xflowMacsecStatTypeInvalid , 
    bcmXflowMacsecUnctrlPortInOctets = xflowMacsecUnctrlPortInOctets, 
    bcmXflowMacsecUnctrlPortInUcastPkts = xflowMacsecUnctrlPortInUcastPkts, 
    bcmXflowMacsecUnctrlPortInMulticastPkts = xflowMacsecUnctrlPortInMulticastPkts, 
    bcmXflowMacsecUnctrlPortInBroadcastPkts = xflowMacsecUnctrlPortInBroadcastPkts, 
    bcmXflowMacsecUnctrlPortInDiscards = xflowMacsecUnctrlPortInDiscards, 
    bcmXflowMacsecUnctrlPortOutOctets = xflowMacsecUnctrlPortOutOctets, 
    bcmXflowMacsecUnctrlPortOutUcastPkts = xflowMacsecUnctrlPortOutUcastPkts, 
    bcmXflowMacsecUnctrlPortOutMulticastPkts = xflowMacsecUnctrlPortOutMulticastPkts, 
    bcmXflowMacsecUnctrlPortOutBroadcastPkts = xflowMacsecUnctrlPortOutBroadcastPkts, 
    bcmXflowMacsecUnctrlPortOutErrors = xflowMacsecUnctrlPortOutErrors, 
    bcmXflowMacsecCtrlPortInOctets = xflowMacsecCtrlPortInOctets, 
    bcmXflowMacsecCtrlPortInUcastPkts = xflowMacsecCtrlPortInUcastPkts, 
    bcmXflowMacsecCtrlPortInMulticastPkts = xflowMacsecCtrlPortInMulticastPkts, 
    bcmXflowMacsecCtrlPortInBroadcastPkts = xflowMacsecCtrlPortInBroadcastPkts, 
    bcmXflowMacsecCtrlPortInDiscards = xflowMacsecCtrlPortInDiscards, 
    bcmXflowMacsecCtrlPortInErrors = xflowMacsecCtrlPortInErrors, 
    bcmXflowMacsecCtrlPortOutOctets = xflowMacsecCtrlPortOutOctets, 
    bcmXflowMacsecCtrlPortOutUcastPkts = xflowMacsecCtrlPortOutUcastPkts, 
    bcmXflowMacsecCtrlPortOutMulticastPkts = xflowMacsecCtrlPortOutMulticastPkts, 
    bcmXflowMacsecCtrlPortOutBroadcastPkts = xflowMacsecCtrlPortOutBroadcastPkts, 
    bcmXflowMacsecCtrlPortOutErrors = xflowMacsecCtrlPortOutErrors, 
    bcmXflowMacsecSecyStatsTxUntaggedPkts = xflowMacsecSecyStatsTxUntaggedPkts, 
    bcmXflowMacsecSecyStatsTxTooLongPkts = xflowMacsecSecyStatsTxTooLongPkts, 
    bcmXflowMacsecSecyStatsRxUntaggedPkts = xflowMacsecSecyStatsRxUntaggedPkts, 
    bcmXflowMacsecSecyStatsRxNoTagPkts = xflowMacsecSecyStatsRxNoTagPkts, 
    bcmXflowMacsecSecyStatsRxBadTagPkts = xflowMacsecSecyStatsRxBadTagPkts, 
    bcmXflowMacsecSecyStatsRxUnknownSCIPkts = xflowMacsecSecyStatsRxUnknownSCIPkts, 
    bcmXflowMacsecSecyStatsRxNoSCIPkts = xflowMacsecSecyStatsRxNoSCIPkts, 
    bcmXflowMacsecSecyStatsRxOverrunPkts = xflowMacsecSecyStatsRxOverrunPkts, 
    bcmXflowMacsecSecyTxSCStatsProtectedPkts = xflowMacsecSecyTxSCStatsProtectedPkts, 
    bcmXflowMacsecSecyTxSCStatsEncryptedPkts = xflowMacsecSecyTxSCStatsEncryptedPkts, 
    bcmXflowMacsecSecyTxSCStatsOctetsProtected = xflowMacsecSecyTxSCStatsOctetsProtected, 
    bcmXflowMacsecSecyTxSCStatsOctetsEncrypted = xflowMacsecSecyTxSCStatsOctetsEncrypted, 
    bcmXflowMacsecSecyRxSCStatsUnusedSAPkts = xflowMacsecSecyRxSCStatsUnusedSAPkts, 
    bcmXflowMacsecSecyRxSCStatsNotUsingSAPkts = xflowMacsecSecyRxSCStatsNotUsingSAPkts, 
    bcmXflowMacsecSecyRxSCStatsLatePkts = xflowMacsecSecyRxSCStatsLatePkts, 
    bcmXflowMacsecSecyRxSCStatsNotValidPkts = xflowMacsecSecyRxSCStatsNotValidPkts, 
    bcmXflowMacsecSecyRxSCStatsInvalidPkts = xflowMacsecSecyRxSCStatsInvalidPkts, 
    bcmXflowMacsecSecyRxSCStatsDelayedPkts = xflowMacsecSecyRxSCStatsDelayedPkts, 
    bcmXflowMacsecSecyRxSCStatsUncheckedPkts = xflowMacsecSecyRxSCStatsUncheckedPkts, 
    bcmXflowMacsecSecyRxSCStatsOKPkts = xflowMacsecSecyRxSCStatsOKPkts, 
    bcmXflowMacsecSecyRxSCStatsOctetsValidated = xflowMacsecSecyRxSCStatsOctetsValidated, 
    bcmXflowMacsecSecyRxSCStatsOctetsDecrypted = xflowMacsecSecyRxSCStatsOctetsDecrypted, 
    bcmXflowMacsecSecyTxSAStatsProtectedPkts = xflowMacsecSecyTxSAStatsProtectedPkts, 
    bcmXflowMacsecSecyTxSAStatsEncryptedPkts = xflowMacsecSecyTxSAStatsEncryptedPkts, 
    bcmXflowMacsecSecyRxSAStatsUnusedSAPkts = xflowMacsecSecyRxSAStatsUnusedSAPkts, 
    bcmXflowMacsecSecyRxSAStatsNotUsingSAPkts = xflowMacsecSecyRxSAStatsNotUsingSAPkts, 
    bcmXflowMacsecSecyRxSAStatsNotValidPkts = xflowMacsecSecyRxSAStatsNotValidPkts, 
    bcmXflowMacsecSecyRxSAStatsInvalidPkts = xflowMacsecSecyRxSAStatsInvalidPkts, 
    bcmXflowMacsecSecyRxSAStatsOKPkts = xflowMacsecSecyRxSAStatsOKPkts, 
    bcmXflowMacsecInMgmtPkts = xflowMacsecInMgmtPkts, 
    bcmXflowMacsecFlowTcamHitCntr = xflowMacsecFlowTcamHitCntr, 
    bcmXflowMacsecFlowTcamMissCntr = xflowMacsecFlowTcamMissCntr, 
    bcmXflowMacsecScTcamHitCntr = xflowMacsecScTcamHitCntr, 
    bcmXflowMacsecScTcamMissCntr = xflowMacsecScTcamMissCntr, 
    bcmXflowMacsecOutMgmtPkts = xflowMacsecOutMgmtPkts, 
    bcmXflowMacsecInPacketDropCntr = xflowMacsecInPacketDropCntr, 
    bcmXflowMacsecOutPacketDropCntr = xflowMacsecOutPacketDropCntr, 
    bcmXflowMacsecBadOlpHdrCntr = xflowMacsecBadOlpHdrCntr, 
    bcmXflowMacsecBadSvtagHdrCntr = xflowMacsecBadSvtagHdrCntr, 
    bcmXflowMacsecUnctrlPortInKayPkts = xflowMacsecUnctrlPortInKayPkts, 
    bcmXflowMacsecIPsecCtrlPortDummyPkts = xflowMacsecIPsecCtrlPortDummyPkts, 
    bcmXflowMacsecIPsecCtrlPortIPLengthMismatch = xflowMacsecIPsecCtrlPortIPLengthMismatch, 
    bcmXflowMacsecIPsecTxOutErrors = xflowMacsecIPsecTxOutErrors, 
    bcmXflowMacsecIPsecUnctrlPortInIkePkts = xflowMacsecIPsecUnctrlPortInIkePkts, 
    bcmXflowMacsecIPsecSecyRxNoSPIPkts = xflowMacsecIPsecSecyRxNoSPIPkts, 
    bcmXflowMacsecIPsecSecyRxIPFragmentsSetPkts = xflowMacsecIPsecSecyRxIPFragmentsSetPkts, 
    bcmXflowMacsecIPSecSecyRxIllegalNxtHdrPkts = xflowMacsecIPSecSecyRxIllegalNxtHdrPkts, 
    bcmXflowMacsecIPSecRxNoSAPkts = xflowMacsecIPSecRxNoSAPkts, 
    bcmXflowMacsecIPSecRxSADummyPkts = xflowMacsecIPSecRxSADummyPkts, 
    bcmXflowMacsecIPSecRxSAPadMismatchPkts = xflowMacsecIPSecRxSAPadMismatchPkts, 
    bcmXflowMacsecStatTypeCount = xflowMacsecStatTypeCount 
} bcm_xflow_macsec_stat_type_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * The API bcm_xflow_macsec_stat_get gets the counter value corresponding
 * to the generic id and stat_type provided.
 */
extern int bcm_xflow_macsec_stat_get(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_id_t id, 
    bcm_xflow_macsec_stat_type_t  stat_type, 
    uint64 *value);

/* 
 * The API bcm_xflow_macsec_stat_set sets the counter value corresponding
 * to the generic id and stat_type provided.
 */
extern int bcm_xflow_macsec_stat_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_id_t id, 
    bcm_xflow_macsec_stat_type_t  stat_type, 
    uint64 value);

/* 
 * The API bcm_xflow_macsec_stat_multi_get gets an array of counter
 * values.
 */
extern int bcm_xflow_macsec_stat_multi_get(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_id_t id, 
    uint32 num_stats, 
    bcm_xflow_macsec_stat_type_t  *stat_type_array, 
    uint64 *value_array);

/* 
 * The API bcm_xflow_macsec_stat_multi_set gets an array of counter
 * values.
 */
extern int bcm_xflow_macsec_stat_multi_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_id_t id, 
    uint32 num_stats, 
    bcm_xflow_macsec_stat_type_t  *stat_type_array, 
    uint64 *value_array);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_XFLOW_MACSEC_MATCH_TPID_SEL_0   XFLOW_MACSEC_MATCH_TPID_SEL_0 /* Select the first TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_MATCH_TPID_SEL_1   XFLOW_MACSEC_MATCH_TPID_SEL_1 /* Select the second TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_MATCH_TPID_SEL_2   XFLOW_MACSEC_MATCH_TPID_SEL_2 /* Select the third TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_MATCH_TPID_SEL_3   XFLOW_MACSEC_MATCH_TPID_SEL_3 /* Select the fourth TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_MATCH_TPID_SEL_4   XFLOW_MACSEC_MATCH_TPID_SEL_4 /* Select the fourth TPID
                                                          to be matched. */
#define BCM_XFLOW_MACSEC_VLAN_TPID_MAX      XFLOW_MACSEC_VLAN_TPID_MAX /* Total TPIDs
                                                          configurable */

typedef xflow_macsec_vlan_tpid_t bcm_xflow_macsec_vlan_tpid_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * The API bcm_xflow_macsec_vlan_tpid_array_set sets the four VLAN TPID
 * for decrypt flow.
 */
extern int bcm_xflow_macsec_vlan_tpid_array_set(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_vlan_tpid_t *vlan_tpid);

/* 
 * The API bcm_xflow_macsec_vlan_tpid_array_get gets the four VLAN TPID
 * for decrypt flow.
 */
extern int bcm_xflow_macsec_vlan_tpid_array_get(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_vlan_tpid_t *vlan_tpid);

/* 
 * The API bcm_xflow_macsec_vlan_tpid_array_index_get retrieves the index
 * corresponding to a TPID value.
 */
extern int bcm_xflow_macsec_vlan_tpid_array_index_get(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    uint32 vlan_tpid, 
    uint8 *tpid_index_sel);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_XFLOW_MACSEC_MTU_WITH_ID    XFLOW_MACSEC_MTU_WITH_ID /* The mtu_index is provided
                                                      as an input. */

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * The API bcm_xflow_macsec_mtu_set sets the MTU for encrypt or decrypt
 * flow.
 */
extern int bcm_xflow_macsec_mtu_set(
    int unit, 
    int flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    uint32 mtu, 
    bcm_xflow_macsec_mtu_t *mtu_sel);

/* 
 * The API bcm_xflow_macsec_mtu_get gets the MTU for encrypt or decrypt
 * flow.
 */
extern int bcm_xflow_macsec_mtu_get(
    int unit, 
    int flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_mtu_t mtu_sel, 
    uint32 *mtu);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_XFLOW_MACSEC_ETHERTYPE_WITH_ID  XFLOW_MACSEC_ETHERTYPE_WITH_ID /* The sectag_etype_index
                                                          is provided as input. */

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * The API bcm_xflow_macsec_sectag_etype_set sets the sectag EtherType
 * for encrypt and decrypt flow.
 */
extern int bcm_xflow_macsec_sectag_etype_set(
    int unit, 
    int flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    uint32 sectag_etype, 
    bcm_xflow_macsec_sectag_ethertype_t *sectag_etype_sel);

/* 
 * The API bcm_xflow_macsec_sectag_etype_get gets the sectag EtherType
 * for encrypt and decrypt flow.
 */
extern int bcm_xflow_macsec_sectag_etype_get(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_sectag_ethertype_t sectag_etype_sel, 
    uint32 *sectag_etype);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef enum bcm_xflow_macsec_event_e {
    bcmXflowMacsecEventSASoftExpire = xflowMacsecEventSASoftExpire, /* SA Soft Expire event. This indicates
                                           that the PN for a given SA has hit
                                           the threshold value configured. The
                                           index id has to be typecasted to
                                           bcm_xflow_macsec_secure_assoc_id_t. */
    bcmXflowMacsecEventSAExpire = xflowMacsecEventSAExpire, /* SA Expire event. This indicates that
                                           the PN for a given SA has hit the
                                           maximum value possible. For AES
                                           algorithm, this is 2^32. For AES XPN
                                           algorithm, this is 2^64. The index id
                                           has to be typecasted to
                                           bcm_xflow_macsec_secure_assoc_id_t. */
    bcmXflowMacsecEventSAMinExpire = xflowMacsecEventSAMinExpire, /* SA PN/XPN Min Expire event. This
                                           indicates that the incoming SA PN/XPN
                                           is outside the SC
                                           replay_protect_window value. The
                                           index id has to be typecasted to
                                           bcm_xflow_macsec_secure_assoc_id_t.
                                           Applicable only for decrypt case. */
    bcmXflowMacsecEventICVFailure = xflowMacsecEventICVFailure, /* ICV failure event.The index id has to
                                           be typecasted to
                                           bcm_xflow_macsec_secure_assoc_id_t.Applicable
                                           only for decrypt case.Not applicable
                                           for inline xflow-macsec. */
    bcmXflowMacsecEventCount = xflowMacsecEventCount 
} bcm_xflow_macsec_event_t;

/* 
 * Callback function to notify Xflow MACsec events. The flags specify
 * whether the callback is for encrypt or decrypt. The index id should be
 * typecasted based on the event.
 */
typedef int (*bcm_xflow_macsec_event_cb)(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_event_t event, 
    bcm_xflow_macsec_id_t id, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * Xflow Macsec callback registration API. The callback function will be
 * invoked when an event occurs.
 */
extern int bcm_xflow_macsec_event_register(
    int unit, 
    bcm_xflow_macsec_event_cb cb, 
    void *user_data);

/* Xflow Macsec callback deregistration API. */
extern int bcm_xflow_macsec_event_deregister(
    int unit, 
    bcm_xflow_macsec_event_cb cb);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef xflow_macsec_port_info_t bcm_xflow_macsec_port_info_t;

typedef xflow_macsec_mac_addr_info_t bcm_xflow_macsec_mac_addr_info_t;

typedef xflow_macsec_port_map_info_t bcm_xflow_macsec_port_map_info_t;

typedef xflow_macsec_handle_info_t bcm_xflow_macsec_handle_info_t;

typedef xflow_macsec_svtag_cpu_flex_map_param_t bcm_xflow_macsec_svtag_cpu_flex_map_param_t;

typedef xflow_macsec_svtag_cpu_flex_map_info_t bcm_xflow_macsec_svtag_cpu_flex_map_info_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Create a logical MACSec handle */
extern int bcm_xflow_macsec_handle_create(
    int unit, 
    bcm_xflow_macsec_handle_info_t *handle_info, 
    bcm_xflow_macsec_id_t *id);

/* 
 * Given MACsec logical handle, return its info (such as HW index,
 * macsec_id, etc...)
 */
extern int bcm_xflow_macsec_handle_info_get(
    int unit, 
    bcm_xflow_macsec_id_t id, 
    bcm_xflow_macsec_handle_info_t *handle_info);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize the data structure. */
extern void bcm_xflow_macsec_port_info_t_init(
    bcm_xflow_macsec_port_info_t *port_info);

#ifndef BCM_HIDE_DISPATCHABLE

/* Set the MACSEC info on a given port. */
extern int bcm_xflow_macsec_port_info_set(
    int unit, 
    bcm_port_t port, 
    bcm_xflow_macsec_port_info_t *port_info);

/* Get the MACSEC info on a given port. */
extern int bcm_xflow_macsec_port_info_get(
    int unit, 
    bcm_port_t port, 
    bcm_xflow_macsec_port_info_t *port_info);

/* 
 * Get the Port Mapping info of a gport : Macsec instance id and Macsec
 * port associated with this Gport.
 */
extern int bcm_xflow_macsec_port_map_info_get(
    int unit, 
    uint32 flags, 
    bcm_gport_t gport, 
    bcm_xflow_macsec_port_map_info_t *port_map_info);

/* 
 * Get all the instances along with the member port bitmap. If input
 * parameter instance_max = 0, return in the output parameter
 * instance_count will be the total number of instances
 */
extern int bcm_xflow_macsec_instance_pbmp_map_get(
    int unit, 
    int instance_max, 
    bcm_xflow_macsec_instance_pbmp_t *instance_pbmp_map, 
    int *instance_count);

#endif /* BCM_HIDE_DISPATCHABLE */

/* 
 * Mac address control enums. Applicable only for Inline Xflow Macsec
 * unless mentioned otherwise. Applicable only for the decrypt case.
 */
typedef enum bcm_xflow_macsec_mac_addr_control_e {
    bcmXflowMacsecMgmtDstMac0 = xflowMacsecMgmtDstMac0, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMac1 = xflowMacsecMgmtDstMac1, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMac2 = xflowMacsecMgmtDstMac2, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMac3 = xflowMacsecMgmtDstMac3, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMac4 = xflowMacsecMgmtDstMac4, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMac5 = xflowMacsecMgmtDstMac5, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMac6 = xflowMacsecMgmtDstMac6, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMac7 = xflowMacsecMgmtDstMac7, /* MACDA to be classified as a
                                           rudimentary management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.ethertype is
                                           ignored. */
    bcmXflowMacsecMgmtDstMacRangeLow = xflowMacsecMgmtDstMacRangeLow, /* Lower limit in a range of MACDA to be
                                           classified as a management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.value is
                                           ignored. */
    bcmXflowMacsecMgmtDstMacRangeHigh = xflowMacsecMgmtDstMacRangeHigh, /* Higher limit in a range of MACDA to
                                           be classified as a management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA.
                                           Argument mac_addr_info.value is
                                           ignored. */
    bcmXflowMacsecMgmtDstMacEthertype0 = xflowMacsecMgmtDstMacEthertype0, /* First set of MACDA and Ethertype to
                                           be classified as a management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA
                                           Argument mac_addr_info.ethertype
                                           specifies the ethertype. */
    bcmXflowMacsecMgmtDstMacEthertype1 = xflowMacsecMgmtDstMacEthertype1, /* Second set of MACDA and Ethertype to
                                           be classified as a management packet.
                                           Argument mac_addr_info.mac_addr
                                           specifies the MACDA
                                           Argument mac_addr_info.ethertype
                                           specifies the ethertype. */
    bcmXflowMacsecStationDstMac = xflowMacsecStationDstMac, /* Station mac addressed to be used when
                                           a dropped packet is to be copied to
                                           cpu.
                                           Argument mac_addr_info.mac_addr
                                           specifies the station MACDA.
                                           Argument mac_addr_info.value is
                                           ignored. Not applicable to Inline
                                           Xflow Macsec. */
    bcmXflowMacsecMacAddrControlCount = xflowMacsecMacAddrControlCount /* Max value. */
} bcm_xflow_macsec_mac_addr_control_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_xflow_macsec_mac_addr_control_set sets the value in HW for the
 * macsec control type provided.
 */
extern int bcm_xflow_macsec_mac_addr_control_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_mac_addr_control_t control_type, 
    bcm_xflow_macsec_mac_addr_info_t *control_info);

/* 
 * bcm_xflow_macsec_mac_addr_control_get gets the value from HW for the
 * macsec control type provided.
 */
extern int bcm_xflow_macsec_mac_addr_control_get(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_mac_addr_control_t control_type, 
    bcm_xflow_macsec_mac_addr_info_t *control_info);

#endif /* BCM_HIDE_DISPATCHABLE */

/* 
 * Port control enums. Applicable only for Inline Xflow Macsec unless
 * mentioned otherwise. Applicable only for the decrypt scenario unless
 * mentioned otherwise.
 */
typedef enum bcm_xflow_macsec_port_control_e {
    bcmXflowMacsecPortSectagEtypeSel = xflowMacsecPortSectagEtypeSel, /* Select from the configured
                                           Ethertypes. */
    bcmXflowMacsecPortSectagVersion = xflowMacsecPortSectagVersion, /* Configure Macsec version for frame
                                           validation. */
    bcmXflowMacsecPortSectagRuleEnable = xflowMacsecPortSectagRuleEnable, /* Enable validation rules for ingress
                                           matching. Select among the following
                                           flags:
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_VERSION_INVALID
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_MAX
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_UNSET
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_MISMATCH
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_E0_C1
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_ES1_SC1
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SC1_SCB1
                                           BCM_XFLOW_MACSEC_VALIDATE_SECTAG_PN0 */
    bcmXflowMacsecPortTPIDEnable = xflowMacsecPortTPIDEnable, /* Enable configured TPID values. Select
                                           among the following flags:
                                           BCM_XFLOW_MACSEC_MATCH_TPID_SEL_0
                                           BCM_XFLOW_MACSEC_MATCH_TPID_SEL_1
                                           BCM_XFLOW_MACSEC_MATCH_TPID_SEL_2
                                           BCM_XFLOW_MACSEC_MATCH_TPID_SEL_3
                                           BCM_XFLOW_MACSEC_MATCH_TPID_SEL_4 */
    bcmXflowMacsecPortPBBEnable = xflowMacsecPortPBBEnable, /* Per port enable for PBB packet
                                           identification. */
    bcmXflowMacsecPortMPLSEnable = xflowMacsecPortMPLSEnable, /* Enable configured MPLS Ethertype.
                                           Select among the following flags:
                                           BCM_XFLOW_MACSEC_MPLS_ETYPE_0
                                           BCM_XFLOW_MACSEC_MPLS_ETYPE_1
                                           BCM_XFLOW_MACSEC_MPLS_ETYPE_2
                                           BCM_XFLOW_MACSEC_MPLS_ETYPE_3 */
    bcmXflowMacsecPortIPv4EtypeEnable = xflowMacsecPortIPv4EtypeEnable, /* Per port enable for IPv4 ethertype. */
    bcmXflowMacsecPortIPv6EtypeEnable = xflowMacsecPortIPv6EtypeEnable, /* Per port enable for IPv6 ethertype. */
    bcmXflowMacsecPortPTPEtypeEnable = xflowMacsecPortPTPEtypeEnable, /* Per port enable for PTP ethertype. */
    bcmXflowMacsecPortNIVEtypeEnable = xflowMacsecPortNIVEtypeEnable, /* Per port enable for VNTAG ethertype. */
    bcmXflowMacsecPortPEEtypeEnable = xflowMacsecPortPEEtypeEnable, /* Per port enable for PE/ETAG
                                           ethertype. */
    bcmXflowMacsecPortUDPEnable = xflowMacsecPortUDPEnable, /* Per port enable for UDP protocol
                                           packet. */
    bcmXflowMacsecPortTCPEnable = xflowMacsecPortTCPEnable, /* Per port enable for TCP protocol
                                           packet. */
    bcmXflowMacsecPortPTPDestPortEnable = xflowMacsecPortPTPDestPortEnable, /* Per port enable for PTP destination
                                           port number match. */
    bcmXflowMacsecPortPTPMatchRuleEnable = xflowMacsecPortPTPMatchRuleEnable, /* Per-port enable for PTP packet
                                           identification types. Select among
                                           the following flags.
                                           BCM_XFLOW_MACSEC_MATCH_PTP_UNTAGGED
                                           BCM_XFLOW_MACSEC_MATCH_PTP_ONE_VLAN
                                           BCM_XFLOW_MACSEC_MATCH_PTP_UDP_IPV4
                                           BCM_XFLOW_MACSEC_MATCH_PTP_UDP_IPV6 */
    bcmXflowMacsecPortSectagAfterIPv4Enable = xflowMacsecPortSectagAfterIPv4Enable, /* If enabled, 2B after IPV4 header is
                                           matched with SecTag Ethertype.
                                           Otherwise, Sectag EtherType is not
                                           checked after IPv4 header. */
    bcmXflowMacsecPortSectagAfterIPv6Enable = xflowMacsecPortSectagAfterIPv6Enable, /* If enabled, 2B after IPV6 header is
                                           matched with SecTag Ethertype.
                                           Otherwise, Sectag EtherType is not
                                           checked after IPv6 header. */
    bcmXflowMacsecPortSectagAfterTCPEnable = xflowMacsecPortSectagAfterTCPEnable, /* If enabled, 2B after TCP header is
                                           matched with SecTag Ethertype.
                                           Otherwise, Sectag EtherType is not
                                           checked after TCP header. */
    bcmXflowMacsecPortSectagAfterUDPEnable = xflowMacsecPortSectagAfterUDPEnable, /* If enabled, 2B after UDP header is
                                           matched with SecTag Ethertype.
                                           Otherwise, Sectag EtherType is not
                                           checked after UDP header. */
    bcmXflowMacsecPortIPv4ChecksumEnable = xflowMacsecPortIPv4ChecksumEnable, /* If enabled, IPv4 checksum is checked
                                           for secure data packets (MACSEC
                                           packets). If failed, the packet is
                                           purged. Otherwise, IPV4 checksum is
                                           not checked. */
    bcmXflowMacsecPortVxLANIpv6UDPVNIMatchEnable = xflowMacsecPortVxLANIpv6UDPVNIMatchEnable, /* If enabled and if UDP destination
                                           port matches the programmed value,
                                           IPv6 with UDP packet Flags and VNI
                                           information is extracted and used for
                                           decrypt flow match. */
    bcmXflowMacsecPortMTU = xflowMacsecPortMTU, /* The value of per port MTU (maximum
                                           transmission unit) for ingress
                                           management, KaY and SP TCAM Miss
                                           packets. A packet will be marked as
                                           an error packet if its (ingress) size
                                           is greater than MTU. This value
                                           doesn't include the CRC bytes. This
                                           must be set to a value less than or
                                           equal to 2^14-1 minus 16 and greater
                                           than or equal to 196. */
    bcmXflowMacsecPortMgmtPktRulesEnable = xflowMacsecPortMgmtPktRulesEnable, /* Enable the following rules used in
                                           rudimentary management packet
                                           detection. Select among the following
                                           flags.
                                           BCM_XFLOW_MACSEC_MGMT_DEST_MAC_0X0180C200000
                                           BCM_XFLOW_MACSEC_MGMT_DEST_MAC_0X01000CCCCCCC
                                           BCM_XFLOW_MACSEC_MGMT_DEST_MAC[0-7]
                                           BCM_XFLOW_MACSEC_MGMT_ETYPE[0-7]
                                           BCM_XFLOW_MACSEC_MGMT_DEST_MAC_RANGE
                                           BCM_XFLOW_MACSEC_MGMT_DEST_MAC_ETYPE[0-1]
                                           BCM_XFLOW_MACSEC_MGMT_E1_C0
                                           BCM_XFLOW_MACSEC_MGMT_PTP
                                           BCM_XFLOW_MACSEC_MGMT_IPSEC_UDP_DST_PORT_WITH_NON_ESP
                                           BCM_XFLOW_MACSEC_MGMT_IPSEC_UDP_DST_PORT_WITHOUT_NON_ESP
                                           BCM_XFLOW_MACSEC_MGMT_IPSEC_UDP_NAT_PORT */
    bcmXflowMacsecPortMgmtDefaultSubPort = xflowMacsecPortMgmtDefaultSubPort, /* The default sub port assigned if
                                           management packet is detected by the
                                           enabled rules for the port. Subport
                                           ID is passed in the value argument. */
    bcmXflowMacsecPortBasedScEnable = xflowMacsecPortBasedScEnable, /* Relevant for Encrypt Only: If working
                                           in this mode for this port, the SC
                                           Index is derived by the core and is
                                           equal to the egress port number. */
    bcmXflowMacsecPortEnable = xflowMacsecPortEnable, /* Used for enabling/disabling MACSEC to
                                           this logical port. */
    bcmXflowMacsecIPsecPortEsp = xflowMacsecIPsecPortEsp, /* Enable ESP IPsec packet detection. */
    bcmXflowMacsecIPsecPortEspUdp = xflowMacsecIPsecPortEspUdp, /* Enable UDP encapsulated ESP IPsec
                                           packet detection. */
    bcmXflowMacsecIPsecPortEspSnZero = xflowMacsecIPsecPortEspSnZero, /* Enable to allow sequence number (SN)
                                           value in ESP to be 0. */
    bcmXflowMacsecIPsecPortEspSpiZero = xflowMacsecIPsecPortEspSpiZero, /* Enable to allow security parameter
                                           index (SPI) field in ESP to be 0. */
    bcmXflowMacsecIPsecPortEspSpi1To255 = xflowMacsecIPsecPortEspSpi1To255, /* Enable to allow SPI field in ESP to
                                           be in the range 1 to 255. */
    bcmXflowMacsecIPsecPortIPv4Fragment = xflowMacsecIPsecPortIPv4Fragment, /* Enable to allow IPsec IPv4 fragment. */
    bcmXflowMacsecIPsecPortEspUdpSrcPort = xflowMacsecIPsecPortEspUdpSrcPort, /* Enable UDP encapsulated ESP packet
                                           detection using UDP source port. */
    bcmXflowMacsecIPsecPortNatKeepalive = xflowMacsecIPsecPortNatKeepalive, /* Enable NAT keepalive packet
                                           detection. */
    bcmXflowMacsecPortCount = xflowMacsecPortCount /* Maximum enum value. */
} bcm_xflow_macsec_port_control_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * Set the per port parameters for Xflow Macsec. This API should be
 * called only in the absence of traffic.
 */
extern int bcm_xflow_macsec_port_control_set(
    int unit, 
    uint32 flags, 
    bcm_gport_t gport, 
    bcm_xflow_macsec_port_control_t control_type, 
    bcm_xflow_macsec_port_info_t *value);

/* Get the per port parameters for Xflow Macsec. */
extern int bcm_xflow_macsec_port_control_get(
    int unit, 
    uint32 flags, 
    bcm_gport_t gport, 
    bcm_xflow_macsec_port_control_t control_type, 
    bcm_xflow_macsec_port_info_t *value);

/* 
 * Get the default Policy ID if flow lookup results in a TCAM miss. Valid
 * only if xflow_macsec_decrypt_flow_default_policy_enable config
 * property is enabled. This API is valid only for the Inline Xflow
 * Macsec architecture.
 */
extern int bcm_xflow_macsec_decrypt_flow_default_policy_get(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_policy_id_t *policy_id);

/* set an entry in the SVTAG CPU Flex Map TCAM */
extern int bcm_xflow_macsec_decrypt_svtag_cpu_flex_map_set(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    int index, 
    bcm_xflow_macsec_svtag_cpu_flex_map_info_t *svtag_cpu_flex_map_info);

/* set an entry in the SVTAG CPU Flex Map TCAM */
extern int bcm_xflow_macsec_decrypt_svtag_cpu_flex_map_get(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    int index, 
    bcm_xflow_macsec_svtag_cpu_flex_map_info_t *svtag_cpu_flex_map_info);

/* Enable the CPU Flex Map rule. */
extern int bcm_xflow_macsec_decrypt_svtag_cpu_flex_map_enable_set(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    int index, 
    int enable);

/* Check if the CPU Flex Map rule is enabled. */
extern int bcm_xflow_macsec_decrypt_svtag_cpu_flex_map_enable_get(
    int unit, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    int index, 
    int *enable);

/* 
 * Get the reserved secure chan ID for the port configured in port mode.
 * Valid only if config bcm_xflow_macsec_encrypt_phy_port_based_macsec is
 * set for the port.
 */
extern int bcm_xflow_macsec_port_rsvd_secure_chan_get(
    int unit, 
    uint32 flags, 
    bcm_gport_t port, 
    bcm_xflow_macsec_secure_chan_id_t *chan_id);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_XFLOW_MACSEC_SUBPORT_NUM_GET(subport_id)                  \
                XFLOW_MACSEC_SUBPORT_NUM_GET(subport_id) 
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_VERSION_INVALID XFLOW_MACSEC_VALIDATE_SECTAG_VERSION_INVALID /* Invalidate packet if
                                                          SECTAG.TCI.V !=
                                                          (configural value).
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_MAX XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_MAX /* Invalidate packet if
                                                          Short_length rcv > 47.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_UNSET XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_UNSET /* Invalidate packet if
                                                          short length not set
                                                          but a short packet has
                                                          arrived. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_MISMATCH XFLOW_MACSEC_VALIDATE_SECTAG_SHORT_LEN_MISMATCH /* Invalidate packet if
                                                          short length does not
                                                          match the received
                                                          packet length. Valid
                                                          only for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_E0_C1 XFLOW_MACSEC_VALIDATE_SECTAG_E0_C1 /* Invalidate packet if
                                                          SECTAG.TCI.E = 0 and
                                                          SECTAG.TCI.C = 1.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_ES1_SC1 XFLOW_MACSEC_VALIDATE_SECTAG_ES1_SC1 /* Invalidate packet if
                                                          SECTAG.TCI.ES = 1 and
                                                          SECTAG.TCI.SC = 1.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_SC1_SCB1 XFLOW_MACSEC_VALIDATE_SECTAG_SC1_SCB1 /* Invalidate packet if
                                                          SECTAG.TCI.SC = 1 and
                                                          SECTAG.TCI.SCB = 1.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_VALIDATE_SECTAG_PN0 XFLOW_MACSEC_VALIDATE_SECTAG_PN0 /* Invalidate packet if
                                                          SECTAG.PN = 0. Valid
                                                          only for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MPLS_ETYPE_0       XFLOW_MACSEC_MPLS_ETYPE_0 /* Enable first
                                                          configured MPLS
                                                          ethertype. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MPLS_ETYPE_1       XFLOW_MACSEC_MPLS_ETYPE_1 /* Enable second
                                                          configured MPLS
                                                          ethertype. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MPLS_ETYPE_2       XFLOW_MACSEC_MPLS_ETYPE_2 /* Enable third
                                                          configured MPLS
                                                          ethertype. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MPLS_ETYPE_3       XFLOW_MACSEC_MPLS_ETYPE_3 /* Enable fourth
                                                          configured MPLS
                                                          ethertype. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MATCH_PTP_UNTAGGED XFLOW_MACSEC_MATCH_PTP_UNTAGGED /* Enable untagged PTP
                                                          EtherType match. Valid
                                                          only for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MATCH_PTP_ONE_VLAN XFLOW_MACSEC_MATCH_PTP_ONE_VLAN /* Enable PTP EtherType
                                                          match with 1 VLAN.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MATCH_PTP_UDP_IPV4 XFLOW_MACSEC_MATCH_PTP_UDP_IPV4 /* Enable PTP packet with
                                                          UDP over IPV4 match.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MATCH_PTP_UDP_IPV6 XFLOW_MACSEC_MATCH_PTP_UDP_IPV6 /* Enable PTP packet with
                                                          UDP over IPV6 match.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC_0X0180C200000 XFLOW_MACSEC_MGMT_DEST_MAC_0X0180C200000 /* Match as management
                                                          packet when
                                                          MAC_DA[47:4] ==
                                                          0x0180_c200_000. Valid
                                                          only for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC_0X01000CCCCCCC XFLOW_MACSEC_MGMT_DEST_MAC_0X01000CCCCCCC /* Match as management
                                                          packet when MAC_DA ==
                                                          0x0100_0ccc_cccc.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC0     XFLOW_MACSEC_MGMT_DEST_MAC0 /* Enable rule to match
                                                          as management packet
                                                          when the first MAC_DA
                                                          matches. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC1     XFLOW_MACSEC_MGMT_DEST_MAC1 /* Enable rule to match
                                                          as management packet
                                                          when the second MAC_DA
                                                          matches. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC2     XFLOW_MACSEC_MGMT_DEST_MAC2 /* Enable rule to match
                                                          as management packet
                                                          when the third MAC_DA
                                                          matches. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC3     XFLOW_MACSEC_MGMT_DEST_MAC3 /* Enable rule to match
                                                          as management packet
                                                          when the fourth MAC_DA
                                                          matches. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC4     XFLOW_MACSEC_MGMT_DEST_MAC4 /* Enable rule to match
                                                          as management packet
                                                          when the fifth MAC_DA
                                                          matches. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC5     XFLOW_MACSEC_MGMT_DEST_MAC5 /* Enable rule to match
                                                          as management packet
                                                          when the sixth MAC_DA
                                                          matches. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC6     XFLOW_MACSEC_MGMT_DEST_MAC6 /* Enable rule to match
                                                          as management packet
                                                          when the seventh
                                                          MAC_DA matches. Valid
                                                          only for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC7     XFLOW_MACSEC_MGMT_DEST_MAC7 /* Enable rule to match
                                                          as management packet
                                                          when the eighth MAC_DA
                                                          matches. Valid only
                                                          for Inline Xflow
                                                          Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE0        XFLOW_MACSEC_MGMT_ETYPE0 /* Enable rule to match
                                                          as management packet
                                                          when the first
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE1        XFLOW_MACSEC_MGMT_ETYPE1 /* Enable rule to match
                                                          as management packet
                                                          when the second
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE2        XFLOW_MACSEC_MGMT_ETYPE2 /* Enable rule to match
                                                          as management packet
                                                          when the third
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE3        XFLOW_MACSEC_MGMT_ETYPE3 /* Enable rule to match
                                                          as management packet
                                                          when the fourth
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE4        XFLOW_MACSEC_MGMT_ETYPE4 /* Enable rule to match
                                                          as management packet
                                                          when the fifth
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE5        XFLOW_MACSEC_MGMT_ETYPE5 /* Enable rule to match
                                                          as management packet
                                                          when the sixth
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE6        XFLOW_MACSEC_MGMT_ETYPE6 /* Enable rule to match
                                                          as management packet
                                                          when the seventh
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_ETYPE7        XFLOW_MACSEC_MGMT_ETYPE7 /* Enable rule to match
                                                          as management packet
                                                          when the eighth
                                                          Ethertype matches.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC_RANGE XFLOW_MACSEC_MGMT_DEST_MAC_RANGE /* Enable rule to match
                                                          as management packet
                                                          when the programmable
                                                          MAC_DA address range
                                                          match. Valid only for
                                                          Inline Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC_ETYPE0 XFLOW_MACSEC_MGMT_DEST_MAC_ETYPE0 /* Enable rule to match
                                                          as management packet
                                                          when the first set of
                                                          programmable MAC_DA
                                                          and EtherType match.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_DEST_MAC_ETYPE1 XFLOW_MACSEC_MGMT_DEST_MAC_ETYPE1 /* Enable rule to match
                                                          as management packet
                                                          when the second set of
                                                          programmable MAC_DA
                                                          and EtherType match.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_E1_C0         XFLOW_MACSEC_MGMT_E1_C0 /* Enable rule to match
                                                          as management packet
                                                          when SECTAG is present
                                                          and SECTAG.TCI.E = 1
                                                          and SECTAG.TCI.C = 0.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_PTP           XFLOW_MACSEC_MGMT_PTP /* Enable rule to match
                                                          PTP/IEEE 1588 packet
                                                          as management packet.
                                                          Valid only for Inline
                                                          Xflow Macsec. */
#define BCM_XFLOW_MACSEC_MGMT_IPSEC_UDP_DST_PORT_WITH_NON_ESP XFLOW_MACSEC_MGMT_IPSEC_UDP_DST_PORT_WITH_NON_ESP /* Enable to allow IPsec
                                                          IKE packet having UDP
                                                          destination port
                                                          provided by the
                                                          control
                                                          xflowMacsecIpsecControlUdpDstPortWithNonEsp,
                                                          when the 4 byteNON-ESP
                                                          marker after UDP
                                                          header is 0. Valid
                                                          only for Inline Xflow
                                                          Macsec (IPSec only). */
#define BCM_XFLOW_MACSEC_MGMT_IPSEC_UDP_DST_PORT_WITHOUT_NON_ESP XFLOW_MACSEC_MGMT_IPSEC_UDP_DST_PORT_WITHOUT_NON_ESP /* Enable to allow IPsec
                                                          IKE packet having UDP
                                                          destination port
                                                          provided by the
                                                          control
                                                          xflowMacsecIpsecControlUdpDstPortWithoutNonEsp,
                                                          when the 4 byte
                                                          NON-ESP marker is not
                                                          present. Valid only
                                                          for Inline Xflow
                                                          Macsec (IPSec only). */
#define BCM_XFLOW_MACSEC_MGMT_IPSEC_UDP_NAT_PORT XFLOW_MACSEC_MGMT_IPSEC_UDP_NAT_PORT /* Enable to allow IPsec
                                                          NAT keep-alive packet
                                                          with UDP destination
                                                          port
                                                          xflowMacsecIpsecControlNatUdpDstPort,
                                                          1 byte payload value
                                                          0xFF and optional UDP
                                                          source port
                                                          xflowMacsecIpsecControlNatUdpSrcPort
                                                          and per-port control
                                                          xflowMacsecIPsecPortNatKeepalive
                                                          set. Valid only for
                                                          Inline Xflow Macsec
                                                          (IPSec only). */

typedef xflow_macsec_ipsec_sc_match_action_info_t bcm_xflow_macsec_ipsec_sc_match_action_info_t;

typedef xflow_macsec_ipsec_secure_chan_info_t bcm_xflow_macsec_ipsec_secure_chan_info_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_xflow_macsec_ipsec_secure_chan_create installs a new IPsec
 * security channel entry. It returns the secure channel ID.
 */
extern int bcm_xflow_macsec_ipsec_secure_chan_create(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_ipsec_secure_chan_info_t *chan_info, 
    bcm_xflow_macsec_secure_chan_id_t *chan_id);

/* Set IPsec configuration for the specified port. */
extern int bcm_xflow_macsec_ipsec_secure_chan_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_ipsec_secure_chan_info_t *chan_info);

/* Get the IPsec security channel configuration for a given SC index. */
extern int bcm_xflow_macsec_ipsec_secure_chan_get(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_ipsec_secure_chan_info_t *chan_info);

/* 
 * Delete the given IPsec security channel. The corresponding SA must be
 * destroyed first.
 */
extern int bcm_xflow_macsec_ipsec_secure_chan_destroy(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize the IPsec secure channel data structure. */
extern void bcm_xflow_macsec_ipsec_secure_chan_info_t_init(
    bcm_xflow_macsec_ipsec_secure_chan_info_t *chan_info);

/* bcm_xflow_macsec_ipsec_chan_traverse_cb */
typedef int (*bcm_xflow_macsec_ipsec_chan_traverse_cb)(
    int unit, 
    bcm_xflow_macsec_ipsec_secure_chan_info_t *chan_info, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* Traverse all secure channels. */
extern int bcm_xflow_macsec_ipsec_secure_chan_info_traverse(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_ipsec_chan_traverse_cb callback, 
    void *user_data);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef xflow_macsec_ipsec_secure_assoc_info_t bcm_xflow_macsec_ipsec_secure_assoc_info_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_xflow_macsec_ipsec_secure_assoc_create creates a new IPsec
 * Security Association for the given Security Channel.
 */
extern int bcm_xflow_macsec_ipsec_secure_assoc_create(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_ipsec_secure_assoc_info_t *assoc_info, 
    bcm_xflow_macsec_secure_assoc_id_t *assoc_id);

/* 
 * Sets/resets the value of any of the parameters of a given IPsec
 * Security Association.
 */
extern int bcm_xflow_macsec_ipsec_secure_assoc_set(
    int unit, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id, 
    bcm_xflow_macsec_ipsec_secure_assoc_info_t *assoc_info);

/* 
 * Retrieve the IPsec Security Association configuration and the SC index
 * from the SA index.
 */
extern int bcm_xflow_macsec_ipsec_secure_assoc_get(
    int unit, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id, 
    bcm_xflow_macsec_ipsec_secure_assoc_info_t *assoc_info, 
    bcm_xflow_macsec_secure_chan_id_t *chan_id);

/* Deletes the SA entry corresponding to the Security Association. */
extern int bcm_xflow_macsec_ipsec_secure_assoc_destroy(
    int unit, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize the Security Association data structure. */
extern void bcm_xflow_macsec_ipsec_secure_assoc_info_t_init(
    bcm_xflow_macsec_ipsec_secure_assoc_info_t *assoc_info);

/* bcm_xflow_macsec_ipsec_secure_assoc_traverse_cb */
typedef int (*bcm_xflow_macsec_ipsec_secure_assoc_traverse_cb)(
    int unit, 
    bcm_xflow_macsec_ipsec_secure_assoc_info_t *assoc, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_secure_assoc_id_t assoc_id, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * Traverse all the secure association belonging to an IPsec Secure
 * channel.
 */
extern int bcm_xflow_macsec_ipsec_secure_assoc_traverse(
    int unit, 
    bcm_xflow_macsec_secure_chan_id_t chan_id, 
    bcm_xflow_macsec_ipsec_secure_assoc_traverse_cb callback, 
    void *user_data);

#endif /* defined(INCLUDE_XFLOW_MACSEC) */

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_XFLOW_MACSEC_H__ */
