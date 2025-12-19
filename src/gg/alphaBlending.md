In trading chart UIs (and many financial platforms like TradingView, Thinkorswim, Sierra Chart, Bookmap, etc.), the nice “faint colored boxes that sit on top of bright green/red candles or volume bars” effect is almost always achieved through **alpha blending** (transparency) combined with careful choice of color and sometimes a slight blur or multiply blend mode.

Here’s exactly how you would do it with **sokol_gfx.h** (the lightweight cross-platform rendering layer by Andre Weissflog):

### 1. Enable alpha blending in the pipeline state

```c
sg_pipeline_desc pip_desc = {0};
// ... vertex shader, layout, etc. ...

pip_desc.colors[0].blend.enabled = true;
pip_desc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
pip_desc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
pip_desc.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_ONE;
pip_desc.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

// Optional: makes very dark backgrounds look better
// pip_desc.colors[0].blend.op_rgb = SG_BLENDOP_ADD;
// pip_desc.colors[0].blend.op_alpha = SG_BLENDOP_ADD;

sg_pipeline pip = sg_make_pipeline(&pip_desc);
```

This is the classic premultiplied-style alpha blending that 99 % of UIs use.

### 2. Draw your candles/volume bars first (opaque or nearly opaque)

```c
// Bright bullish green
float bullish_bright[] = { 0.00f, 1.00f, 0.20f, 1.0f };  // #00FF33
// Bright bearish red
float bearish_bright[] = { 1.00f, 0.10f, 0.10f, 1.0f };  // #FF1919

// set uniform or push constant with this color
sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &SG_RANGE(bullish_bright));
// then draw your candle rectangles
```

### 3. Then draw the semi-transparent overlay boxes

```c
// Light / faint bullish overlay
float bullish_faint[] = { 0.15f, 0.90f, 0.35f, 0.27f };   // ≈ 27% opacity

// Light / faint bearish overlay
float bearish_faint[] = { 1.00f, 0.30f, 0.30f, 0.22f };   // ≈ 22% opacity

sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &SG_RANGE(bullish_faint));
sg_apply_draw_call_for_your_box_geometry();
```

That’s literally all you need. The hardware alpha blending will automatically make the faint green box brighten the bright green candle underneath it, and the faint red box will darken/warm the bright red candle — exactly the pleasant look you see on professional platforms.

### Extra polish tricks used by pro trading UIs

| Trick                        | How to do it in sokol_gfx                                                                                 |
|--------------------------------|-------------------------------------------------------------------------------------------------------------------|
| Slightly glowing effect        | Use a higher alpha (0.4-0.5) + slightly more saturated color than the candle itself                            |
| Multiply instead of normal blend (darker shadow feel for bearish zones) | Change blend factors to `SRC_ALPHA`, `ONE_MINUS_SRC_COLOR` or just use `SG_BLENDFACTOR_SRC_COLOR` etc.         |
| Soft edges on the box          | Render the box slightly larger into an offscreen buffer with Gaussian blur, then blend that back (overkill for most of the time) |
| Pulse animation on new signal  | Animate the alpha of the overlay between 0.15 ↔ 0.45 over 1–2 seconds                                        |

### Minimal complete example snippet

```c
sg_begin_default_pass(&pass_action, w, h);

// 1. Draw all candles / volume bars (opaque bright colors)
draw_candles();

// 2. Draw all signal boxes (faint colors with alpha)
sg_apply_pipeline(overlay_pipe);  // the one with blending enabled

for (each signal) {
    float color[4] = { r, g, b, signal.is_bullish ? 0.15f:1.0f,
                                 signal.is_bullish ? 0.90f:0.30f,
                                 signal.is_bullish ? 0.35f:0.30f,
                                 signal.is_bullish ? 0.27f:0.22f };
    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_color, &SG_RANGE(color));
    sg_draw(... box vertices ...);
}

sg_end_pass();
sg_commit();
```

That’s exactly how TradingView, Bookmap, Jigsaw, etc. achieve that clean overlapping color effect — just simple alpha-blended quads on top of bright candlesticks, nothing fancier required.

So in short:  
Enable standard alpha blending → draw bright candles → draw faint
semi-transparent boxes on top → done. sokol_gfx makes this trivial.
