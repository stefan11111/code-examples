/* gcc gbm-bo-dup.c -o gbm-bo-dup $(pkg-config --libs --cflags gbm) -fsanitize=address -O0 -ggdb3 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <gbm.h>

static struct gbm_bo*
gbm_bo_dup(struct gbm_bo *bo, uint32_t usage)
{
if (!bo) exit(0);
    struct gbm_device *gbm;
    struct gbm_bo *ret;

    int fds[GBM_MAX_PLANES];

    struct gbm_import_fd_modifier_data data = {
        .width = gbm_bo_get_width(bo),
        .height = gbm_bo_get_height(bo),
        .format = gbm_bo_get_format(bo),
        .fds = {-1, -1, -1, -1},
        .modifier = gbm_bo_get_modifier(bo),
    };

    gbm = gbm_bo_get_device(bo);

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
    int fd = -1;
    struct gbm_device *gbm = NULL;
    struct gbm_bo *orig = NULL, *copy = NULL;

    uint32_t w = 1920, h = 1080, fmt = GBM_FORMAT_XRGB8888;

    int cnt = 0;
    uint32_t usage;

    if ((fd = open(ac >= 2 ? av[1] : "/dev/dri/card0", O_RDWR)) < 0) {
        goto err;
    }

    if (!(gbm = gbm_create_device(fd))) {
        goto err;
    }
again:
    if (cnt < 100) {
        usage = GBM_BO_USE_WRITE;
    } else if (cnt < 200) {
        usage = GBM_BO_USE_SCANOUT;
    } else {
        usage = GBM_BO_USE_RENDERING;
    }
usage = GBM_BO_USE_RENDERING;
    if (!(orig = gbm_bo_create(gbm, w, h, fmt, usage))) {
        goto err;
    }

    if (!(copy = gbm_bo_dup(orig, 0 /* flags */))) {
        goto err;
    }
printf("TEST 1\n");
    /* Destroy the copy, try to make a new one from the original */
    gbm_bo_destroy(copy);
printf("DESTROY\n");
    copy = gbm_bo_dup(orig, 0 /* flags */);
    printf("copy: %p\n", copy);
printf("TEST 2\n");
    /* Destroy the orig, try to re-create if from the copy */
    gbm_bo_destroy(orig);
    orig = gbm_bo_dup(copy, 0 /* flags */);
    printf("orig: %p\n", orig);
printf("END\n");
    /* Keep track of the handle, we don't want it to increase */
    uint32_t handle = gbm_bo_get_handle_for_plane(orig, 0).u32;
    printf("handle: %u\n", handle);
    handle = gbm_bo_get_handle(orig).u32;
    printf("handle: %u\n", handle);

err:
    if (copy) {
        gbm_bo_destroy(copy);
    }
    if (orig) {
        gbm_bo_destroy(orig);
    }
    if (++cnt < 1000) {
//        goto again;
    }
    if (gbm) {
        gbm_device_destroy(gbm);
    }
    if (fd >= 0) {
        close(fd);
    }
}
