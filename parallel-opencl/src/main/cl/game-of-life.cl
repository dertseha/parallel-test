__kernel
void golKernel(__read_only image2d_t input, __write_only image2d_t output) {
    const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | // pixel coordinates
         CLK_ADDRESS_REPEAT | // wrap around coordinates
         CLK_FILTER_NEAREST;
    int2 coord = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, smp, coord);
    write_imageui(output, coord, pixel);
}
