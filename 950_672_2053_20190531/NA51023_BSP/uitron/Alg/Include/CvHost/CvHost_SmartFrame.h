#ifndef _CVHOST_SMARTFRAME_H_
#define _CVHOST_SMARTFRAME_H_


#define CVHOST_CEVA_VER_MAJOR         1  // 1 // 1
#define CVHOST_CEVA_VER_MINOR         16 // 9 // 8
#define CVHOST_CEVA_VER_BUILD         0  // 3 // 0


/// \brief Frame buffer structure for a single color plane (or packed format)
typedef struct frame_buf_
{
    UINT8* data;       ///< pointer to image data
    UINT16 width;      ///< width (in elements)
    UINT16 height;     ///< height
    UINT16 stride;     ///< stride (bytes)
    UINT16 elem_size;  ///< element size (bytes)
    UINT16 id;         ///< buffer identifier (application-dependent)
    UINT16 padding;    // padding for DWORD alignment
} frame_buf_t;


/**
* \brief Kernel index
*/
typedef enum
{
    SMARTFRAME_KERNEL_INDEX_0 = 0,          //!< Index to kernel 0
    SMARTFRAME_KERNEL_INDEX_1 = 1,          //!< Index to kernel 1
    SMARTFRAME_KERNEL_INDEX_2 = 2,          //!< Index to kernel 2
    ENUM_DUMMY4WORD(sf_kernel_index_e)
} sf_kernel_index_e;


/**
 *  \brief Data types
 */
typedef enum
{
    SMARTFRAME_DATA_TYPE_U8  = 0x0001,      //!< Unsigned 8 bits data type
    SMARTFRAME_DATA_TYPE_S8  = 0x0002,      //!< Signed 8 bits data type
    SMARTFRAME_DATA_TYPE_U16 = 0x0004,      //!< Unsigned 16 bits data type
    SMARTFRAME_DATA_TYPE_S16 = 0x0008,      //!< Signed 16 bits data type
    SMARTFRAME_DATA_TYPE_U32 = 0x0010,      //!< Unsigned 32 bits data type
    SMARTFRAME_DATA_TYPE_S32 = 0x0020,      //!< Signed 32 bits data type
    ENUM_DUMMY4WORD(sf_data_type_e)
} sf_data_type_e;


/**
 * \brief the index of the parameter in a kernel
 */
typedef enum
{
    SMARTFRAME_SEQ_PADD_TYPE    = 0,        //!< Index parameter 0
    SMARTFRAME_SEQ_PADD_VALUE   = 1,        //!< Index parameter 1
    SMARTFRAME_SEQ_PRIORITY     = 2,        //!< Index parameter 2
    ENUM_DUMMY4WORD(sf_param_seq_e)
} sf_param_seq_e;


/**
 * \brief Padding type
 */
typedef enum
{
    SMARTFRAME_PADDING_REPLICATE = 0,       //!< aaaaaa|abcdefgh|hhhhhhh
    SMARTFRAME_PADDING_MIRROR    = 1,       //!< fedcba|abcdefgh|hgfedcb
    SMARTFRAME_PADDING_CONSTANT  = 2        //!< iiiiii|abcdefgh|iiiiiii  with some specified 'i'
} sf_padding_type_e;


/**
 * \brief the index of the parameter in a kernel
 */
typedef enum
{
    SMARTFRAME_PARAM_0  = 0,                //!< Index parameter 0
    SMARTFRAME_PARAM_1  = 1,                //!< Index parameter 1
    SMARTFRAME_PARAM_2  = 2,                //!< Index parameter 2
    SMARTFRAME_PARAM_3  = 3,                //!< Index parameter 3
    SMARTFRAME_PARAM_4  = 4,                //!< Index parameter 4
    SMARTFRAME_PARAM_5  = 5,                //!< Index parameter 5
    SMARTFRAME_PARAM_6  = 6,                //!< Index parameter 6
    SMARTFRAME_PARAM_7  = 7,                //!< Index parameter 7
    SMARTFRAME_PARAM_8  = 8,                //!< Index parameter 8
    SMARTFRAME_PARAM_9  = 9,                //!< Index parameter 9
    ENUM_DUMMY4WORD(sf_param_num_e)
} sf_param_num_e;


#endif
