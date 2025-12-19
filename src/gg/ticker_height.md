In modern OpenGL (and in **sokol_gfx**, which is a thin wrapper around OpenGL/GL3/GLES3/Metal/D3D11/WGPU), you can easily make **each instance** have its own height (scale), color, or any other per-instance data you want — this is exactly what **instanced attributes** (also called per-instance vertex attributes) are for.

Here’s how you typically do it with sokol_gfx (and the same idea applies directly in OpenGL):

### 1. Define per-instance vertex attributes

You usually have two kinds of data:
- Per-vertex data (shared by all instances): position, uv, normal of the base mesh (e.g., a unit quad or bar)
- Per-instance data (unique per instance): model matrix, offset, scale/height, color, etc.

Example vertex layout in sokol_gfx (C, but the concept is identical in the Rust or Zig bindings):

```c
sg_buffer_layout_desc vertex_layout = {
    .buffers[0] = { .stride = 4*sizeof(float) },  }, // positions + uvs of the base mesh
    .attrs = {
        [0] = { .format = SG_VERTEXFORMAT_FLOAT3, .buffer_index = 0 }, // position
        [1] = { .format = SG_VERTEXFORMAT_FLOAT2, .buffer_index = 0 }, // uv
        // per-instance attributes start here
        [2] = { .format = SG_VERTEXFORMAT_FLOAT3, .buffer_index = 1, .instancing = true }, // instance offset (or base position)
        [3] = { .format = SG_VERTEXFORMAT_FLOAT,  .buffer_index = 1, .instancing = true }, // instance height (scale.y)
        [4] = { .format = SG_VERTEXFORMAT_FLOAT4, .buffer_index = 1, .instancing = true }, // instance color (or rgba)
    }
};
```

You can pack multiple per-instance values into one buffer (buffer_index = 1) and mark them with `.instancing = true`.

### 2. Upload per-instance data every frame (or when it changes)

```c
typedef struct {
    float offset_x, offset_y, offset_z;  // or just float2 if 2D
    float height;                            // scale in Y direction
    uint32_t color_rgba8;                // or float4
    // padding to 16-byte if needed
} instance_data_t;

instance_data_t instance_data[MAX_INSTANCES];
// fill with your stock ticker data:
//   instance_data[i].offset_x = ticker_x_position;
//   instance_data[i].height    = normalized_price;
//   instance_data[i].color_rgba8 = is_green ? 0xFF00FF00 : 0xFF0000FF;

sg_update_buffer(instance_buffer, &(sg_range){
    .ptr = instance_data,
    .size = num_tickers * sizeof(instance_data_t)
});
```

### 3. In the vertex shader, use the per-instance data

GLSL example:

```glsl
#version 330
layout(location = 0) in vec3 a_position;     // base mesh vertex
layout(location = 1) in vec2 a_uv;

layout(location = 2) in vec3 a_instance_offset;
layout(location = 3) in float a_instance_height;
layout(location = 4) in vec4 a_instance_color;

out vec4 v_color;
out vec2 v_uv;

uniform mat4 u_mvp;

void main() {
    vec3 pos = a_position;
    pos.y *= a_instance_height;                 // stretch the bar vertically
    pos += a_instance_offset;                   // translate to its place

    gl_Position = u_mvp * vec4(pos, 1.0);
    v_color = a_instance_color;
    // or compute red/green from a boolean
    v_uv = a_uv;
}
```

If you only have a boolean (up/down), you can pack it as a float (0.0 or 1.0) or as part of the color already.

### 4. Draw all instances in one call

```c
sg_draw(base_vertex, index_count, num_tickers);  // third argument = instance count
```

That’s it — a single draw call renders thousands of differently positioned, differently tall, differently colored bars.

### Alternative common patterns

- Use a `mat4 instance_model_matrix` as per-instance data → full transform freedom
- Pack offset + scale + color into a single `vec4` or two if you want to save attributes
- Use a storage buffer (SSBO) or uniform buffer with `gl_InstanceID` indexing if you have > ~16 attributes or very dynamic data (sokol_gfx also supports this)

### Summary for your stock ticker use-case

Yes — you can 100% give each ticker bar:
- Its own X position
- Its own height (price)
- Its own color (green/red boolean → encode as float4 color)

And still render all of them with **one single draw call** using GPU instancing in sokol_gfx.

This is actually one of the most common and efficient ways to draw financial charts, particle systems, UI elements, vegetation, etc.

