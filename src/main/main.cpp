/* instanced arrays */
#define HANDMADE_MATH_IMPLEMENTATION
#include "header/HandmadeMath.h"

#define SOKOL_IMPL
#define SOKOL_GFX_IMPL
#define SOKOL_GLCORE

#include "header/sokol_app.h"
#include "header/sokol_gfx.h"
#include "header/sokol_glue.h"
#include "header/sokol_log.h"
#include "header/2-instanced-arrays.glsl.h"

static struct {
	sg_pipeline pip;
	sg_bindings bind;
	sg_pass_action pass_action;
} state;

static void init (void) {

	sg_desc desc = {
		.logger = {.func = slog_func},
		.environment = sglue_environment()
	};
	sg_setup(&desc);

	sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));	
	float vertices[] = {
	 /* positions         	*/
    -0.05f, 0.05f,    // top left
    0.05f, -0.05f,    // bottom right
    -0.05f,  -0.05f,	// bottom left

		-0.05f, 0.05f, 		// top left
		0.05f, -0.05f, 		// bottom right
    0.05f,  0.05f   	// top right
	};

	sg_buffer_desc buffer_desc = { // loading up vertex data into vertex buffer object (VBO)
		.size = sizeof(vertices),
		.data = SG_RANGE(vertices),
		.label = "quad_vertices"
	};
	state.bind.vertex_buffers[0] = sg_make_buffer(&buffer_desc);

	// load offset data directly into vertex buffer object (VBO)
	HMM_Vec2 translations[100];
	int index = 0; 
	float offset = 0.1f;
	for (int y=-10; y<10; y+=2) {
		for (int x=-10; x<10; x+=2) {
			float x_pos = (float)x /10.0f + offset;
			float y_pos = (float)y /10.0f + offset;
			translations[index++] = HMM_V2(x_pos, y_pos);
		}
	}
	buffer_desc = {}; // reset buffer struct
	buffer_desc = {
		.size = sizeof(translations),
		.data = SG_RANGE(translations),
		.label = "offsets"
	};
	state.bind.vertex_buffers[1] = sg_make_buffer(&buffer_desc); 

	sg_pipeline_desc pipeline_desc = {
		.shader = shd,
		.layout = {
			.buffers = {
				[0] = {.stride = 0, .step_func = SG_VERTEXSTEP_PER_VERTEX, .step_rate = 1},
				[1] = {.stride = 0, .step_func = SG_VERTEXSTEP_PER_INSTANCE, .step_rate = 1}
			},
			.attrs = {
				[0] = {.buffer_index = 0, .offset = 0, .format = SG_VERTEXFORMAT_FLOAT2},
				[1] = {.buffer_index = 1, .offset = 0, .format = SG_VERTEXFORMAT_FLOAT2}
			}
		},
		.primitive_type = SG_PRIMITIVETYPE_TRIANGLES,
		.label = "quad_pipeline"
	};
	state.pip = sg_make_pipeline(&pipeline_desc); 
	state.pass_action = (sg_pass_action){};
	state.pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
	state.pass_action.colors[0].clear_value = {0.2f, 0.3f, 0.3f, 1.0f};
}

void frame(void) {

	sg_pass pass {
		.action = state.pass_action,
		.swapchain = sglue_swapchain()
	};
	sg_begin_pass(&pass);
	sg_apply_pipeline(state.pip);
	sg_apply_bindings(&state.bind);

	sg_draw(0, 6, 100);
	sg_end_pass();
	sg_commit();
}
	
void cleanup(void) {
	sg_shutdown();
}

void event(const sapp_event* e) {
	if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
		if (e->key_code == SAPP_KEYCODE_ESCAPE) {
			sapp_request_quit();
		}
	}
}

sapp_desc sokol_main(int argc, char *argv[]) {
  return (sapp_desc) {
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .event_cb = event,
    .width = 800,
    .height = 600,
    .high_dpi = true,
    .window_title = "Instancing..."
  };
}



