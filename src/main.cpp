
#define SOKOL_IMPL
#define SOKOL_GFX_IMPL
#define SOKOL_GLCORE

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

using namespace std;

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

 /* create shader from code-generated sg_shader_desc */
	sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));

 /* a vertex buffer with 3 vertices */
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f,		// bottom right
		0.0f, 0.5f, 0.0f		// top 
	}

	state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc) {
		.size = sizeof(vertices),
		.data = SG_RANGE(vertices),
		.label = "triangle_vertices"
	});


	state.pass_action = (sg_pass_action){};
	state.pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
	state.pass_action.colors[0].clear_value = {0.2f, 0.3f, 0.3f, 1.0f};
}

void frame (void) {
	sg_pass pass = {
		.action = state.pass_action,
		.swapchain = sglue_swapchain()
	};
	sg_begin_pass(&pass); 
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
		.window_title = "sokol app"
	};
}








