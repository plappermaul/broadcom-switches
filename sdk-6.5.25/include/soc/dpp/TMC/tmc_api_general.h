/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
*/

#ifndef __SOC_TMC_API_GENERAL_INCLUDED__

#define __SOC_TMC_API_GENERAL_INCLUDED__




#include <soc/dpp/SAND/Utils/sand_header.h>
#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Utils/sand_integer_arithmetic.h>

#include <soc/dpp/SAND/SAND_FM/sand_user_callback.h>
#include <soc/dpp/SAND/SAND_FM/sand_chip_defines.h>
#include <shared/error.h>






#define SOC_TMC_NOF_TM_DOMAIN_IN_SYSTEM     (16)


#define SOC_TMC_NOF_FAPS_IN_SYSTEM     (2048)
#define SOC_TMC_MAX_FAP_ID             (SOC_TMC_NOF_FAPS_IN_SYSTEM-1)
#define SOC_TMC_MAX_DEVICE_ID          (SOC_TMC_NOF_FAPS_IN_SYSTEM-1)




#define SOC_TMC_DEVICE_ID_IRRELEVANT   SOC_TMC_MAX_DEVICE_ID


#define SOC_TMC_NOF_SYS_PHYS_PORTS      (4096)
#define SOC_TMC_NOF_SYS_PHYS_PORTS_ARAD (32*1024)
#define SOC_TMC_MAX_SYSTEM_PHYSICAL_PORT_ID      (SOC_TMC_NOF_SYS_PHYS_PORTS - 1)      
#define SOC_TMC_MAX_SYSTEM_PHYSICAL_PORT_ID_ARAD (SOC_TMC_NOF_SYS_PHYS_PORTS_ARAD - 1) 


#define SOC_TMC_SYS_PHYS_PORT_INVALID_INDIRECT   (SOC_TMC_NOF_SYS_PHYS_PORTS - 1)
#define SOC_TMC_SYS_PHYS_PORT_INVALID_DIRECT   (SOC_TMC_NOF_SYS_PHYS_PORTS_ARAD - 1)


#define SOC_TMC_NOF_SYSTEM_PORTS_ARAD         (64*1024)








#define SOC_TMC_NOF_DROP_PRECEDENCE    4





#define SOC_TMC_NOF_TRAFFIC_CLASSES        8
#define SOC_TMC_NOF_TRAFFIC_CLASSES_VIPER  2



#define SOC_TMC_NOF_TCGS               8
 
#define SOC_TMC_TCG_MIN                0
#define SOC_TMC_TCG_MAX                SOC_TMC_NOF_TCGS-1


#define SOC_TMC_NOF_TCG_IDS            (256)


#define SOC_TMC_TCG_NOF_PRIORITIES_SUPPORT    (8)


#define SOC_TMC_NOF_FAP_PORTS                  80


#define SOC_TMC_NOF_FAP_PORTS_PETRA            80


#define SOC_TMC_NOF_FAP_PORTS_ARAD             256

#define SOC_TMC_NOF_FAP_PORTS_MAX              (SOC_TMC_NOF_FAP_PORTS_ARAD)


#define SOC_TMC_NOF_FAP_PORTS_PER_CORE         256
#define SOC_TMC_NOF_FAP_PORTS_JERICHO          (SOC_TMC_NOF_FAP_PORTS_PER_CORE * 2)


#define SOC_TMC_NOF_FAP_PORTS_MAX_IN_LONGS ((SOC_TMC_NOF_FAP_PORTS_MAX + 31) / 32)


#define SOC_TMC_FAP_PORT_ID_INVALID            81


#define SOC_TMC_FAP_EGRESS_REPLICATION_SCH_PORT_ID       80
#define SOC_TMC_FAP_EGRESS_REPLICATION_IPS_PORT_ID       255





#define SOC_TMC_FBR_NOF_LINKS               36


#define SOC_TMC_RATE_1K                           1000
               
#define SOC_TMC_SCH_MAX_RATE_MBPS_ARAD(unit)            (SOC_TMC_RATE_1K * SOC_DPP_DEFS_GET(unit, max_gbps_rate_sch))


#define SOC_TMC_SCH_MAX_RATE_KBPS_ARAD(unit)             (SOC_TMC_RATE_1K * SOC_TMC_SCH_MAX_RATE_MBPS_ARAD(unit))


#define SOC_TMC_IF_MAX_RATE_MBPS_ARAD(unit)              (SOC_TMC_RATE_1K * SOC_DPP_DEFS_GET(unit, max_gbps_rate_egq)) 

#define SOC_TMC_IF_MAX_RATE_KBPS_ARAD(unit)              (SOC_TMC_RATE_1K * SOC_TMC_IF_MAX_RATE_MBPS_ARAD(unit))





#define SOC_TMC_DEFAULT_DESTINATION_FAP    (SOC_TMC_NOF_FAPS_IN_SYSTEM-1)












#define SOC_TMC_MULT_NOF_MULTICAST_GROUPS_ARAD  (64*1024)




#define SOC_TMC_NOF_SIGNATURE        4
#define SOC_TMC_MAX_SIGNATURE        (SOC_TMC_NOF_SIGNATURE-1)






#define SOC_TMC_MAX_NOF_LAG_MEMBER_IN_GROUP              (256)




#define SOC_TMC_NOF_INGRESS_UC_TC_MAPPING_PROFILES    4
#define SOC_TMC_NOF_INGRESS_FLOW_TC_MAPPING_PROFILES  4




#define SOC_TMC_DEBUG (SOC_SAND_DEBUG)
#define SOC_TMC_DEBUG_IS_LVL1   (SOC_TMC_DEBUG >= SOC_SAND_DBG_LVL1)

#define SOC_TMC_DO_NOTHING_AND_EXIT                       goto exit


#define SOC_TMC_OLP_PORT_ID                   79




























#define SOC_TMC_IF_NOF_NIFS 32


#define NUM_OF_COUNTERS_CMDS (2)
#define CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(num,nof_bits) ((num >= 0) ? num : ((~(0-num) + 1)&((1<<nof_bits)-1)))
#define CONVERT_TWO_COMPLEMENT_INTO_SIGNED_NUM(num,nof_bits) ( ((num & (1<<(nof_bits-1))) == 0) ? num : 0-((~(num-1))&((1<<nof_bits)-1)) )






typedef uint32 SOC_TMC_MULT_ID;


typedef uint32 SOC_TMC_TR_CLS;


typedef uint32 SOC_TMC_TCG_NDX;


typedef uint32 SOC_TMC_FAP_PORT_ID;


typedef uint32 SOC_TMC_PON_TUNNEL_ID;

typedef enum
{
  SOC_TMC_OLP_ID       = 0 ,
  SOC_TMC_IRE_ID       = 1 ,
  SOC_TMC_IDR_ID       = 2 ,
  SOC_TMC_IRR_ID       = 3 ,
  SOC_TMC_IHP_ID       = 4 ,
  SOC_TMC_QDR_ID       = 5 ,
  SOC_TMC_IPS_ID       = 6 ,
  SOC_TMC_IPT_ID       = 7 ,
  SOC_TMC_DPI_A_ID     = 8 ,
  SOC_TMC_DPI_B_ID     = 9 ,
  SOC_TMC_DPI_C_ID     = 10,
  SOC_TMC_DPI_D_ID     = 11,
  SOC_TMC_DPI_E_ID     = 12,
  SOC_TMC_DPI_F_ID     = 13,
  SOC_TMC_RTP_ID       = 14,
  SOC_TMC_EGQ_ID       = 15,
  SOC_TMC_SCH_ID       = 16,
  SOC_TMC_CFC_ID       = 17,
  SOC_TMC_EPNI_ID      = 18,
  SOC_TMC_IQM_ID       = 19,
  SOC_TMC_MMU_ID       = 20,
  SOC_TMC_NOF_MODULES  = 21
}SOC_TMC_MODULE_ID;

typedef enum
{
  
  SOC_TMC_FAR_DEVICE_TYPE_FE1=6,
  
  SOC_TMC_FAR_DEVICE_TYPE_FE2=3,
  
  SOC_TMC_FAR_DEVICE_TYPE_FE3=2,
  
  SOC_TMC_FAR_DEVICE_TYPE_FAP=0,
  SOC_TMC_FAR_NOF_DEVICE_TYPES = 8

}SOC_TMC_FAR_DEVICE_TYPE;

typedef enum
{
  
  SOC_TMC_IF_TYPE_NONE=0,
  
  SOC_TMC_IF_TYPE_CPU=1,
  
  SOC_TMC_IF_TYPE_RCY=2,
  
  SOC_TMC_IF_TYPE_OLP=3,
  
  SOC_TMC_IF_TYPE_ERP=4,
  
  SOC_TMC_IF_TYPE_NIF=5,
  
  SOC_TMC_IF_TYPE_OAMP=6,

  SOC_TMC_IF_NOF_TYPES = 7
}SOC_TMC_INTERFACE_TYPE;

typedef enum
{
  
  SOC_TMC_IF_ID_0 = 0,
  
  SOC_TMC_IF_ID_1 = 1,
  
  SOC_TMC_IF_ID_2 = 2,
  
  SOC_TMC_IF_ID_3 = 3,
  
  SOC_TMC_IF_ID_4 = 4,
  
  SOC_TMC_IF_ID_5 = 5,
  
  SOC_TMC_IF_ID_6 = 6,
  
  SOC_TMC_IF_ID_7 = 7,
  
  SOC_TMC_IF_ID_8 = 8,
  
  SOC_TMC_IF_ID_9 = 9,
  
  SOC_TMC_IF_ID_10 = 10,
  
  SOC_TMC_IF_ID_11 = 11,
  
  SOC_TMC_IF_ID_12 = 12,
  
  SOC_TMC_IF_ID_13 = 13,
  
  SOC_TMC_IF_ID_14 = 14,
  
  SOC_TMC_IF_ID_15 = 15,
  
  SOC_TMC_IF_ID_16 = 16,
  
  SOC_TMC_IF_ID_17 = 17,
  
  SOC_TMC_IF_ID_18 = 18,
  
  SOC_TMC_IF_ID_19 = 19,
  
  SOC_TMC_IF_ID_20 = 20,
  
  SOC_TMC_IF_ID_21 = 21,
  
  SOC_TMC_IF_ID_22 = 22,
  
  SOC_TMC_IF_ID_23 = 23,
  
  SOC_TMC_IF_ID_24 = 24,
  
  SOC_TMC_IF_ID_25 = 25,
  
  SOC_TMC_IF_ID_26 = 26,
  
  SOC_TMC_IF_ID_27 = 27,
  
  SOC_TMC_IF_ID_28 = 28,
  
  SOC_TMC_IF_ID_29 = 29,
  
  SOC_TMC_IF_ID_30 = 30,
  
  SOC_TMC_IF_ID_31 = 31,
  
  SOC_TMC_NIF_ID_XAUI_0 = 1000,
  
  SOC_TMC_NIF_ID_XAUI_1 = 1001,
  
  SOC_TMC_NIF_ID_XAUI_2 = 1002,
  
  SOC_TMC_NIF_ID_XAUI_3 = 1003,
  
  SOC_TMC_NIF_ID_XAUI_4 = 1004,
  
  SOC_TMC_NIF_ID_XAUI_5 = 1005,
  
  SOC_TMC_NIF_ID_XAUI_6 = 1006,
  
  SOC_TMC_NIF_ID_XAUI_7 = 1007,
  
  SOC_TMC_NIF_ID_RXAUI_0 = 2000,
  
  SOC_TMC_NIF_ID_RXAUI_1 = 2001,
  
  SOC_TMC_NIF_ID_RXAUI_2 = 2002,
  
  SOC_TMC_NIF_ID_RXAUI_3 = 2003,
  
  SOC_TMC_NIF_ID_RXAUI_4 = 2004,
  
  SOC_TMC_NIF_ID_RXAUI_5 = 2005,
  
  SOC_TMC_NIF_ID_RXAUI_6 = 2006,
  
  SOC_TMC_NIF_ID_RXAUI_7 = 2007,
  
  SOC_TMC_NIF_ID_RXAUI_8 = 2008,
  
  SOC_TMC_NIF_ID_RXAUI_9 = 2009,
  
  SOC_TMC_NIF_ID_RXAUI_10 = 2010,
  
  SOC_TMC_NIF_ID_RXAUI_11 = 2011,
  
  SOC_TMC_NIF_ID_RXAUI_12 = 2012,
  
  SOC_TMC_NIF_ID_RXAUI_13 = 2013,
  
  SOC_TMC_NIF_ID_RXAUI_14 = 2014,
  
  SOC_TMC_NIF_ID_RXAUI_15 = 2015,
  
  SOC_TMC_NIF_ID_SGMII_0 = 3000,
  
  SOC_TMC_NIF_ID_SGMII_1 = 3001,
  
  SOC_TMC_NIF_ID_SGMII_2 = 3002,
  
  SOC_TMC_NIF_ID_SGMII_3 = 3003,
  
  SOC_TMC_NIF_ID_SGMII_4 = 3004,
  
  SOC_TMC_NIF_ID_SGMII_5 = 3005,
  
  SOC_TMC_NIF_ID_SGMII_6 = 3006,
  
  SOC_TMC_NIF_ID_SGMII_7 = 3007,
  
  SOC_TMC_NIF_ID_SGMII_8 = 3008,
  
  SOC_TMC_NIF_ID_SGMII_9 = 3009,
  
  SOC_TMC_NIF_ID_SGMII_10 = 3010,
  
  SOC_TMC_NIF_ID_SGMII_11 = 3011,
  
  SOC_TMC_NIF_ID_SGMII_12 = 3012,
  
  SOC_TMC_NIF_ID_SGMII_13 = 3013,
  
  SOC_TMC_NIF_ID_SGMII_14 = 3014,
  
  SOC_TMC_NIF_ID_SGMII_15 = 3015,
  
  SOC_TMC_NIF_ID_SGMII_16 = 3016,
  
  SOC_TMC_NIF_ID_SGMII_17 = 3017,
  
  SOC_TMC_NIF_ID_SGMII_18 = 3018,
  
  SOC_TMC_NIF_ID_SGMII_19 = 3019,
  
  SOC_TMC_NIF_ID_SGMII_20 = 3020,
  
  SOC_TMC_NIF_ID_SGMII_21 = 3021,
  
  SOC_TMC_NIF_ID_SGMII_22 = 3022,
  
  SOC_TMC_NIF_ID_SGMII_23 = 3023,
  
  SOC_TMC_NIF_ID_SGMII_24 = 3024,
  
  SOC_TMC_NIF_ID_SGMII_25 = 3025,
  
  SOC_TMC_NIF_ID_SGMII_26 = 3026,
  
  SOC_TMC_NIF_ID_SGMII_27 = 3027,
  
  SOC_TMC_NIF_ID_SGMII_28 = 3028,
  
  SOC_TMC_NIF_ID_SGMII_29 = 3029,
  
  SOC_TMC_NIF_ID_SGMII_30 = 3030,
  
  SOC_TMC_NIF_ID_SGMII_31 = 3031,
  
  SOC_TMC_NIF_ID_QSGMII_0 = 4000,
  
  SOC_TMC_NIF_ID_QSGMII_1 = 4001,
  
  SOC_TMC_NIF_ID_QSGMII_2 = 4002,
  
  SOC_TMC_NIF_ID_QSGMII_3 = 4003,
  
  SOC_TMC_NIF_ID_QSGMII_4 = 4004,
  
  SOC_TMC_NIF_ID_QSGMII_5 = 4005,
  
  SOC_TMC_NIF_ID_QSGMII_6 = 4006,
  
  SOC_TMC_NIF_ID_QSGMII_7 = 4007,
  
  SOC_TMC_NIF_ID_QSGMII_8 = 4008,
  
  SOC_TMC_NIF_ID_QSGMII_9 = 4009,
  
  SOC_TMC_NIF_ID_QSGMII_10 = 4010,
  
  SOC_TMC_NIF_ID_QSGMII_11 = 4011,
  
  SOC_TMC_NIF_ID_QSGMII_12 = 4012,
  
  SOC_TMC_NIF_ID_QSGMII_13 = 4013,
  
  SOC_TMC_NIF_ID_QSGMII_14 = 4014,
  
  SOC_TMC_NIF_ID_QSGMII_15 = 4015,
  
  SOC_TMC_NIF_ID_QSGMII_16 = 4016,
  
  SOC_TMC_NIF_ID_QSGMII_17 = 4017,
  
  SOC_TMC_NIF_ID_QSGMII_18 = 4018,
  
  SOC_TMC_NIF_ID_QSGMII_19 = 4019,
  
  SOC_TMC_NIF_ID_QSGMII_20 = 4020,
  
  SOC_TMC_NIF_ID_QSGMII_21 = 4021,
  
  SOC_TMC_NIF_ID_QSGMII_22 = 4022,
  
  SOC_TMC_NIF_ID_QSGMII_23 = 4023,
  
  SOC_TMC_NIF_ID_QSGMII_24 = 4024,
  
  SOC_TMC_NIF_ID_QSGMII_25 = 4025,
  
  SOC_TMC_NIF_ID_QSGMII_26 = 4026,
  
  SOC_TMC_NIF_ID_QSGMII_27 = 4027,
  
  SOC_TMC_NIF_ID_QSGMII_28 = 4028,
  
  SOC_TMC_NIF_ID_QSGMII_29 = 4029,
  
  SOC_TMC_NIF_ID_QSGMII_30 = 4030,
  
  SOC_TMC_NIF_ID_QSGMII_31 = 4031,
  
  SOC_TMC_NIF_ID_QSGMII_32 = 4032,
  
  SOC_TMC_NIF_ID_QSGMII_33 = 4033,
  
  SOC_TMC_NIF_ID_QSGMII_34 = 4034,
  
  SOC_TMC_NIF_ID_QSGMII_35 = 4035,
  
  SOC_TMC_NIF_ID_QSGMII_36 = 4036,
  
  SOC_TMC_NIF_ID_QSGMII_37 = 4037,
  
  SOC_TMC_NIF_ID_QSGMII_38 = 4038,
  
  SOC_TMC_NIF_ID_QSGMII_39 = 4039,
  
  SOC_TMC_NIF_ID_QSGMII_40 = 4040,
  
  SOC_TMC_NIF_ID_QSGMII_41 = 4041,
  
  SOC_TMC_NIF_ID_QSGMII_42 = 4042,
  
  SOC_TMC_NIF_ID_QSGMII_43 = 4043,
  
  SOC_TMC_NIF_ID_QSGMII_44 = 4044,
  
  SOC_TMC_NIF_ID_QSGMII_45 = 4045,
  
  SOC_TMC_NIF_ID_QSGMII_46 = 4046,
  
  SOC_TMC_NIF_ID_QSGMII_47 = 4047,
  
  SOC_TMC_NIF_ID_QSGMII_48 = 4048,
  
  SOC_TMC_NIF_ID_QSGMII_49 = 4049,
  
  SOC_TMC_NIF_ID_QSGMII_50 = 4050,
  
  SOC_TMC_NIF_ID_QSGMII_51 = 4051,
  
  SOC_TMC_NIF_ID_QSGMII_52 = 4052,
  
  SOC_TMC_NIF_ID_QSGMII_53 = 4053,
  
  SOC_TMC_NIF_ID_QSGMII_54 = 4054,
  
  SOC_TMC_NIF_ID_QSGMII_55 = 4055,
  
  SOC_TMC_NIF_ID_QSGMII_56 = 4056,
  
  SOC_TMC_NIF_ID_QSGMII_57 = 4057,
  
  SOC_TMC_NIF_ID_QSGMII_58 = 4058,
  
  SOC_TMC_NIF_ID_QSGMII_59 = 4059,
  
  SOC_TMC_NIF_ID_QSGMII_60 = 4060,
  
  SOC_TMC_NIF_ID_QSGMII_61 = 4061,
  
  SOC_TMC_NIF_ID_QSGMII_62 = 4062,
  
  SOC_TMC_NIF_ID_QSGMII_63 = 4063,
  
  SOC_TMC_NIF_ID_ILKN_0 = 5000,
  
  SOC_TMC_NIF_ID_ILKN_1 = 5001,
  
  SOC_TMC_NIF_ID_ILKN_TDM_0 = 5002,
  
  SOC_TMC_NIF_ID_ILKN_TDM_1 = 5003,
  
  SOC_TMC_NIF_ID_CGE_0 = 7000,
  
  SOC_TMC_NIF_ID_CGE_1 = 7001,
  
  SOC_TMC_NIF_ID_XLGE_0 = 8000,
  
  SOC_TMC_NIF_ID_XLGE_1 = 8001,
  
  SOC_TMC_NIF_ID_XLGE_2 = 8002,
  
  SOC_TMC_NIF_ID_XLGE_3 = 8003,
  
  SOC_TMC_NIF_ID_XLGE_4 = 8004,
  
  SOC_TMC_NIF_ID_XLGE_5 = 8005,
  
  SOC_TMC_NIF_ID_XLGE_6 = 8006,
  
  SOC_TMC_NIF_ID_XLGE_7 = 8007,
  
  SOC_TMC_NIF_ID_10GBASE_R_0 = 9000,
  
  SOC_TMC_NIF_ID_10GBASE_R_1 = 9001,
  
  SOC_TMC_NIF_ID_10GBASE_R_2 = 9002,
  
  SOC_TMC_NIF_ID_10GBASE_R_3 = 9003,
  
  SOC_TMC_NIF_ID_10GBASE_R_4 = 9004,
  
  SOC_TMC_NIF_ID_10GBASE_R_5 = 9005,
  
  SOC_TMC_NIF_ID_10GBASE_R_6 = 9006,
  
  SOC_TMC_NIF_ID_10GBASE_R_7 = 9007,
  
  SOC_TMC_NIF_ID_10GBASE_R_8 = 9008,
  
  SOC_TMC_NIF_ID_10GBASE_R_9 = 9009,
  
  SOC_TMC_NIF_ID_10GBASE_R_10 = 9010,
  
  SOC_TMC_NIF_ID_10GBASE_R_11 = 9011,
  
  SOC_TMC_NIF_ID_10GBASE_R_12 = 9012,
  
  SOC_TMC_NIF_ID_10GBASE_R_13 = 9013,
  
  SOC_TMC_NIF_ID_10GBASE_R_14 = 9014,
  
  SOC_TMC_NIF_ID_10GBASE_R_15 = 9015,
  
  SOC_TMC_NIF_ID_10GBASE_R_16 = 9016,
  
  SOC_TMC_NIF_ID_10GBASE_R_17 = 9017,
  
  SOC_TMC_NIF_ID_10GBASE_R_18 = 9018,
  
  SOC_TMC_NIF_ID_10GBASE_R_19 = 9019,
  
  SOC_TMC_NIF_ID_10GBASE_R_20 = 9020,
  
  SOC_TMC_NIF_ID_10GBASE_R_21 = 9021,
  
  SOC_TMC_NIF_ID_10GBASE_R_22 = 9022,
  
  SOC_TMC_NIF_ID_10GBASE_R_23 = 9023,
  
  SOC_TMC_NIF_ID_10GBASE_R_24 = 9024,
  
  SOC_TMC_NIF_ID_10GBASE_R_25 = 9025,
  
  SOC_TMC_NIF_ID_10GBASE_R_26 = 9026,
  
  SOC_TMC_NIF_ID_10GBASE_R_27 = 9027,
  
  SOC_TMC_NIF_ID_10GBASE_R_28 = 9028,
  
  SOC_TMC_NIF_ID_10GBASE_R_29 = 9029,
  
  SOC_TMC_NIF_ID_10GBASE_R_30 = 9030,
  
  SOC_TMC_NIF_ID_10GBASE_R_31 = 9031,
  
  SOC_TMC_IF_ID_CPU = 200,
  
  SOC_TMC_IF_ID_OLP = 300,
  
  SOC_TMC_IF_ID_RCY = 400,
  
  SOC_TMC_IF_ID_ERP = 500,
  
  SOC_TMC_IF_ID_OAMP = 600,
  
  SOC_TMC_IF_ID_TM_INTERNAL_PKT = 700,
  
  SOC_TMC_IF_ID_RESERVED = 800,
  
   SOC_TMC_IF_ID_NONE = 65535
}SOC_TMC_INTERFACE_ID;

#define SOC_TMC_NIF_ID_NONE SOC_TMC_IF_ID_NONE



typedef enum
{
  
  SOC_TMC_MAL_TYPE_NONE=0,
  
  SOC_TMC_MAL_TYPE_CPU=1,
  
  SOC_TMC_MAL_TYPE_RCY=2,
  
  SOC_TMC_MAL_TYPE_OLP=3,
  
  SOC_TMC_MAL_TYPE_ERP=4,
  
  SOC_TMC_MAL_TYPE_NIF=5,

  SOC_TMC_MAL_TYPE_NOF_TYPES=6
}SOC_TMC_MAL_EQUIVALENT_TYPE;

typedef enum
{
  
  SOC_TMC_FC_DIRECTION_GEN=0,
  
  SOC_TMC_FC_DIRECTION_REC=1,
  
  SOC_TMC_FC_NOF_DIRECTIONS=2
}SOC_TMC_FC_DIRECTION;




typedef enum
{
  
  SOC_TMC_DEST_TYPE_QUEUE=0,
  
  SOC_TMC_DEST_TYPE_MULTICAST=1,
  
  SOC_TMC_DEST_TYPE_SYS_PHY_PORT=2,
  
  SOC_TMC_DEST_TYPE_LAG=3,
  
  SOC_TMC_NOF_DEST_TYPES_PETRA=4,
  
  SOC_TMC_DEST_TYPE_ISQ_FLOW_ID=4,
  
  SOC_TMC_DEST_TYPE_OUT_LIF=5,
  
  SOC_TMC_DEST_TYPE_MULTICAST_FLOW_ID=6,
  
  SOC_TMC_NOF_DEST_TYPES_ARAD=7,
  
  SOC_TMC_DEST_TYPE_FEC_PTR = SOC_TMC_NOF_DEST_TYPES_ARAD,
  
  SOC_TMC_NOF_DEST_TYPES_JER=8

}SOC_TMC_DEST_TYPE;

typedef enum
{
  
  SOC_TMC_DEST_SYS_PORT_TYPE_SYS_PHY_PORT=0,
  
  SOC_TMC_DEST_SYS_PORT_TYPE_LAG=1,
  
  SOC_TMC_DEST_SYS_PORT_NOF_TYPES=2
}SOC_TMC_DEST_SYS_PORT_TYPE;

typedef enum
{
    SOC_TMC_CMD_TYPE_MIRROR = 0,
    SOC_TMC_CMD_TYPE_SNOOP = 1
}SOC_TMC_CMD_TYPE;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_DEST_SYS_PORT_TYPE type;
 
  uint32 id;
  
  uint32 member_id;
}SOC_TMC_DEST_SYS_PORT_INFO;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_DEST_TYPE type;
  
  uint32 id;
}SOC_TMC_DEST_INFO;

typedef enum
{
  
  SOC_TMC_MULTICAST_CLASS_0=0,
  
  SOC_TMC_MULTICAST_CLASS_1=1,
  
  SOC_TMC_MULTICAST_CLASS_2=2,
  
  SOC_TMC_MULTICAST_CLASS_3=3,
  
  SOC_TMC_NOF_MULTICAST_CLASSES=4,
  
  SOC_TMC_MULTICAST_CLASS_LAST

}SOC_TMC_MULTICAST_CLASS;

typedef enum
{
  
  SOC_TMC_CONNECTION_DIRECTION_RX=0,
  
  SOC_TMC_CONNECTION_DIRECTION_TX=1,
  
  SOC_TMC_CONNECTION_DIRECTION_BOTH=2,
  
  SOC_TMC_NOF_CONNECTION_DIRECTIONS=3
}SOC_TMC_CONNECTION_DIRECTION;

typedef enum
{
  
  SOC_TMC_PKT_FRWRD_TYPE_BRIDGE = 0,
  
  SOC_TMC_PKT_FRWRD_TYPE_IPV4_UC = 1,
  
  SOC_TMC_PKT_FRWRD_TYPE_IPV4_MC = 2,
  
  SOC_TMC_PKT_FRWRD_TYPE_IPV6_UC = 3,
  
  SOC_TMC_PKT_FRWRD_TYPE_IPV6_MC = 4,
  
  SOC_TMC_PKT_FRWRD_TYPE_MPLS = 5,
  
  SOC_TMC_PKT_FRWRD_TYPE_TRILL = 6,
  
  SOC_TMC_PKT_FRWRD_TYPE_CPU_TRAP = 7,
  
  SOC_TMC_PKT_FRWRD_TYPE_BRIDGE_AFTER_TERM = 8,
  
  SOC_TMC_PKT_FRWRD_TYPE_CUSTOM1 = 9,
  
  SOC_TMC_PKT_FRWRD_TYPE_CUSTOM2 = 0xA,
  
   SOC_TMC_PKT_FRWRD_TYPE_SNOOP = 0xB,
  
   SOC_TMC_PKT_FRWRD_TYPE_TM = 0xE,
  
  SOC_TMC_NOF_PKT_FRWRD_TYPES = 0x10
}SOC_TMC_PKT_FRWRD_TYPE;

typedef enum
{
  
  SOC_TMC_TUNNEL_TERM_CODE_NONE = 0,
  
  SOC_TMC_TUNNEL_TERM_CODE_ETH = 1,
  
  SOC_TMC_TUNNEL_TERM_CODE_IPV4_ETH = 2,
  
  SOC_TMC_TUNNEL_TERM_CODE_MPLS_ETH = 3,
  
  SOC_TMC_TUNNEL_TERM_CODE_MPLS_ETH_CW = 4,
  
  SOC_TMC_TUNNEL_TERM_CODE_MPLS2_ETH = 5,
  
  SOC_TMC_TUNNEL_TERM_CODE_MPLS2_ETH_CW = 6,
  
  SOC_TMC_TUNNEL_TERM_CODE_MPLS3_ETH = 7,
  
  SOC_TMC_TUNNEL_TERM_CODE_MPLS3_ETH_CW = 8,
  
  SOC_TMC_TUNNEL_TERM_CODE_TRILL_ETH = 9,
  
  SOC_TMC_NOF_TUNNEL_TERM_CODES = 10
}SOC_TMC_TUNNEL_TERM_CODE;

typedef enum
{
  
  SOC_TMC_MPLS_COMMAND_TYPE_PUSH = 0,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP = 1,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_MPLS_PIPE = 8,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_MPLS_UNIFORM = 9,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV4_PIPE = 10,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV4_UNIFORM = 11,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV6_PIPE = 12,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV6_UNIFORM = 13,
  
  SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_ETHERNET = 14,
  
  SOC_TMC_MPLS_COMMAND_TYPE_SWAP = 15,
  
  SOC_TMC_NOF_MPLS_COMMAND_TYPES = 16
}SOC_TMC_MPLS_COMMAND_TYPE;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  uint32 set;
  
  uint32 clear;
}SOC_TMC_THRESH_WITH_HYST_INFO;

typedef struct
{
  
  uint32 max_threshold;
  
  uint32 min_threshold;
  
  int   alpha;
} SOC_TMC_FADT_INFO;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_FADT_INFO set;
  
  uint32 clear_offset;
} SOC_TMC_THRESH_WITH_FADT_HYST_INFO;

typedef enum
{
  
  SOC_TMC_COMBO_QRTT_0=0,
  
  SOC_TMC_COMBO_QRTT_1=1,
  
  SOC_TMC_COMBO_NOF_QRTTS=2
}SOC_TMC_COMBO_QRTT;


typedef enum
{
  
  SOC_TMC_SRD_POWER_STATE_DOWN=0,
  
  SOC_TMC_SRD_POWER_STATE_UP=1,
  
  SOC_TMC_SRD_POWER_STATE_UP_AND_RELOCK=2,
  
  SOC_TMC_SRD_NOF_POWER_STATES=3
}SOC_TMC_SRD_POWER_STATE;

typedef _shr_error_t SOC_TMC_ERROR;


typedef enum
{
  
  SOC_TMC_SWAP_MODE_4_BYTES = 0,
  
  SOC_TMC_SWAP_MODE_8_BYTES,
  
  SOC_TMC_SWAP_MODES
} SOC_TMC_SWAP_MODE;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_SWAP_MODE mode;
  
  uint32 offset;
} SOC_TMC_SWAP_INFO;











void
  SOC_TMC_DEST_SYS_PORT_INFO_clear(
    SOC_SAND_OUT SOC_TMC_DEST_SYS_PORT_INFO *info
  );

void
  SOC_TMC_DEST_INFO_clear(
    SOC_SAND_OUT SOC_TMC_DEST_INFO *info
  );

void
  SOC_TMC_THRESH_WITH_HYST_INFO_clear(
    SOC_SAND_OUT SOC_TMC_THRESH_WITH_HYST_INFO *info
  );

void
  SOC_TMC_THRESH_WITH_FADT_HYST_INFO_clear(
    SOC_SAND_OUT SOC_TMC_THRESH_WITH_FADT_HYST_INFO *info
  );

void
  SOC_TMC_SWAP_INFO_clear(
    SOC_SAND_OUT SOC_TMC_SWAP_INFO *info
  );

#if SOC_TMC_DEBUG_IS_LVL1

const char*
  SOC_TMC_COMBO_QRTT_to_string(
    SOC_SAND_IN SOC_TMC_COMBO_QRTT enum_val
  );

const char*
  SOC_TMC_FAR_DEVICE_TYPE_to_string(
    SOC_SAND_IN SOC_TMC_FAR_DEVICE_TYPE enum_val
  );

const char*
  SOC_TMC_INTERFACE_TYPE_to_string(
    SOC_SAND_IN SOC_TMC_INTERFACE_TYPE enum_val
  );

void
  SOC_TMC_INTERFACE_ID_print(
    SOC_SAND_IN SOC_TMC_INTERFACE_ID if_ndx
  );

const char*
  SOC_TMC_INTERFACE_ID_to_string(
    SOC_SAND_IN SOC_TMC_INTERFACE_ID enum_val
  );

const char*
  SOC_TMC_FC_DIRECTION_to_string(
    SOC_SAND_IN SOC_TMC_FC_DIRECTION enum_val
  );

const char*
  SOC_TMC_CONNECTION_DIRECTION_to_string(
    SOC_SAND_IN SOC_TMC_CONNECTION_DIRECTION enum_val
  );

const char*
  SOC_TMC_DEST_TYPE_to_string(
    SOC_SAND_IN SOC_TMC_DEST_TYPE enum_val,
    SOC_SAND_IN uint8       short_name
  );

const char*
  SOC_TMC_DEST_SYS_PORT_TYPE_to_string(
    SOC_SAND_IN SOC_TMC_DEST_SYS_PORT_TYPE enum_val
    );

const char*
  SOC_TMC_PKT_FRWRD_TYPE_to_string(
    SOC_SAND_IN  SOC_TMC_PKT_FRWRD_TYPE enum_val
  );

const char*
  SOC_TMC_TUNNEL_TERM_CODE_to_string(
    SOC_SAND_IN  SOC_TMC_TUNNEL_TERM_CODE enum_val
  );


void
  SOC_TMC_DEST_SYS_PORT_INFO_print(
    SOC_SAND_IN SOC_TMC_DEST_SYS_PORT_INFO *info
  );

void
  SOC_TMC_DEST_INFO_print(
    SOC_SAND_IN SOC_TMC_DEST_INFO *info
  );

void
  SOC_TMC_THRESH_WITH_HYST_INFO_print(
    SOC_SAND_IN SOC_TMC_THRESH_WITH_HYST_INFO *info
  );

void
  SOC_TMC_DEST_SYS_PORT_INFO_table_format_print(
    SOC_SAND_IN SOC_TMC_DEST_SYS_PORT_INFO *info
  );

const char*
  SOC_TMC_SWAP_MODE_to_string(
    SOC_SAND_IN  SOC_TMC_SWAP_MODE enum_val
  );

void
  SOC_TMC_SWAP_INFO_print(
    SOC_SAND_IN SOC_TMC_SWAP_INFO *info
  );

#endif 



#include <soc/dpp/SAND/Utils/sand_footer.h>


#endif