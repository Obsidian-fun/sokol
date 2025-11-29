/* shaders - in and out , part 1*/
#define SOKOL_IMPL
#define SOKOL_GFX_IMPL
#define SOKOL_GLCORE

#include "iostream"
#include "iomanip"
#include "header/sokol_app.h"
#include "header/sokol_gfx.h"
#include "header/sokol_glue.h"
#include "header/sokol_log.h"
#include "header/sokol_time.h"
#include "header/3-attributes.glsl.h"

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

	stm_setup(); // setup sokol_time

	sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));	
	float vertices[] = {
	 /* positions         	colors*/
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,    // bottom left, red
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    // bottom right, green
    0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f     // top, blue
	};
	sg_buffer_desc buffer_desc = { // loading up vertex data from buffer object
		.size = sizeof(vertices),
		.data = SG_RANGE(vertices),
		.label = "triangle_vertices"
	};
	state.bind.vertex_buffers[0] = sg_make_buffer(&buffer_desc);

	sg_pipeline_desc pipeline_desc = {
		.shader = shd,
		.layout = {
			.attrs = {
				[0] = {.format = SG_VERTEXFORMAT_FLOAT3},
				[1] = {.format = SG_VERTEXFORMAT_FLOAT3}
			}
		},
		.primitive_type = SG_PRIMITIVETYPE_TRIANGLES,
		.label = "triangle_position"
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

//	float now = (float) stm_sec(stm_now());
//	float greenValue = (sinf(now)/2.0f);	
//	cout<<fixed<<setprecision(6)<<now<<endl;
//	fs_params_t fs_params = {
//		.ourColor = {0.1f, greenValue, 0.0f, 1.0f}
//	};
//	sg_range range = {&fs_params, sizeof(fs_params)};
//	sg_apply_uniforms(UB_fs_params, range); 

	sg_draw(0, 3, 1);

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



