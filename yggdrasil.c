#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <leif/leif.h>

typedef enum {
  ALL = 0,
  IN_PROGRESS,
  COMPLETED,
  LOW,
  MEDIUM,
  HIGH
} entry_filter;

#define WIN_MARGIN 20.0f

static int winw = 1280, winh = 720;
static LfFont titlefont;
static entry_filter current_filter;

static void rendertopbar() {
  lf_push_font(&titlefont);
  lf_text("Yggdrasil - Todo");
  lf_pop_font();

  {
    const float width = 160.0f;

    lf_set_ptr_x_absolute(winw - width - WIN_MARGIN * 2.0f);
    LfUIElementProps props = lf_get_theme().button_props;
    props.margin_left = 0.0f; props.margin_right = 0.0f;
    props.color = (LfColor){65, 167, 204, 255};
    props.border_width = 0.0f; props.corner_radius = 4.0f;
    lf_push_style_props(props);
    lf_set_line_should_overflow(false);
    lf_button_fixed("New Task", 140, -1);
    lf_set_line_should_overflow(true);
    lf_pop_style_props();
  }
}

static void renderfilters() {
  uint32_t numfilters = 6;
  static const char* filters[] = {"ALL", "IN PROGRESS", "COMPLETED", "LOW", "MEDIUM", "HIGH"};

  LfUIElementProps props = lf_get_theme().button_props;
  props.color = LF_NO_COLOR;
  props.margin_left = 10.0f;
  props.margin_right = 10.0f;
  props.margin_top = 10.0f;
  props.padding = 10.0f;
  props.border_width = 0.0f;
  props.color = LF_NO_COLOR;
  props.text_color = LF_WHITE;
  props.corner_radius = 8.0f;

  float width = 0.0f;
  float ptrx_before = lf_get_ptr_x();
  float ptry_before = lf_get_ptr_y();
  lf_push_style_props(props);
  lf_set_no_render(true);
  lf_set_ptr_y_absolute(lf_get_ptr_y() + 50.0f);
  for(uint32_t i = 0; i < numfilters; i++) {
    lf_button(filters[i]); 
  }
  lf_set_no_render(false);
  lf_set_ptr_y_absolute(ptry_before);

  width = lf_get_ptr_x() - ptrx_before - props.margin_right - props.padding;

  lf_set_ptr_x_absolute(winw - width - WIN_MARGIN * 2.0f);

  lf_set_line_should_overflow(false);
  for(uint32_t i = 0; i < numfilters; i++) {
    props.color = (current_filter == (entry_filter)i) ? (LfColor){255, 255, 255, 50} : LF_NO_COLOR;
    lf_push_style_props(props);
    if(lf_button(filters[i]) == LF_CLICKED) {
      current_filter = (entry_filter)i;
    }
    lf_pop_style_props();
  }
  lf_set_line_should_overflow(true);
}

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(winw, winh, "Todo", NULL, NULL);

    glfwMakeContextCurrent(window);

    lf_init_glfw(winw, winh, window);

    LfTheme theme = lf_default_theme();
    theme.div_props.color = LF_NO_COLOR;
    lf_set_theme(theme);

    titlefont = lf_load_font("./fonts/inter-bold.ttf", 30);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

        lf_begin();

        lf_div_begin(((vec2s){WIN_MARGIN, WIN_MARGIN}), 
                      ((vec2s){winw - WIN_MARGIN * 2.0f, winh - WIN_MARGIN * 2.0f}),
        true);

        rendertopbar();
        lf_next_line();
        renderfilters();

        lf_div_end();
        lf_end();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    lf_free_font(&titlefont);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
