#include <bgfx_compute.sh>

// Read/write buffers
BUFFER_RW(PositionBuffer, vec4, 0);
BUFFER_RO(VelocityBuffer, vec4, 1);

NUM_THREADS(64, 1, 1) // local group size
void main()
{
    uint i = gl_GlobalInvocationID.x;
    PositionBuffer[i] += VelocityBuffer[i] * 0.01;
}