/* stock ticker - bullish OHLC */
#define SOKOL_IMPL
#define SOKOL_GFX_IMPL
#define SOKOL_GLCORE

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#include "2-quad.glsl.h"

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
		-0.25f, -0.5f, 0.0f,	// 0: Open - left corner
		0.25f, -0.5f, 0.0f,		// 1: Open - righet corner
		-0.25f, 0.5f, 0.0f,		// 2: Close - left corner
		0.25f, 0.5f,	0.0f,		// 3: Close - right corner
		0.0f, 0.0f, 0.0f,			// 4: center
		0.0f, 0.7f, 0.0f, 		// 5: High 
		0.0f, -0.7f, 0.0f 		// 6: Low
	};
	// for quad,
	sg_buffer_desc buffer_desc = {
		.size = sizeof(triangle_vertices),
		.data = SG_RANGE(triangle_vertices),
		.label = "quad_vertices"
	};
	state.bind.vertex_buffers[0] = sg_make_buffer(&buffer_desc);
	uint16_t indices[] = {
		0, 1, 2,
		2, 3, 1
	};

	state.bind.vertex_buffers[0] = sg_make_buffer(&buffer_desc);
	uint16_t indices[] = {
		0,5,
		0,6
	};
	buffer_desc = {	
		.size = sizeof(indices),
		.usage = {
			.index_buffer = true,
			.immutable = true
		},	
		.data = SG_RANGE(indices),
		.label = "ticker_indices"
	};
	state.bind.index_buffer = sg_make_buffer(&buffer_desc);
	sg_pipeline_desc pipeline_desc = {
		.shader = shd,
		.layout = {
			.attrs = {
				[0] = {.format = SG_VERTEXFORMAT_FLOAT3}
			}
		},
		.primitive_type = SG_PRIMITIVETYPE_LINES,
		.index_type = SG_INDEXTYPE_UINT16,
		.label = "ticker_position"
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
	sg_draw(0, 16, 1);
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
    .window_title = "Stock Ticker"
  };
}

