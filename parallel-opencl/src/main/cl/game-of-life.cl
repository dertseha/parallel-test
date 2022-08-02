bool isAliveAt(__read_only image2d_t input, float2 size, int2 coord)
{
    const sampler_t smp = CLK_NORMALIZED_COORDS_TRUE | // normalized coordinates
         CLK_ADDRESS_REPEAT | // wrap around coordinates, requires normalized coordinates
         CLK_FILTER_NEAREST;
    uint4 pixel = read_imageui(input, smp, (float2)(((float)(coord.x) + 0.5) / size.x, ((float)(coord.y) + 0.5) / size.y));
    return pixel.x > 0;
}

int aliveCountAt(__read_only image2d_t input, float2 size, int2 coord)
{
    return isAliveAt(input, size, coord) ? 1 : 0;
}

__kernel
void golKernel(__read_only image2d_t input, __write_only image2d_t output) {
    float2 size = (float2)((float)(get_image_width(input)), (float)(get_image_height(input)));
    int x = get_global_id(0);
    int y = get_global_id(1);
    bool selfAlive = isAliveAt(input, size, (int2)(x, y));
    int aliveNeighbors =
        aliveCountAt(input, size, (int2)(x - 1, y - 1)) + aliveCountAt(input, size, (int2)(x, y - 1)) + aliveCountAt(input, size, (int2)(x + 1, y - 1)) +
        aliveCountAt(input, size, (int2)(x - 1, y)) + aliveCountAt(input, size, (int2)(x + 1, y)) +
        aliveCountAt(input, size, (int2)(x - 1, y + 1)) + aliveCountAt(input, size, (int2)(x, y + 1)) + aliveCountAt(input, size, (int2)(x + 1, y + 1));
    bool shallBeAlive = (selfAlive && (aliveNeighbors == 2 || aliveNeighbors == 3)) || (!selfAlive && (aliveNeighbors == 3));
    uint value = shallBeAlive ? 255 : 0;
    uint4 pixel = (uint4)(value, value, value, value);
    write_imageui(output, (int2)(x, y), pixel);
}
