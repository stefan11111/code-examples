/* gcc gbm-import-test.c -o gbm-import-test $(pkg-config --libs --cflags libdrm gbm)  -O0 -ggdb3 -fsanitize=address */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <gbm.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>

static struct gbm_bo*
gbm_bo_dup(struct gbm_device *gbm, struct gbm_bo *bo, uint32_t usage)
{
    struct gbm_bo *ret;

    int fds[GBM_MAX_PLANES];

    struct gbm_import_fd_modifier_data data = {
        .width = gbm_bo_get_width(bo),
        .height = gbm_bo_get_height(bo),
        .format = gbm_bo_get_format(bo),
        .fds = {-1, -1, -1, -1},
        .modifier = gbm_bo_get_modifier(bo),
    };

    data.num_fds = gbm_bo_get_plane_count(bo);
    for (int i = 0; i < data.num_fds; i++) {
        data.fds[i] = gbm_bo_get_fd_for_plane(bo, i);
        fds[i] = data.fds[i];
        data.strides[i] = gbm_bo_get_stride_for_plane(bo, i);
        data.offsets[i] = gbm_bo_get_offset(bo, i);
    }

    ret = gbm_bo_import(gbm, GBM_BO_IMPORT_FD_MODIFIER, &data, usage);
    for (int i = 0; i < data.num_fds; i++) {
        close(fds[i]);
    }
    return ret;
}

int main(int ac, char **av)
{
    struct gbm_device *gbm = NULL;
    struct gbm_bo *bo = NULL;
    int fd = -1;

    int cnt = 0;
    const int tresh = 1;

    const uint32_t w = 1920, h = 1080, fmt = GBM_FORMAT_XRGB8888, usage = GBM_BO_USE_SCANOUT, bpp = 32;

    uint32_t handle = 0, stride = 0;
    uint64_t size = 0;
    int dma_buf = -1;

    if ((fd = open(ac >= 2 ? av[1] : "/dev/dri/card0", O_RDWR)) < 0) goto err;
    if (!(gbm = gbm_create_device(fd))) goto err;

again:
    bo = NULL;
    handle = 0;
    stride = 0;
    size = 0;
    dma_buf = -1;

    /* Create a gem buffer */
    if (drmModeCreateDumbBuffer(fd, w, h, bpp, 0 /* flags */,
                                &handle, &stride, &size)) {
        goto err;
    }

    /* Export it as dma bufs */
    if (drmPrimeHandleToFD(fd, handle, DRM_RDWR, &dma_buf)) {
        dma_buf = -1;
        drmCloseBufferHandle(fd, handle);
        handle = 0;
        goto err;
    }

    /* Import it into gbm */
    struct gbm_import_fd_modifier_data data = {
        .width = w,
        .height = h,
        .format = fmt,
        .num_fds = 1,
        .fds = {dma_buf, -1, -1, -1},
        .strides[0] = stride,
        .modifier = DRM_FORMAT_MOD_LINEAR,
    };
    bo = gbm_bo_import(gbm, GBM_BO_IMPORT_FD_MODIFIER, &data, 0 /* usage */);
    if (!bo) {
        close(dma_buf);
        dma_buf = -1;
        drmCloseBufferHandle(fd, handle);
        handle = 0;
        goto err;
    }

    /* Drop our references to the gem buffer */
    close(dma_buf);
    dma_buf = -1;
    drmCloseBufferHandle(fd, handle);
    handle = 0;

    /* Try to use the imported buffer */
    handle = gbm_bo_get_handle(bo).u32;
    if (drmPrimeHandleToFD(fd, handle, DRM_RDWR, &dma_buf)) dma_buf = -1;

    printf("Handle: %u, dma_buf: %d\n", handle, dma_buf);

err:
    if (dma_buf >= 0) {
        close(dma_buf);
    }
    if (bo) {
        gbm_bo_destroy(bo);
    }

    if (++cnt < tresh) {
        goto again;
    }

    if (gbm) {
        gbm_device_destroy(gbm);
    }
    if (fd >= 0) {
        close(fd);
    }
    return 0;
}
