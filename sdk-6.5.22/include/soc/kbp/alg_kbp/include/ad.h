

#ifndef __AD_H
#define __AD_H



#include <stdint.h>
#include <stdio.h>

#include "errors.h"
#include "device.h"
#include "db.h"

#ifdef __cplusplus
extern "C" {
#endif





struct kbp_ad_db;



struct kbp_ad;



kbp_status kbp_ad_db_init(struct kbp_device *device, uint32_t id, uint32_t capacity,
                          uint32_t width_1, struct kbp_ad_db **db);



kbp_status kbp_ad_db_destroy(struct kbp_ad_db *db);



kbp_status kbp_ad_db_set_resource(struct kbp_ad_db *db, enum kbp_hw_resource resource, ...);



kbp_status kbp_ad_db_get_resource(struct kbp_ad_db *db, enum kbp_hw_resource resource, ...);



kbp_status kbp_ad_db_set_property(struct kbp_ad_db *db, enum kbp_db_properties property, ...);



kbp_status kbp_ad_db_get_property(struct kbp_ad_db *db, enum kbp_db_properties property, ...);



kbp_status kbp_ad_db_add_entry(struct kbp_ad_db *db, uint8_t * value, struct kbp_ad **ad);



kbp_status kbp_ad_db_delete_entry(struct kbp_ad_db *db, struct kbp_ad *ad);



kbp_status kbp_ad_db_update_entry(struct kbp_ad_db *db, struct kbp_ad *ad, uint8_t * value);



kbp_status kbp_ad_db_get(struct kbp_ad_db *db, struct kbp_ad *ad, uint8_t * value);



kbp_status kbp_ad_entry_print(struct kbp_ad_db *db, struct kbp_ad *ad, FILE * fp);



kbp_status kbp_ad_db_delete_all_entries(struct kbp_ad_db *db);




kbp_status kbp_ad_db_delete_unused_entries(struct kbp_ad_db *db);








kbp_status kbp_ad_db_refresh_handle(struct kbp_device *device, struct kbp_ad_db *stale_ptr, struct kbp_ad_db **dbp);



#ifdef __cplusplus
}
#endif
#endif 
