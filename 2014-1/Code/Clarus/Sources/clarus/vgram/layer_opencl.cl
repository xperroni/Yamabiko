constant uint m1 = 0x55555555;
constant uint m2 = 0x33333333;
constant uint m4 = 0x0f0f0f0f;

// See http://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
uint hamming_weight(uint4 x) {
    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += (x >> 8);
    x += (x >> 16);
    return
        (x.x & 0x7f) +
        (x.y & 0x7f) +
        (x.z & 0x7f) +
        (x.w & 0x7f)
    ;
}

// Common sampler used for all image operations performed on this program.
constant sampler_t s = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;

// Length of bit strings.
constant int R = 128;

kernel void query_layer(
    global uint4 *b,
    global uint4 *I,
    global uchar *O,
    global uint *y,
    global uint *d
) {
    uint i = get_global_id(0);
    uint j = get_global_id(1);
    uint k = get_global_id(2);

    uint w = get_global_size(0);
    uint h = get_global_size(1);
    uint z = w * h;

    uint u = i + j * w;
    uint v = u + k * z;

    uint4 bk = b[u];
    uint4 bz = I[v];
    uint e = hamming_weight(bk ^ bz);

    if (atomic_min(d + u, e) > e) {
        atomic_xchg(y + u, O[v]);
    }
}

kernel void setup_d0(global uint *d) {
    uint i = get_global_id(0);
    uint j = get_global_id(1);
    uint w = get_global_size(0);
    d[i + j * w] = R;
}
