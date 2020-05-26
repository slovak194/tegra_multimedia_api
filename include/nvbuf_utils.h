/*
 * Copyright (c) 2016-2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/**
 * @file
 * <b>NVIDIA Multimedia Utilities: Buffering and Transform/Composition/Blending</b>
 *
 */

/**
 * @defgroup ee_nvbuffering_group Buffer Manager
 * @ingroup common_utility_group
 * NVIDIA buffering utility library for use by applications.
 * The utility also transforms, composits, and blends.
 * @{
 */

#ifndef _NVBUF_UTILS_H_
#define _NVBUF_UTILS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <errno.h>

/**
 * Defines the maximum number of planes for video frame.
 */
#define MAX_NUM_PLANES 3

/**
 * Defines the maximum number of input video frames that can be used for composition.
 */
#define MAX_COMPOSITE_FRAME 16

/**
 * Defines Payload types for NvBuffer.
 */
typedef enum
{
  /** buffer payload with HW memory handle for set of planes. */
  NvBufferPayload_SurfArray,
  /** buffer payload with HW memory handle for specific memory size. */
  NvBufferPayload_MemHandle,
} NvBufferPayloadType;

/**
 * Defines Layout formats for NvBuffer video planes.
 */
typedef enum
{
  /** Pitch Layout. */
  NvBufferLayout_Pitch,
  /** BlockLinear Layout. */
  NvBufferLayout_BlockLinear,
} NvBufferLayout;

/**
 * Defines memory access flags for NvBuffer.
 */
typedef enum
{
  /** Memory read. */
  NvBufferMem_Read,
  /** Memory write. */
  NvBufferMem_Write,
  /** Memory read & write. */
  NvBufferMem_Read_Write,
} NvBufferMemFlags;

/**
 * Defines tags to associate, while allocation of NvBuffer.
 */
typedef enum
{
  /** tag None. */
  NvBufferTag_NONE            = 0x0,
  /** tag for Camera. */
  NvBufferTag_CAMERA          = 0x200,
  /** tag for Jpeg Encoder/Decoder. */
  NvBufferTag_JPEG            = 0x1500,
  /** tag for H264/H265 Video Encoder. */
  NvBufferTag_VIDEO_ENC       = 0x1200,
  /** tag for H264/H265/VP9 Video Decoder. */
  NvBufferTag_VIDEO_DEC       = 0x1400,
  /** tag for Video Transform/Composite. */
  NvBufferTag_VIDEO_CONVERT   = 0xf01,
} NvBufferTag;

/**
 * Defines color formats for NvBuffer.
 */
typedef enum
{
  /** BT.601 colorspace - YUV420 multi-planar. */
  NvBufferColorFormat_YUV420,
  /** BT.601 colorspace - YUV420 multi-planar. */
  NvBufferColorFormat_YVU420,
  /** BT.601 colorspace - YUV420 ER multi-planar. */
  NvBufferColorFormat_YUV420_ER,
  /** BT.601 colorspace - YVU420 ER multi-planar. */
  NvBufferColorFormat_YVU420_ER,
  /** BT.601 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NvBufferColorFormat_NV12,
  /** BT.601 colorspace - Y/CbCr ER 4:2:0 multi-planar. */
  NvBufferColorFormat_NV12_ER,
  /** BT.601 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NvBufferColorFormat_NV21,
  /** BT.601 colorspace - Y/CbCr ER 4:2:0 multi-planar. */
  NvBufferColorFormat_NV21_ER,
  /** BT.601 colorspace - YUV 4:2:2 planar. */
  NvBufferColorFormat_UYVY,
  /** BT.601 colorspace - YUV ER 4:2:2 planar. */
  NvBufferColorFormat_UYVY_ER,
  /** BT.601 colorspace - YUV 4:2:2 planar. */
  NvBufferColorFormat_VYUY,
  /** BT.601 colorspace - YUV ER 4:2:2 planar. */
  NvBufferColorFormat_VYUY_ER,
  /** BT.601 colorspace - YUV 4:2:2 planar. */
  NvBufferColorFormat_YUYV,
  /** BT.601 colorspace - YUV ER 4:2:2 planar. */
  NvBufferColorFormat_YUYV_ER,
  /** BT.601 colorspace - YUV 4:2:2 planar. */
  NvBufferColorFormat_YVYU,
  /** BT.601 colorspace - YUV ER 4:2:2 planar. */
  NvBufferColorFormat_YVYU_ER,
  /** LegacyRGBA colorspace - BGRA-8-8-8-8 planar. */
  NvBufferColorFormat_ABGR32,
  /** LegacyRGBA colorspace - XRGB-8-8-8-8 planar. */
  NvBufferColorFormat_XRGB32,
  /** LegacyRGBA colorspace - ARGB-8-8-8-8 planar. */
  NvBufferColorFormat_ARGB32,
  /** BT.601 colorspace - Y/CbCr 4:2:0 10-bit multi-planar. */
  NvBufferColorFormat_NV12_10LE,
  /** BT.601 colorspace - Y/CrCb 4:2:0 10-bit multi-planar. */
  NvBufferColorFormat_NV21_10LE,
  /** BT.601 colorspace - Y/CbCr 4:2:0 12-bit multi-planar. */
  NvBufferColorFormat_NV12_12LE,
  /** BT.601 colorspace - Y/CrCb 4:2:0 12-bit multi-planar. */
  NvBufferColorFormat_NV21_12LE,
  /** BT.709 colorspace - YUV420 multi-planar. */
  NvBufferColorFormat_YUV420_709,
  /** BT.709 colorspace - YUV420 ER multi-planar. */
  NvBufferColorFormat_YUV420_709_ER,
  /** BT.709 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NvBufferColorFormat_NV12_709,
  /** BT.709 colorspace - Y/CbCr ER 4:2:0 multi-planar. */
  NvBufferColorFormat_NV12_709_ER,
  /** BT.2020 colorspace - YUV420 multi-planar. */
  NvBufferColorFormat_YUV420_2020,
  /** BT.2020 colorspace - Y/CbCr 4:2:0 multi-planar. */
  NvBufferColorFormat_NV12_2020,
  /** Optical flow */
  NvBufferColorFormat_SignedR16G16,
  /** BT.601 colorspace - YUV444 multi-planar. */
  NvBufferColorFormat_YUV444,
  /** Invalid color format. */
  NvBufferColorFormat_Invalid,
} NvBufferColorFormat;

/**
 * Defines video flip methods.
 */
typedef enum
{
  /** Video flip none. */
  NvBufferTransform_None,
  /** Video flip rotate 90 degree clockwise. */
  NvBufferTransform_Rotate90,
  /** Video flip rotate 180 degree clockwise. */
  NvBufferTransform_Rotate180,
  /** Video flip rotate 270 degree clockwise. */
  NvBufferTransform_Rotate270,
  /** Video flip with respect to X-axis. */
  NvBufferTransform_FlipX,
  /** Video flip with respect to Y-axis. */
  NvBufferTransform_FlipY,
  /** Video flip transpose. */
  NvBufferTransform_Transpose,
  /** Video flip inverse transpode. */
  NvBufferTransform_InvTranspose,
} NvBufferTransform_Flip;

/**
 * Defines transform video filter types.
 */
typedef enum
{
  /** transform filter nearest. */
  NvBufferTransform_Filter_Nearest,
  /** transform filter bilinear. */
  NvBufferTransform_Filter_Bilinear,
  /** transform filter 5 tap. */
  NvBufferTransform_Filter_5_Tap,
  /** transform filter 10 tap. */
  NvBufferTransform_Filter_10_Tap,
  /** transform filter smart. */
  NvBufferTransform_Filter_Smart,
  /** transform filter nicest. */
  NvBufferTransform_Filter_Nicest,
} NvBufferTransform_Filter;

/**
 * Defines flags to indicate for valid transform.
 */
typedef enum {
  /** transform flag to crop source rectangle. */
  NVBUFFER_TRANSFORM_CROP_SRC   = 1,
  /** transform flag to crop destination rectangle. */
  NVBUFFER_TRANSFORM_CROP_DST   = 1 << 1,
  /** transform flag to set filter type. */
  NVBUFFER_TRANSFORM_FILTER     = 1 << 2,
  /** transform flag to set flip method. */
  NVBUFFER_TRANSFORM_FLIP       = 1 << 3,
} NvBufferTransform_Flag;

/**
 * Defines flags that specify valid composition/blending operations.
 */
typedef enum {
  /** flag to set for composition. */
  NVBUFFER_COMPOSITE  = 1,
  /** flag to set for blending. */
  NVBUFFER_BLEND      = 1 << 1,
} NvBufferComposite_Flag;

/**
 * Holds composition background r,g,b colors.
 */
typedef struct
{
  /** background color value for r. */
  float r;
  /** background color value for g. */
  float g;
  /** background color value for b. */
  float b;
}NvBufferCompositeBackground;

/**
 * Holds coordinates for a rectangle.
 */
typedef struct
{
  /** rectangle top. */
  uint32_t top;
  /** rectangle left. */
  uint32_t left;
  /** rectangle width. */
  uint32_t width;
  /** rectangle height. */
  uint32_t height;
}NvBufferRect;

/**
 * Holds an opaque NvBuffer session type required for parallel buffer
 * tranformations and compositions. Operations using a single session are
 * scheduled sequentially, after the previous operation finishes. Operations for
 * multiple sessions are scheduled in parallel.
 */
typedef struct _NvBufferSession * NvBufferSession;

/**
 * Holds the input parameters for hardware buffer creation.
 */
typedef struct _NvBufferCreateParams
{
  /** width of the buffer. */
  int32_t width;
  /** height of the buffer. */
  int32_t height;
  /** payload type of the buffer. */
  NvBufferPayloadType payloadType;
  /** size of the memory.(Applicale for NvBufferPayload_MemHandle) */
  int32_t memsize;
  /** layout of the buffer. */
  NvBufferLayout layout;
  /** colorformat of the buffer. */
  NvBufferColorFormat colorFormat;
  /** tag to associate with the buffer. */
  NvBufferTag nvbuf_tag;
}NvBufferCreateParams;

/**
 * Holds parameters for a hardware buffer.
 */
typedef struct _NvBufferParams
{
  /** dmabuf_fd of hardware buffer. */
  uint32_t dmabuf_fd;
  /** pointer to hardware buffer memory. */
  void *nv_buffer;
  /** payload type of the buffer. */
  NvBufferPayloadType payloadType;
  /** size of the memory.(Applicale for NvBufferPayload_MemHandle) */
  int32_t memsize;
  /** size of hardware buffer. */
  uint32_t nv_buffer_size;
  /** video format type of hardware buffer. */
  NvBufferColorFormat pixel_format;
  /** number of planes of hardware buffer. */
  uint32_t num_planes;
  /** width of each planes of hardware buffer. */
  uint32_t width[MAX_NUM_PLANES];
  /** height of each planes of hardware buffer. */
  uint32_t height[MAX_NUM_PLANES];
  /** pitch of each planes of hardware buffer. */
  uint32_t pitch[MAX_NUM_PLANES];
  /** memory offset values of each video planes of hardware buffer. */
  uint32_t offset[MAX_NUM_PLANES];
  /** size of each vodeo planes of hardware buffer. */
  uint32_t psize[MAX_NUM_PLANES];
  /** layout type of each planes of hardware buffer. */
  uint32_t layout[MAX_NUM_PLANES];
}NvBufferParams;

/**
 * Holds parameters related to compositing/blending.
 */
typedef struct _NvBufferCompositeParams
{
  /** flag to indicate which of the composition/blending parameters are valid. */
  uint32_t composite_flag;
  /** number of the input buffers to be composited. */
  uint32_t input_buf_count;
  /** alpha values of input buffers for the blending. */
  float dst_comp_rect_alpha[MAX_COMPOSITE_FRAME];
  /** source rectangle coordinates of input buffers for composition. */
  NvBufferRect src_comp_rect[MAX_COMPOSITE_FRAME];
  /** destination rectangle coordinates of input buffers for composition. */
  NvBufferRect dst_comp_rect[MAX_COMPOSITE_FRAME];
  /** background color values for composition. */
  NvBufferCompositeBackground composite_bgcolor;
  /** NvBufferSession to be used for composition. If NULL, the default session
   * is used. */
  NvBufferSession session;
}NvBufferCompositeParams;

/**
 * Holds parameters for buffer transform functions.
 */
typedef struct _NvBufferTransformParams
{
  /** flag to indicate which of the transform parameters are valid. */
  uint32_t transform_flag;
  /** flip method. */
  NvBufferTransform_Flip transform_flip;
  /** transform filter. */
  NvBufferTransform_Filter transform_filter;
  /** source rectangle coordinates for crop opeartion. */
  NvBufferRect src_rect;
  /** destination rectangle coordinates for crop opeartion. */
  NvBufferRect dst_rect;
  /** NvBufferSession to be used for transform. If NULL, the default session
   * is used. */
  NvBufferSession session;
}NvBufferTransformParams;


/**
* This method can be used to get HW Buffer struct size.
*
* @returns HW Buffer struct size.
*/
int NvBufferGetSize (void);

/**
* Creates an `EGLImage` instance from `dmabuf-fd`.
*
* @param[in] display `EGLDisplay` object used during the creation of `EGLImage`.
*                    If NULL, the nvbuf_utils API uses its own EGLDisplay instance.
* @param[in] dmabuf_fd `DMABUF FD` of buffer from which `EGLImage` to be created.
*
* @returns `EGLImageKHR` for success, `NULL` for failure
*/
EGLImageKHR NvEGLImageFromFd (EGLDisplay display, int dmabuf_fd);

/**
* Destroys an `EGLImage` object.

* @param[in] display `EGLDisplay` object used to destroy `EGLImage`.
*                    If NULL, the nvbuf_utils API uses its own EGLDisplay instance.
* @param[in] eglImage `EGLImageKHR` object to be destroyed.
*
* @returns 0 for success, -1 for failure
*/
int NvDestroyEGLImage (EGLDisplay display, EGLImageKHR eglImage);

/**
 * Allocates a HW buffer (deprecated).
 * @deprecated Instead use NvBufferCreateEx().
 * @param[out] dmabuf_fd Returns `dmabuf_fd` of hardware buffer.
 * @param[in] width Hardware buffer width, in bytes.
 * @param[in] height Hardware buffer height, in bytes.
 * @param[in] layout Layout of buffer.
 * @param[in] colorFormat The `colorFormat` of buffer.
 *
 * @returns 0 for success, -1 for failure
 */
int NvBufferCreate (int *dmabuf_fd, int width, int height,
    NvBufferLayout layout, NvBufferColorFormat colorFormat);

/**
 * Allocates a HW buffer.
 * @param[out] dmabuf_fd Returns `dmabuf_fd` of hardware buffer.
 * @param[in] input_params Input parameters for hardware buffer creation.
 *
 * @returns 0 for success, -1 for failure
 */
int NvBufferCreateEx (int *dmabuf_fd, NvBufferCreateParams *input_params);

/**
 * Gets buffer parameters.
 * @param[in] dmabuf_fd `DMABUF FD` of buffer.
 * @param[out] params A pointer to the structure to fill with parameters.
 *
 * @returns 0 for success, -1 for failure.
 */
int NvBufferGetParams (int dmabuf_fd, NvBufferParams *params);

/**
* Destroys a HW buffer.
* @param[in] dmabuf_fd Specifies the `dmabuf_fd` `hw_buffer` to destroy.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferDestroy (int dmabuf_fd);

/**
* Extracts the `dmabuf_fd` from the hardware buffer.
* @param[in] nvbuf Specifies the `hw_buffer`.
* @param[out] dmabuf_fd Returns DMABUF FD of `hw_buffer`.
*
* @returns 0 for success, -1 for failure.
*/
int ExtractFdFromNvBuffer (void *nvbuf, int *dmabuf_fd);

/**
* Releases the `dmabuf_fd` buffer.
* @see ExtractfdFromNvBuffer()
* @param[in] dmabuf_fd Specifies the `dmabuf_fd` to release.
*
* @returns 0 for success, -1 for failure.
*/
int NvReleaseFd (int dmabuf_fd);

/**
* Syncs the HW memory cache for the CPU.
*
* \sa NvBufferMemMap for the purpose of the function
*
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.
* @param[in] pVirtAddr Virtual Address pointer of the memory-mapped plane.
*
* @returns 0 for success, -1 for failure.

*/
int NvBufferMemSyncForCpu (int dmabuf_fd, unsigned int plane, void **pVirtAddr);

/**
* Syncs the HW memory cache sync for the device.
*
* \sa NvBufferMemMap for the purpose of the function
*
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.
* @param[in] pVirtAddr Virtual Address pointer of the memory-mapped plane.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferMemSyncForDevice (int dmabuf_fd, unsigned int plane, void **pVirtAddr);

/**
* Gets the memory-mapped virtual address of the plane.
*
* The client must call NvBufferMemSyncForCpu() with the virtual address returned
* by this function before accessing the mapped memory in CPU.
*
* After memory mapping is complete, mapped memory modification
* must be coordinated between the CPU and hardware device as
* follows:
* - CPU: If the CPU modifies any mapped memory, the client must call
*   NvBufferMemSyncForDevice() before any hardware device accesses the memory.
* - Hardware device: If the mapped memory is modified by any hardware device,
*   the client must call NvBufferMemSyncForCpu() before CPU accesses the memory.
*
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.(Applies to @ref NvBufferPayload_SurfArray.)
* @param[in] memflag NvBuffer memory flag.
* @param[out] pVirtAddr Virtual Address pointer of the memory-mapped plane.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferMemMap (int dmabuf_fd, unsigned int plane, NvBufferMemFlags memflag, void **pVirtAddr);

/**
* Unmaps the mapped virtual address of the plane.
*
* If the following conditions are both true, the client must call
* NvBufferMemSyncForDevice() before unmapping the memory:
* - Mapped memory was modified by the CPU.
* - Mapped memory will be accessed by a hardware device.
*
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.(Applies to @ref NvBufferPayload_SurfArray.)
* @param[in] pVirtAddr Memory-mapped Virtual Address pointer of the plane.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferMemUnMap (int dmabuf_fd, unsigned int plane, void **pVirtAddr);

/**
* Copies the NvBuffer plane contents to a raw buffer plane.
* @param[in] dmabuf_fd DMABUF FD of NvBuffer.
* @param[in] plane video frame plane.
* @param[in] out_width aligned width of the raw data plane.
* @param[in] out_height aligned height of the raw data plane.
* @param[in] ptr pointer to the output raw plane data.
*
* @returns 0 for success, -1 for failure.
*/
int NvBuffer2Raw (int dmabuf_fd, unsigned int plane, unsigned int out_width, unsigned int out_height, unsigned char *ptr);

/**
* Copies raw buffer plane contents to an NvBuffer plane.
* @param[in] ptr pointer to the input raw plane data.
* @param[in] plane video frame plane.
* @param[in] in_width aligned width of the raw data plane.
* @param[in] in_height aligned height of the raw data plane.
* @param[in] dmabuf_fd DMABUF FD of NvBuffer.
*
* @returns 0 for success, -1 for failure.
*/
int Raw2NvBuffer (unsigned char *ptr, unsigned int plane, unsigned int in_width, unsigned int in_height, int dmabuf_fd);

/**
* Creates a new NvBufferSession for parallel scheduling of
* buffer transformations and compositions.
*
* @returns A session pointer, NULL for failure.
*/
NvBufferSession NvBufferSessionCreate(void);

/**
* Destroys an exisiting NvBufferSession.
* @param[in] session An existing NvBufferSession.
*/
void NvBufferSessionDestroy(NvBufferSession session);

/**
 * Transforms one DMA buffer to another DMA buffer.
 * This function can support transforms for copying, scaling, fliping, rotating, and cropping.
 * @param[in] src_dmabuf_fd DMABUF FD of source buffer
 * @param[in] dst_dmabuf_fd DMABUF FD of destination buffer
 * @param[in] transform_params transform parameters
 *
 * @return 0 for sucess, -1 for failure.
 */
int NvBufferTransform (int src_dmabuf_fd, int dst_dmabuf_fd, NvBufferTransformParams *transform_params);

/**
 * Composites multiple input DMA buffers to one output DMA buffer.
 * This function can support composition of multiple input frames to one composited output.
 * @param[in] src_dmabuf_fds array of DMABUF FDs of source buffers to composite from
 * @param[in] dst_dmabuf_fd DMABUF FD of destination buffer for composition
 * @param[in] composite_params composition parameters
 *
 * @return 0 for success, -1 for failure.
 */
int NvBufferComposite (int *src_dmabuf_fds, int dst_dmabuf_fd, NvBufferCompositeParams *composite_params);

#ifdef __cplusplus
}
#endif

/** @} */

#endif