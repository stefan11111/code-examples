#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <gbm.h>
#include <drm_fourcc.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int main(int ac, char **av)
{
    int fd = open(ac >=2 ? av[1] : "/dev/dri/card0", O_RDWR);
    if (fd < 0) {
        return 0;
    }

    struct gbm_device *gbm = gbm_create_device(fd);
    if (!gbm) {
        return 0;
    }

    uint32_t handle = 0;
    uint32_t stride = 0;
    uint64_t size = 0;

    uint32_t width = 1920, height = 1080, bpp = 4;

    if (drmModeCreateDumbBuffer(fd, width, height, bpp, 0 /* flags */,
                                &handle, &stride, &size) || !handle) {
        return 0;
    }

    int prime_fd = -1;
    if (drmPrimeHandleToFD(fd, handle, DRM_RDWR, &prime_fd) || prime_fd < 0) {
        return 0;
    }


    struct gbm_import_fd_modifier_data data = {
        .width = width,
        .height = height,
        .format = GBM_FORMAT_XRGB8888,
        .num_fds = 1,
        .fds = {prime_fd, -1, -1, -1},
        .strides = {stride},
        .offsets = {0},
        .modifier = DRM_FORMAT_MOD_LINEAR,
    };

    struct gbm_bo *bo = gbm_bo_import(gbm, GBM_BO_IMPORT_FD_MODIFIER, &data, 0 /* flags */);
    if (!bo) {
        return 0;
    }

    gbm_bo_destroy(bo);

    /* The actual test */
    drmPrimeFDToHandle(fd, prime_fd, &handle);
    close(prime_fd);
    prime_fd = -1;
    if (drmPrimeHandleToFD(fd, handle, DRM_RDWR, &prime_fd)) {
        prime_fd = -1;
    }
    printf("prime_fd: %d\n", prime_fd);
}
