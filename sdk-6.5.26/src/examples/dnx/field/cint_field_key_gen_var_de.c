/*
 * Configuration example start:
 *
  cint ../../../../src/examples/dnx/field/cint_field_key_gen_var_de.c
  cint ../../../src/examples/dnx/field/cint_field_key_gen_var_de.c
  cint;
  int unit = 0;
  bcm_field_context_t context_id = 0;
  uint32 key_gen_val = 0xABCDEF;
  cint_field_key_gen_var_de_main(unit, context_id, key_gen_val);
 *
 * Configuration example end
 *
 * This CINT shows how to set constant value per context and use it as a qualifier for building a DE key.
 *
 * The main function:
 * cint_field_key_gen_var_de_main(unit, context_id)
 *
 * In the function, depends of the stage we are doing this steps:
 * !Note that for ipmf1/2 the same key_gen_var is set.
 *    1. Create a context.
 *    2. Set the context param to be bcmFieldContextParamTypeKeyVal
 *    4. Create a FG.
 *    5. Attach a FG to a context.
 *
 * The destroy function is destroying all configuration depends of the stage:
 * cint_field_key_gen_var_de_destroy(unit)
 */

 /**
  * Global variables
  * {
  */
bcm_field_group_t Cint_field_key_gen_var_de_fg_id;
int Cint_field_key_gen_var_de_nof_quals = 2;
/**
 * Structure, which contains information,
 * for creating of user actions and predefined one.
 */
struct Cint_field_key_gen_var_de_qual_info {
    char * name;
    uint32 qual_length;
    bcm_field_qualify_t qual_id;
};
/**
 * This array will be used for creating User Defined Qualifiers.
 * We are calling it in cint_field_key_gen_var_de_main.
 */
Cint_field_key_gen_var_de_qual_info Cint_field_key_gen_var_de_qual_info_arr[Cint_field_key_gen_var_de_nof_quals] = {
  /** qual_name */  /* size */
  {"user_qual_1",      8},
  {"user_qual_2",      8}
};
 /**
  * }
  */
/**
 * This function configure the user qualifiers.
 * It is used later in the cint_field_key_gen_var_de_main.
 *
 * \param [in] unit - The unit number.
 * \param [in] stage - The PMF stage.
 * \param [in] qual_info_p - Pointer to Structure which contains
 *      all needed information for creating a user defined qualifier.
 *
 */
int
cint_field_key_gen_var_de_qual_config(
    int unit,
    bcm_field_stage_t stage,
    Cint_field_key_gen_var_de_qual_info * qual_info_p)
{
    bcm_field_qualifier_info_create_t qual_info;
    void *dest_char;

    printf("Running: cint_field_key_gen_var_de_qual_config()\n");

    bcm_field_qualifier_info_create_t_init(&qual_info);
    dest_char = &(qual_info.name[0]);
    sal_strncpy_s(dest_char, qual_info_p->name, sizeof(qual_info.name));
    qual_info.size = qual_info_p->qual_length;
    BCM_IF_ERROR_RETURN(bcm_field_qualifier_create(unit, 0, &qual_info, &(qual_info_p->qual_id)));

    return BCM_E_NONE;
}
/**
 * The flow:
 *    1. Create a context.
 *    2. Set the context param to be bcmFieldContextParamTypeKeyVal.
 *    3. Set a preselector.
 *    4. Create a DE FG.
 *    5. Attach a FG to a context.
 *    6. Set a preselector.
 *
 * The key is created from the bits of the ETH header and KEY_GEN_VAR value.
 * We are using DE field group thats why:
 * The position in the key depends on Qualifiers sequence.
 *  * Qualifier 0 is added to LSB.
 *  * Following qualifiers follow in the direction of MSB.
 *      * We have 2 user defined qualifiers which are of
 *        type bcmFieldInputTypeLayerAbsolute(means that we are
 *        taking the key from the header) and one KEY_GEN_VAR
 *      * First qualifier (user defined) is with size 8 bits with input arg 0 and offset 0
 *      * Second qualifier (bcmFieldQualifyKeyGenVar) is 32 bits and value which we are
 *             setting with the API bcm_field_context_param_set
 *      * Third qualifier (user defined) is with size 8 bits with input arg 0 and offset 20
 *
 * We are adding 2 different actions
 *       * First Action is bcmFieldActionUDHData0 with size 32 bits
 *       * Second Action is bcmFieldActionStackingRouteNew with size 16 bits
 *
 * FOR EXAMPLE:
 *
 * In the first case we are setting the value of KeyGenVar to be equal to 89ABCDEF
 *
 * THE HEADER OF THE PACKET:
 *      _____________________  _______________________
 * field [        ETH.DA       ][          ETH.SA       ]
 *        ~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~
 *  bits [  11:96:c6:98:F0:01  ][    00:00:00:00:00:01  ]
 *        ~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~
 *        ______________________________________________________
 * THE KEY should look like this
 *        __________________________________  _________________________________   ________________________
 * field  [       Qualifier_3               ] [       Qualifier_2              ]  [       Qualifier_1     ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~
 * binary [              11                 ] [10001001101010111100110111101111]  [          11           ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * hexa   [                                      0x1189ABCDEF11                                           ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *              MSB                                                                                     LSB
 *
 * The PAYLOAD:
 *                 bcmFieldActionUDHData0                      bcmFieldActionStackingRouteNew
 **        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * hexa   [                 1189                ][                       ABCDEF11                         ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *              MSB                                                                                     LSB
 *
 * The value which we are expecting for each action is:
 *       * First action must be with value ABCDEF11
 *       Because it is taking the value from the 32LSB of the key.
 *       * Second action must be with value 1189
 *       Next 16bits.
 *
 * In the second case we are setting the value of KeyGenVar to be equal to 12345678

 * THE KEY should look like this
 *        __________________________________  _________________________________   ________________________
 * field  [       Qualifier_3               ] [       Qualifier_2              ]  [       Qualifier_1     ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~
 * binary [              11                 ] [00010010001101000101011001111000]  [          11           ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * hexa   [                                      0x111234567811                                           ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *              MSB                                                                                     LSB
 *
 * * The PAYLOAD:
 *                 bcmFieldActionUDHData0                      bcmFieldActionStackingRouteNew
 **        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * hexa   [                 1112                ][                       34567811                         ]
 *        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *              MSB                                                                                     LSB
 *
 * The value which we are expecting for each action is:
 *       * First action must be with value 34567811
 *       Because it is taking the value from the 32LSB of the key.
 *       * Second action must be with value 1112
 *       Next 16bits.
 *
 * \param [in] unit - The unit number.
 * \param [in] context_id - The context_id.
 * \param [in] key_gen_val -The value of the key_gen_val qual.
 *
 *  \return
 *  int - Error Type, in sense of bcm_error_t.
 */
int
cint_field_key_gen_var_de_main(
    int unit,
    bcm_field_context_t context_id,
    uint32 key_gen_val)
{
    bcm_field_group_info_t fg_info;
    bcm_field_group_attach_info_t attach_info;
    bcm_field_context_param_info_t param_info;
    int qual_index;
    bcm_field_stage_t stage;
    void *dest_char;

    printf("Running: cint_field_key_gen_var_de_main()\n");
    stage = bcmFieldStageIngressPMF2;

    for (qual_index = 0; qual_index < Cint_field_key_gen_var_de_nof_quals; qual_index++)
    {
        BCM_IF_ERROR_RETURN(cint_field_key_gen_var_de_qual_config(unit, stage, Cint_field_key_gen_var_de_qual_info_arr[qual_index]));

        printf("%d step: Create user defined qual(%d) \n", qual_index+1, qual_index+1);
    }

    param_info.param_type = bcmFieldContextParamTypeKeyVal;
    param_info.param_val = key_gen_val;
    param_info.param_arg = 0;

    BCM_IF_ERROR_RETURN(bcm_field_context_param_set(unit, 0, bcmFieldStageIngressPMF1, context_id, &param_info));

    printf("3 step: Set the context param KEY GEN VAR \n");

    bcm_field_group_info_t_init(&fg_info);

    fg_info.stage = stage;
    fg_info.fg_type = bcmFieldGroupTypeDirectExtraction;
    fg_info.nof_quals = 3;
    fg_info.qual_types[0] = Cint_field_key_gen_var_de_qual_info_arr[0].qual_id;
    fg_info.qual_types[1] = bcmFieldQualifyKeyGenVar;
    fg_info.qual_types[2] = Cint_field_key_gen_var_de_qual_info_arr[1].qual_id;
    fg_info.nof_actions = 2;
    fg_info.action_types[0] = bcmFieldActionUDHData0;
    fg_info.action_with_valid_bit[0] = FALSE;
    fg_info.action_types[1] = bcmFieldActionStackingRouteNew;
    fg_info.action_with_valid_bit[1] = FALSE;
    dest_char = &(fg_info.name[0]);
    sal_strncpy_s(dest_char, "key_gen_var_FG", sizeof(fg_info.name));

    BCM_IF_ERROR_RETURN(bcm_field_group_add(unit, 0, &fg_info, &Cint_field_key_gen_var_de_fg_id));

    printf("4 step: Add a DE FG (%d)\n", Cint_field_key_gen_var_de_fg_id);

    bcm_field_group_attach_info_t_init(&attach_info);

    attach_info.key_info.nof_quals = 3;

    attach_info.key_info.qual_types[0] = Cint_field_key_gen_var_de_qual_info_arr[0].qual_id;
    attach_info.key_info.qual_info[0].input_type = bcmFieldInputTypeLayerAbsolute;
    attach_info.key_info.qual_info[0].input_arg = 0;
    attach_info.key_info.qual_info[0].offset = 0;
    attach_info.key_info.qual_types[1] = bcmFieldQualifyKeyGenVar;
    attach_info.key_info.qual_info[1].input_type = bcmFieldInputTypeMetaData;
    attach_info.key_info.qual_types[2] = Cint_field_key_gen_var_de_qual_info_arr[1].qual_id;
    attach_info.key_info.qual_info[2].input_type = bcmFieldInputTypeLayerAbsolute;
    attach_info.key_info.qual_info[2].input_arg = 0;
    attach_info.key_info.qual_info[2].offset = 0;

    attach_info.payload_info.nof_actions = 2;
    attach_info.payload_info.action_types[0] = bcmFieldActionUDHData0;
    attach_info.payload_info.action_types[1] = bcmFieldActionStackingRouteNew;

    BCM_IF_ERROR_RETURN(bcm_field_group_context_attach(unit, 0, Cint_field_key_gen_var_de_fg_id, context_id, &attach_info));

    printf("5 step: Attach the FG (%d) to the context(%d))\n", Cint_field_key_gen_var_de_fg_id, context_id);
    return BCM_E_NONE;
}

/**
 * The destroy function of the test, to be called from the TCL test.
 * It is deleting the whole configuration.
 * \param [in] unit - The unit number.
 * \param [in] stage - The field stage.
 * \param [in] context_id - The context ID.
 *
 *  \return
 *  int - Error Type, in sense of bcm_error_t.
 */
int
cint_field_key_gen_var_de_destroy(
    int unit,
    bcm_field_context_t context_id)
{
    int qual_index;
    printf("Running: cint_field_key_gen_var_de_destroy()\n");

    BCM_IF_ERROR_RETURN(bcm_field_group_context_detach(unit, Cint_field_key_gen_var_de_fg_id, context_id));

    BCM_IF_ERROR_RETURN(bcm_field_group_delete(unit, Cint_field_key_gen_var_de_fg_id));

    for(qual_index = 0; qual_index < Cint_field_key_gen_var_de_nof_quals; qual_index++)
    {

        BCM_IF_ERROR_RETURN(bcm_field_qualifier_destroy(unit, Cint_field_key_gen_var_de_qual_info_arr[qual_index].qual_id));
    }
    return BCM_E_NONE;
}
