
#define SOKOL_IMPL
#define SOKOL_GFX_IMPL
#define SOKOL_GLCORE

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"
// shader goes here

static struct {
	sg_pipeline pip;
	sg_bindings bind;
	sg_pass_action pass_action;
} state;

static void init (void) {
	sg_desc desc = {
		.logger = {.func = slog_func},
		.environment = sglue_environment
	};
	sg_setup(sg_desc);
	sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // botton left
		0.5f, -0.5f,	0.0f, // bottom right
		0.5f,	0.5f,		0.0f, // top right
		-0.5f, 0.5f,	0.0f  // top left
	}








}


