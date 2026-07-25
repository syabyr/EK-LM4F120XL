#******************************************************************************
#
# Makefile - LVGL demo for LM4F120H5QR + ST7789V 240x240 + IT7269 touch
#
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=LM4F120H5QR

#
# Set the processor variant.
#
VARIANT=cm4f

#
# The base directory for StellarisWare.
#
ROOT=../../..

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find header files that do not live in the source directory.
#
IPATH=../../.. \
      lvgl \
      lvgl/src \
      st7789 \
      .

#
# VPATH for finding source files in subdirectories
#
VPATH += st7789
VPATH += lvgl/src
VPATH += lvgl/src/core
VPATH += lvgl/src/display
VPATH += lvgl/src/indev
VPATH += lvgl/src/draw
VPATH += lvgl/src/draw/sw
VPATH += lvgl/src/draw/sw/blend
VPATH += lvgl/src/draw/convert
VPATH += lvgl/src/libs/bin_decoder
VPATH += lvgl/src/font
VPATH += lvgl/src/font/fmt_txt
VPATH += lvgl/src/misc
VPATH += lvgl/src/misc/cache
VPATH += lvgl/src/misc/cache/class
VPATH += lvgl/src/misc/cache/instance
VPATH += lvgl/src/stdlib
VPATH += lvgl/src/stdlib/builtin
VPATH += lvgl/src/osal
VPATH += lvgl/src/tick
VPATH += lvgl/src/layouts
VPATH += lvgl/src/layouts/flex
VPATH += lvgl/src/themes
VPATH += lvgl/src/themes/default
VPATH += lvgl/src/widgets/arc
VPATH += lvgl/src/widgets/bar
VPATH += lvgl/src/widgets/button
VPATH += lvgl/src/widgets/buttonmatrix
VPATH += lvgl/src/widgets/image
VPATH += lvgl/src/widgets/label
VPATH += lvgl/src/widgets/slider
VPATH += lvgl/src/widgets/spinner
VPATH += lvgl/src/widgets/switch
VPATH += lvgl/src/widgets/win

#
# The default rule, which causes the lvgl_demo to be built.
#
all: ${COMPILER}
all: ${COMPILER}/lvgl_demo.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

flash:
	lm4flash ${COMPILER}/lvgl_demo.bin

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

# ==========================================================================
# Object file dependencies
# All .c files are found via VPATH; object names are just the basename
# ==========================================================================

# Application + porting layer
${COMPILER}/lvgl_demo.axf: ${COMPILER}/main.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_port_disp.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_port_indev.o

# Board-level drivers (from st7789/)
${COMPILER}/lvgl_demo.axf: ${COMPILER}/startup_gcc.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/st7789.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/st7789v.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/st7789_wrapper.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/spi_wrapper.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/i2c_wrapper.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/it7259_wrapper.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/uart_wrapper.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/retargetserial.o

# LVGL core
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_init.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_group.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_class.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_draw.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_event.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_pos.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_property.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_scroll.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_style.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_style_gen.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_obj_tree.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_refr.o

# LVGL display + indev
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_display.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_indev.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_indev_scroll.o

# LVGL draw (core + sw + blend + convert)
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_arc.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_buf.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_image.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_label.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_line.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_mask.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_rect.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_triangle.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_blur.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_image_decoder.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_buf_convert.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_arc.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_border.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blur.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_box_shadow.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_fill.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_grad.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_img.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_letter.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_line.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_mask.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_mask_rect.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_transform.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_triangle.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_utils.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_vector.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend_to_rgb565.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend_to_rgb565_swapped.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend_to_rgb888.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend_to_argb8888.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend_to_argb8888_premultiplied.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend_to_a8.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_draw_sw_blend_to_i1.o

# LVGL fonts
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_font.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_font_fmt_txt.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_font_montserrat_14.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_font_montserrat_16.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_font_montserrat_20.o

# LVGL libs
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_bin_decoder.o

# LVGL misc
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_anim.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_anim_timeline.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_area.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_array.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_color.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_color_op.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_event.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_fs.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_grad.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_ll.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_log.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_lru.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_math.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_palette.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_style.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_style_gen.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_text.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_timer.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_utils.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_tree.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_rb.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_circle_buf.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_iter.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_pending.o

# LVGL misc/cache
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_cache.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_cache_entry.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_cache_lru_ll.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_cache_lru_rb.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_image_cache.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_image_header_cache.o

# LVGL stdlib (builtin)
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_mem.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_mem_core_builtin.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_sprintf_builtin.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_string_builtin.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_tlsf.o

# LVGL osal
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_os.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_os_none.o

# LVGL tick
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_tick.o

# LVGL layouts
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_layout.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_flex.o

# LVGL themes
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_theme.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_theme_default.o

# LVGL widgets
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_arc.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_bar.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_button.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_buttonmatrix.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_image.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_label.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_slider.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_spinner.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_switch.o
${COMPILER}/lvgl_demo.axf: ${COMPILER}/lv_win.o

# StellarisWare driver library
${COMPILER}/lvgl_demo.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a

# Linker script
${COMPILER}/lvgl_demo.axf: lm4f120.ld

SCATTERgcc_lvgl_demo=lm4f120.ld
ENTRY_lvgl_demo=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA1 -DPART_LM4F120H5QR -DLV_CONF_PATH=\"lv_conf.h\"

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
