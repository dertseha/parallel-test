bool isAliveAt(__read_only image2d_t input, int2 coord)
{
    const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | // pixel coordinates
         CLK_ADDRESS_REPEAT | // wrap around coordinates
         CLK_FILTER_NEAREST;
   uint4 pixel = read_imageui(input, smp, coord);
   return pixel[0] > 0;
}

int aliveCountAt(__read_only image2d_t input, int2 coord)
{
   return isAliveAt(input, coord) ? 1 : 0;
}

__kernel
void golKernel(__read_only image2d_t input, __write_only image2d_t output) {
    int x = get_global_id(0);
    int y = get_global_id(1);
    bool selfAlive = isAliveAt(input, (int2)(x, y));
    int aliveNeighbors =
        aliveCountAt(input, (int2)(x - 1, y - 1)) + aliveCountAt(input, (int2)(x, y - 1)) + aliveCountAt(input, (int2)(x + 1, y - 1)) +
        aliveCountAt(input, (int2)(x - 1, y)) + aliveCountAt(input, (int2)(x + 1, y)) +
        aliveCountAt(input, (int2)(x - 1, y + 1)) + aliveCountAt(input, (int2)(x, y + 1)) + aliveCountAt(input, (int2)(x + 1, y + 1));
    bool shallBeAlive = (selfAlive && (aliveNeighbors == 2 || aliveNeighbors == 3)) || (!selfAlive && (aliveNeighbors == 3));
    uint value = shallBeAlive ? 255 : 0;
    uint4 pixel = (uint4)(value, value, value, value);
    write_imageui(output, (int2)(x, y), pixel);
}
