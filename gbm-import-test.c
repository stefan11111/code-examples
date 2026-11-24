/* gcc gbm-import-test.c -o gbm-import-test $(pkg-config --libs --cflags libdrm gbm)  -O0 -ggdb3 -fsanitize=address */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <gbm.h>
#include <xf86drm.h>

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
    struct gbm_device *gbm1 = NULL, *gbm2 = NULL;
    struct gbm_bo *bo1 = NULL, *bo2 = NULL;
    int fd = -1;

    int cnt = 0;
    const int tresh = 1;

    const uint32_t w = 1920, h = 1080, fmt = GBM_FORMAT_XRGB8888, usage = GBM_BO_USE_SCANOUT;

    uint32_t handle = 0;
    int dma_buf = -1;

    if ((fd = open(ac >= 2 ? av[1] : "/dev/dri/card0", O_RDWR)) < 0) goto err;
    if (!(gbm1 = gbm_create_device(fd))) goto err;
    if (!(gbm2 = gbm_create_device(fd))) goto err;

again:
    bo1 = NULL;
    bo2 = NULL;
    handle = 0;
    dma_buf = -1;

    if (!(bo1 = gbm_bo_create(gbm1, w, h, fmt, usage))) goto err;
    if (!(bo2 = gbm_bo_dup(gbm2, bo1, 0))) goto err;

printf("Before Destroy\n");
    gbm_bo_destroy(bo2);
printf("After Destroy\n");
    bo2 = NULL;
    handle = gbm_bo_get_handle(bo1).u32;
    if (drmPrimeHandleToFD(fd, handle, DRM_RDWR, &dma_buf)) dma_buf = -1;

    printf("Handle: %u, dma_buf: %d\n", handle, dma_buf);

err:
    if (dma_buf >= 0) {
        close(dma_buf);
    }
    if (bo2) {
        gbm_bo_destroy(bo2);
    }
    if (bo1) {
        gbm_bo_destroy(bo1);
    }

    if (++cnt < tresh) {
        goto again;
    }

    if (gbm2) {
        gbm_device_destroy(gbm2);
    }
    if (gbm1) {
        gbm_device_destroy(gbm1);
    }
    if (fd >= 0) {
        close(fd);
    }
    return 0;
}
