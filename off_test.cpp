#include <cstdint>
#include <cstdio>
#include <cstddef>

typedef struct voodoo_state_t {
        int xstart, xend, xdir;
        uint32_t base_r, base_g, base_b, base_a, base_z;
        struct {
                int64_t base_s, base_t, base_w;
                int lod;
        } tmu[2];
        int64_t base_w;
        int lod;
        int lod_min[2], lod_max[2];
        int dx1, dx2;
        int y, yend, ydir;
        int32_t dxAB, dxAC, dxBC;
        int tex_b[2], tex_g[2], tex_r[2], tex_a[2];
        int tex_s, tex_t;
        int clamp_s[2], clamp_t[2];
        int32_t vertexAx, vertexAy, vertexBx, vertexBy, vertexCx, vertexCy;
        uint32_t *tex[2][6];
        int tformat;
        int *tex_w_mask[2];
        int *tex_h_mask[2];
        int *tex_shift[2];
        int *tex_lod[2];
} voodoo_state_t;

typedef struct voodoo_params_t {
        int command;
        int32_t vertexAx;
        struct {
                int64_t startS, startT, startW, p1;
        } tmu[2];
        uint32_t color0;
        int *tex_w_mask[2];
        int *tex_h_mask[2];
} voodoo_params_t;

int main() {
    for (int tmu = 0; tmu < 2; tmu++) {
        // new-style arithmetic (what the fixed code computes at runtime)
        size_t new1 = offsetof(voodoo_state_t, tmu[0].lod) + (size_t)tmu * sizeof(((voodoo_state_t*)0)->tmu[0]);
        size_t new2 = offsetof(voodoo_state_t, lod_min[0]) + (size_t)tmu * sizeof(((voodoo_state_t*)0)->lod_min[0]);
        size_t new3 = offsetof(voodoo_state_t, tex[0]) + (size_t)tmu * sizeof(((voodoo_state_t*)0)->tex[0]);
        size_t new4 = offsetof(voodoo_params_t, tex_w_mask[0]) + (size_t)tmu * sizeof(((voodoo_params_t*)0)->tex_w_mask[0]);
        // reference: offsetof with constant index per tmu value
        size_t ref1 = tmu ? offsetof(voodoo_state_t, tmu[1].lod) : offsetof(voodoo_state_t, tmu[0].lod);
        size_t ref2 = tmu ? offsetof(voodoo_state_t, lod_min[1]) : offsetof(voodoo_state_t, lod_min[0]);
        size_t ref3 = tmu ? offsetof(voodoo_state_t, tex[1]) : offsetof(voodoo_state_t, tex[0]);
        size_t ref4 = tmu ? offsetof(voodoo_params_t, tex_w_mask[1]) : offsetof(voodoo_params_t, tex_w_mask[0]);
        printf("tmu=%d: %s %s %s %s\n", tmu,
               new1==ref1?"ok":"FAIL", new2==ref2?"ok":"FAIL", new3==ref3?"ok":"FAIL", new4==ref4?"ok":"FAIL");
    }
    return 0;
}
