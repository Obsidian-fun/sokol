/* creating instanced offsets using uniforms */
#define HANDMADE_MATH_IMPLEMENTATION
#include "header/HandmadeMath.h"

#define SOKOL_IMPL
#define SOKOL_GFX_IMPL
#define SOKOL_GLCORE

#include "header/sokol_app.h"
#include "header/sokol_gfx.h"
#include "header/sokol_glue.h"
#include "header/sokol_log.h"
#include "header/sokol_time.h"
#include "header/1-instancing.h" //insert shader

static struct {
	sg_pipeline pip;
	sg_bindings bind;
	sg_pass_action pass_action;
	HMM_Vec4 translations[100];
} state;

static void init (void) {
	stm_setup(); // setup sokol time

	sg_desc desc = {
		.logger = {.func = slog_func},
		.environment = sglue_environment()
	};
	sg_setup(&desc);

	sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));	
	float vertices[] = {
	 /* positions         	*/
    -0.05f, -0.07f, 0.0f,   // bottom left
    0.05f, -0.07f, 0.0f,    // bottom right
    0.05f,  0.07f, 0.0f,	  // top right
													//
		-0.05f, -0.07f, 0.0f,		// bottom left
    0.05f,  0.07f, 0.0f,	  // top right
		-0.05f, 0.07f, 0.0f			// top left
	};
	sg_buffer_desc buffer_desc = { // loading up vertex data from buffer object
		.size = sizeof(vertices),
		.data = SG_RANGE(vertices),
		.label = "quad_vertices"
	};
	state.bind.vertex_buffers[0] = sg_make_buffer(&buffer_desc);

	sg_pipeline_desc pipeline_desc = {
		.shader = shd,
		.layout = {
			.attrs = {
				[0] = {.format = SG_VERTEXFORMAT_FLOAT2}
			}
		},
		.primitive_type = SG_PRIMITIVETYPE_TRIANGLES,
		.index_type = SG_INDEXTYPE_UINT16,
		.label = "quad_position"
	};
	state.pip = sg_make_pipeline(&pipeline_desc); 
	state.pass_action = (sg_pass_action){};
	state.pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
	state.pass_action.colors[0].clear_value = {0.2f, 0.3f, 0.3f, 1.0f};

	int index = 0; 
	float offset = 0.0f;
	for (int y=-10; y<10; y +=2) {
		for (int x=-10; x<10; x+=2) {
			float x_pos = (float)x /10.0f + offset;
			float y_post = (float)y /10.0f + offset;
			state.translations[index++] = HMM_V4(x_pos, y_pos, 0.0, 0.0); 
		}
	}
}

void frame(void) {

	sg_pass pass {
		.action = state.pass_action,
		.swapchain = sglue_swapchain()
	};
	sg_begin_pass(&pass);
	sg_apply_pipeline(state.pip);
	sg_apply_bindings(&state.bind);

	vs_params_t vs_params;
	std::memcpy(vs_params.offsets, state.translations, sizeof(vs_params.offsets));
	sg_apply_uniforms(UB_vs_params, &SG_RANGE(vs_params));

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



