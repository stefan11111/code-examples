#include <stdlib.h>

typedef struct _RegData {
    long        size;
    long        numRects;
} RegDataRec, *RegDataPtr;

#define xfreeData(reg) if ((reg)->data && (reg)->data->size) free((reg)->data)

static RegDataRec miEmptyData = {0, 0};

struct _Region {
    int     extents;
    RegDataPtr  data;
};

typedef struct _Region RegionRec, *RegionPtr;

static RegionRec region;
static RegionPtr preg = &region;

int main()
{
    preg->data = &miEmptyData;
    xfreeData(preg);
    return preg->extents;
}
