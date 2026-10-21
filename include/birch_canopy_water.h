#ifndef BIRCH_CANOPY_WATER_H
#define BIRCH_CANOPY_WATER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Input is metres per patch area; result is metres per stratum area.
 * ID 99 sheds its complete input to same-layer ID 98 strata.
 * Bare fraction keeps null_cover * incoming_m in the caller. */
static inline double birch_layer_water_input(double incoming_m,
    const struct patch_object *patch, const struct layer_object *layer,
    const struct canopy_strata_object *stratum)
{
    double panels = 0.0, receivers = 0.0, cover = layer->null_cover;
    for (int i = 0; i < layer->count; ++i) {
        const struct canopy_strata_object *s = patch->canopy_strata[layer->strata[i]];
        if (!isfinite(s->cover_fraction) || s->cover_fraction < 0.0) {
            fprintf(stderr, "Invalid canopy cover in patch %d\n", patch->ID);
            exit(EXIT_FAILURE);
        }
        cover += s->cover_fraction;
        if (s->defaults[0][0].ID == 99) panels += s->cover_fraction;
        if (s->defaults[0][0].ID == 98) receivers += s->cover_fraction;
    }
    if (!isfinite(cover) || !isfinite(layer->null_cover) ||
        layer->null_cover < 0.0 || fabs(cover - 1.0) > 1.0e-8 ||
        (panels > 0.0 && receivers <= 0.0)) {
        fprintf(stderr, "Invalid water allocation in patch %d, layer %.17g: "
            "cover=%.17g panel=%.17g open=%.17g; "
            "ID 99 requires positive ID 98 cover in the same layer\n",
            patch->ID, layer->height, cover, panels, receivers);
        exit(EXIT_FAILURE);
    }
    if (stratum->cover_fraction == 0.0 || stratum->defaults[0][0].ID == 99)
        return 0.0;
    if (stratum->defaults[0][0].ID == 98 && panels > 0.0)
        return incoming_m * (1.0 + panels / receivers);
    return incoming_m;
}
static inline const struct layer_object *birch_water_layer(
    const struct patch_object *patch, const struct canopy_strata_object *stratum)
{
    for (int i = 0; i < patch->num_layers; ++i)
        for (int j = 0; j < patch->layers[i].count; ++j)
            if (patch->canopy_strata[patch->layers[i].strata[j]] == stratum)
                return &patch->layers[i];
    fprintf(stderr, "Missing canopy water layer in patch %d\n", patch->ID);
    exit(EXIT_FAILURE);
}
#endif
